/*
 Copyright 2018 Google Inc.

 Licensed under the Apache License, Version 2.0 (the "License");
 you may not use this file except in compliance with the License.
 You may obtain a copy of the License at

 http://www.apache.org/licenses/LICENSE-2.0

 Unless required by applicable law or agreed to in writing, software
 distributed under the License is distributed on an "AS IS" BASIS,
 WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 See the License for the specific language governing permissions and
 limitations under the License.
*/

#include "gfr.h"

#if defined(WIN32)
// For OutputDebugString
#include <process.h>
#include <windows.h>
#endif

#ifdef __linux__
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <sys/un.h>
#endif

#include <algorithm>
#include <chrono>
#include <fstream>
#include <iostream>
#include <memory>
#include <sstream>

#include "logger.h"
#include "util.h"

#if defined(WIN32)
#include <direct.h>
#endif

namespace gfr {

const char* kGfrVersion = "1.0.2";

const char* k_env_var_output_path = "GFR_OUTPUT_PATH";
const char* k_env_var_output_name = "GFR_OUTPUT_NAME";

const char* k_env_var_log_configs = "GFR_DEBUG_LOG_CONFIGS";
const char* k_env_var_debug_dump_on_begin = "GFR_DEBUG_DUMP_ON_BEGIN";
const char* k_env_var_trace_on = "GFR_TRACE_ON";
const char* k_env_var_debug_autodump = "GFR_AUTODUMP";
const char* k_env_var_dump_all_command_buffers = "GFR_DUMP_ALL_COMMAND_BUFFERS";
const char* k_env_var_track_semaphores = "GFR_TRACK_SEMAPHORES";
const char* k_env_var_trace_all_semaphores = "GFR_TRACE_ALL_SEMAPHORES";
const char* k_env_var_instrument_all_commands = "GFR_INSTRUMENT_ALL_COMMANDS";
const char* k_env_var_validate_command_buffer_state =
    "GFR_VALIDATE_COMMAND_BUFFER_STATE";

const char* k_env_var_debug_shaders_dump = "GFR_SHADERS_DUMP";
const char* k_env_var_debug_shaders_dump_on_crash = "GFR_SHADERS_DUMP_ON_CRASH";
const char* k_env_var_debug_shaders_dump_on_bind = "GFR_SHADERS_DUMP_ON_BIND";

const char* k_env_var_debug_buffers_dump_indirect = "GFR_BUFFERS_DUMP_INDIRECT";

const char* k_env_var_watchdog_timeout = "GFR_WATCHDOG_TIMEOUT_MS";

const char* k_env_var_driver_hang = "GFR_DRIVER_HANG";

#if defined(WIN32)
const char* k_path_separator = "\\";
#else
const char* k_path_separator = "/";
#endif

namespace {
void MakeDir(const std::string& path) {
#if defined(WIN32)
  int mkdir_result = _mkdir(path.c_str());
#else
  int mkdir_result = mkdir(path.c_str(), ACCESSPERMS);
#endif

  if (mkdir_result && EEXIST != errno) {
    std::cerr << "GFR: Error creating output directory \'" << path
              << "\': " << strerror(errno) << std::endl;
  }
}
}  // namespace

// =============================================================================
// GfrContext
// =============================================================================
GfrContext::GfrContext() : intercept::BaseInterceptor() {
  // output path
  {
    char* p_env_value = getenv(k_env_var_output_path);
    if (p_env_value) {
      output_path_ = p_env_value;

      if (output_path_.back() != k_path_separator[0]) {
        output_path_ += k_path_separator;
      }
    } else {
#if defined(WIN32)
      output_path_ = getenv("USERPROFILE");
#else
      output_path_ = "/var/log";
#endif

      output_path_ += k_path_separator;
      output_path_ += "gfr";
      output_path_ += k_path_separator;
    }

    // ensure base path is created
    MakeDir(output_path_);

    // if output_name_ is given, don't create a subdirectory
    char* d_env_value = getenv(k_env_var_output_name);
    if (d_env_value) {
      output_name_ = d_env_value;
    } else {
      // calculate a unique sub directory based on time
      auto now = std::chrono::system_clock::now();
      auto in_time_t = std::chrono::system_clock::to_time_t(now);

      std::stringstream ss;
      ss << std::put_time(std::localtime(&in_time_t), "%Y-%m-%d-%H%M%S");
      output_path_ += ss.str();
      output_path_ += k_path_separator;
    }
  }

  // report gfr configs
  {
    char* p_env_value = getenv(k_env_var_log_configs);
    log_configs_ = (p_env_value != nullptr) && (std::atol(p_env_value) == 1);
    if (log_configs_) {
      configs_.push_back(std::string(k_env_var_log_configs) + "=1");
    }
  }

  // trace mode
  GetEnvVal<bool>(k_env_var_trace_on, &trace_all_);

  // setup shader loading modes
  shader_module_load_options_ = ShaderModule::LoadOptions::kNone;

  {
    bool dump_shaders = false;
    GetEnvVal<bool>(k_env_var_debug_shaders_dump, &dump_shaders);
    if (dump_shaders) {
      shader_module_load_options_ |= ShaderModule::LoadOptions::kDumpOnCreate;
    } else {
      // if we're not dumping all shaders then check if we dump in other cases
      {
        GetEnvVal<bool>(k_env_var_debug_shaders_dump_on_crash,
                        &debug_dump_shaders_on_crash_);
        if (debug_dump_shaders_on_crash_) {
          shader_module_load_options_ |=
              ShaderModule::LoadOptions::kKeepInMemory;
        }
      }

      {
        GetEnvVal<bool>(k_env_var_debug_shaders_dump_on_bind,
                        &debug_dump_shaders_on_bind_);
        if (debug_dump_shaders_on_bind_) {
          shader_module_load_options_ |=
              ShaderModule::LoadOptions::kKeepInMemory;
        }
      }
    }
  }

  // manage the watchdog thread
  {
    GetEnvVal<uint64_t>(k_env_var_watchdog_timeout, &watchdog_timer_ms_);

    last_submit_time_ =
        std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::high_resolution_clock::now().time_since_epoch())
            .count();

    if (watchdog_timer_ms_ > 0) {
      StartWatchdogTimer();
    }
  }

  // Manage the gpuhangd listener thread.
  {
    bool enable_driver_hang_thread = false;
    GetEnvVal<bool>(k_env_var_driver_hang, &enable_driver_hang_thread);

    if (enable_driver_hang_thread) {
      StartGpuHangdListener();
    }
  }
}

GfrContext::~GfrContext() {
  StopWatchdogTimer();
  StopGpuHangdListener();
}

template <class T>
void GfrContext::GetEnvVal(const char* name, T* value) {
  char* p_env_value = getenv(name);
  if (p_env_value) {
    if (log_configs_) {
      auto config = std::string(name) + "=" + std::string(p_env_value);
      if (std::find(configs_.begin(), configs_.end(), config) ==
          configs_.end()) {
        configs_.push_back(config);
      }
    }
    *value = std::atol(p_env_value);
  }
}

void GfrContext::StartWatchdogTimer() {
  // Start up the watchdog timer thread.
  watchdog_running_ = true;
  watchdog_thread_ =
      std::make_unique<std::thread>([&]() { this->WatchdogTimer(); });
}

void GfrContext::StopWatchdogTimer() {
  if (watchdog_running_ && watchdog_thread_->joinable()) {
    std::cerr << "GFR: Stopping Watchdog" << std::endl;
    watchdog_running_ = false;  // TODO: condition variable that waits
    watchdog_thread_->join();
    std::cerr << "GFR: Watchdog Stopped" << std::endl;
  }
}

void GfrContext::WatchdogTimer() {
  std::cerr << "GFR: Begin Watchdog: " << watchdog_timer_ms_ << "ms"
            << std::endl;

  uint64_t test_interval_us =
      std::min((uint64_t)(1000 * 1000), watchdog_timer_ms_ * 500);
  while (watchdog_running_) {
    // TODO: condition variable that waits
    std::this_thread::sleep_for(std::chrono::microseconds(test_interval_us));

    auto now = std::chrono::duration_cast<std::chrono::milliseconds>(
                   std::chrono::high_resolution_clock::now().time_since_epoch())
                   .count();
    auto ms = (int64_t)(now - last_submit_time_);

    if (ms > (int64_t)watchdog_timer_ms_) {
      std::cout << "GFR: Watchdog check failed, no submit in " << ms << "ms"
                << std::endl;

      DumpAllDevicesExecutionState();

      // Reset the timer to prevent constantly dumping the log.
      last_submit_time_ =
          std::chrono::duration_cast<std::chrono::milliseconds>(
              std::chrono::high_resolution_clock::now().time_since_epoch())
              .count();
    }
  }
}

void GfrContext::StartGpuHangdListener() {
#ifdef __linux__
  // Start up the hang deamon thread.
  gpuhangd_thread_ =
      std::make_unique<std::thread>([&]() { this->GpuHangdListener(); });
#endif  // __linux__
}

void GfrContext::StopGpuHangdListener() {
#ifdef __linux__
  if (gpuhangd_thread_ && gpuhangd_thread_->joinable()) {
    std::cerr << "GFR: Stopping Listener" << std::endl;
    if (gpuhangd_socket_ >= 0) {
      shutdown(gpuhangd_socket_, SHUT_RDWR);
    }
    gpuhangd_thread_->join();
    std::cerr << "GFR: Listener Stopped" << std::endl;
  }
#endif  // __linux__
}

void GfrContext::GpuHangdListener() {
#ifdef __linux__
  const char* kSocketName = "/run/gpuhangd";
  std::cerr << "GFR: Begin Listener: " << kSocketName << std::endl;

  gpuhangd_socket_ = socket(AF_LOCAL, SOCK_STREAM, 0);
  if (gpuhangd_socket_ < 0) {
    std::cerr << "GFR: Could not create socket: " << strerror(errno)
              << std::endl;
    return;
  }

  struct sockaddr_un addr = {};
  addr.sun_family = AF_LOCAL;
  addr.sun_path[0] = '\0';
  strncpy(addr.sun_path + 1, kSocketName, sizeof(addr.sun_path) - 2);

  int connect_ret = connect(gpuhangd_socket_, (const struct sockaddr*)&addr,
                            sizeof(struct sockaddr_un));
  if (connect_ret < 0) {
    std::cerr << "GFR: Could not connect socket: " << strerror(errno)
              << std::endl;
    return;
  }

  for (;;) {
    int msg = 0;
    int read_ret = read(gpuhangd_socket_, &msg, sizeof(int));
    if (read_ret < 0) {
      std::cerr << "GFR: Could not read socket: " << strerror(errno)
                << std::endl;
      break;
    } else if (0 == read_ret) {
      std::cerr << "GFR: Socket closed\n" << std::endl;
      break;
    }

    if (0x8badf00d == msg) {
      std::cerr << "GFR: Driver signalled a hang." << std::endl;
      DumpAllDevicesExecutionState();
    }
  }
#endif  // __linux__
}

void GfrContext::PreApiFunction(const char* api_name) {
  if (trace_all_) {
    std::cout << "> " << api_name << std::endl;
  }
}

void GfrContext::PostApiFunction(const char* api_name) {
  if (trace_all_) {
    std::cout << "< " << api_name << std::endl;
  }
}

const VkInstanceCreateInfo* GfrContext::GetModifiedInstanceCreateInfo(
    const VkInstanceCreateInfo* pCreateInfo) {
  instance_create_info_ = *pCreateInfo;
  instance_extension_names_ = std::make_unique<CStringArray>();
  instance_extension_names_->assign(pCreateInfo->ppEnabledExtensionNames,
                                    pCreateInfo->ppEnabledExtensionNames +
                                        pCreateInfo->enabledExtensionCount);

  bool requested_debug_report = false;
  for (uint32_t i = 0; i < pCreateInfo->enabledExtensionCount; ++i) {
    const char* name = pCreateInfo->ppEnabledExtensionNames[i];
    requested_debug_report =
        (strcmp(name, VK_EXT_DEBUG_REPORT_EXTENSION_NAME) == 0);
    if (requested_debug_report) {
      break;
    }
  }
  // Create persistent storage for the extension names
  if (!requested_debug_report) {
    instance_extension_names_->push_back(VK_EXT_DEBUG_REPORT_EXTENSION_NAME);
    instance_create_info_.enabledExtensionCount =
        static_cast<uint32_t>(instance_extension_names_->size());
    instance_create_info_.ppEnabledExtensionNames =
        instance_extension_names_->data();
  }
  return &instance_create_info_;
}

const VkDeviceCreateInfo* GfrContext::GetModifiedDeviceCreateInfo(
    VkPhysicalDevice physicalDevice, const VkDeviceCreateInfo* pCreateInfo) {
  bool requested_buffer_marker = false;
  for (uint32_t i = 0; i < pCreateInfo->enabledExtensionCount; ++i) {
    const char* name = pCreateInfo->ppEnabledExtensionNames[i];
    requested_buffer_marker =
        (strcmp(name, VK_AMD_BUFFER_MARKER_EXTENSION_NAME) == 0);
    if (requested_buffer_marker) {
      break;
    }
  }

  bool requested_coherent_memory = false;
  for (uint32_t i = 0; i < pCreateInfo->enabledExtensionCount; ++i) {
    const char* name = pCreateInfo->ppEnabledExtensionNames[i];
    requested_coherent_memory =
        (strcmp(name, "VK_AMD_device_coherent_memory") == 0);
    if (requested_coherent_memory) {
      break;
    }
  }

  // Keep a copy of extensions
  UniqueCStringArray extension_names = std::make_unique<CStringArray>();
  extension_names->assign(pCreateInfo->ppEnabledExtensionNames,
                          pCreateInfo->ppEnabledExtensionNames +
                              pCreateInfo->enabledExtensionCount);

  UniqueCStringArray original_extension_names = std::make_unique<CStringArray>();
  *original_extension_names = *extension_names;

  if (!requested_buffer_marker) {
    // Get available extensions and add buffer marker if possible
    bool has_buffer_marker = false;
    {
      uint32_t count = 0;
      VkResult vk_result = intercept::EnumerateDeviceExtensionProperties(
          physicalDevice, nullptr, &count, nullptr);
      if (vk_result == VK_SUCCESS) {
        std::vector<VkExtensionProperties> extension_properties(count);
        vk_result = intercept::EnumerateDeviceExtensionProperties(
            physicalDevice, nullptr, &count, extension_properties.data());
        if (vk_result == VK_SUCCESS) {
          for (const auto& properties : extension_properties) {
            if (strcmp(properties.extensionName,
                       VK_AMD_BUFFER_MARKER_EXTENSION_NAME) == 0) {
              has_buffer_marker = true;
              break;
            }
          }
        }
      }
    }

    if (has_buffer_marker) {
      extension_names->push_back(VK_AMD_BUFFER_MARKER_EXTENSION_NAME);
    } else {
      std::cerr << "GFR Warning: No VK_AMD_buffer_marker extension, "
                   "progression tracking will be disabled. "
                << std::endl;
    }
  }

  if (!requested_coherent_memory) {
    bool has_coherent_memory = false;
    {
      uint32_t count = 0;
      VkResult vk_result = intercept::EnumerateDeviceExtensionProperties(
          physicalDevice, nullptr, &count, nullptr);
      if (vk_result == VK_SUCCESS) {
        std::vector<VkExtensionProperties> extension_properties(count);
        vk_result = intercept::EnumerateDeviceExtensionProperties(
            physicalDevice, nullptr, &count, extension_properties.data());
        if (vk_result == VK_SUCCESS) {
          for (const auto& properties : extension_properties) {
            if (strcmp(properties.extensionName,
                       "VK_AMD_device_coherent_memory") == 0) {
              has_coherent_memory = true;
              break;
            }
          }
        }
      }
    }

    // Create persistent storage for the extension names
    if (has_coherent_memory) {
      extension_names->push_back("VK_AMD_device_coherent_memory");
    } else {
      std::cerr << "GFR Warning: No VK_AMD_device_coherent_memory extension, "
                   "results may not be as accurate as possible."
                << std::endl;
    }
  }

  auto device_create_info = std::make_unique<DeviceCreateInfo>();
  device_create_info->original_create_info = *pCreateInfo;
  device_create_info->original_create_info.enabledExtensionCount =
      static_cast<uint32_t>(original_extension_names->size());
  device_create_info->original_create_info.ppEnabledExtensionNames =
      original_extension_names->data();
  device_create_info->modified_create_info = *pCreateInfo;
  device_create_info->modified_create_info.enabledExtensionCount =
      static_cast<uint32_t>(extension_names->size());
  device_create_info->modified_create_info.ppEnabledExtensionNames =
      extension_names->data();
  auto p_modified_create_info = &(device_create_info->modified_create_info);
  {
    std::lock_guard<std::mutex> lock(device_create_infos_mutex_);
    device_create_infos_[p_modified_create_info] =
        std::move(device_create_info);
  }

  // Store the extension names
  device_extension_names_.push_back(std::move(extension_names));
  device_extension_names_.push_back(std::move(original_extension_names));

  return p_modified_create_info;
}

bool GfrContext::DumpShadersOnCrash() const {
  return debug_dump_shaders_on_crash_;
}

bool GfrContext::DumpShadersOnBind() const {
  return debug_dump_shaders_on_bind_;
}

void GfrContext::AddObjectInfo(VkDevice device, uint64_t handle,
                               ObjectInfoPtr info) {
  std::lock_guard<std::mutex> lock(devices_mutex_);
  if (devices_.find(device) != devices_.end()) {
    devices_[device]->AddObjectInfo(handle, std::move(info));
  }
}

std::string GfrContext::GetObjectName(VkDevice vk_device, uint64_t handle) {
  std::lock_guard<std::mutex> lock(devices_mutex_);
  if (devices_.find(vk_device) != devices_.end()) {
    return devices_[vk_device]->GetObjectName(handle);
  }
  return Uint64ToStr(handle);
}

std::string GfrContext::GetObjectInfo(VkDevice vk_device, uint64_t handle) {
  std::lock_guard<std::mutex> lock(devices_mutex_);
  if (devices_.find(vk_device) != devices_.end()) {
    return devices_[vk_device]->GetObjectInfo(handle);
  }
  return Uint64ToStr(handle);
}

void GfrContext::DumpAllDevicesExecutionState() {
  std::lock_guard<std::mutex> lock(devices_mutex_);
  bool dump_prologue = true;
  for (auto& it : devices_) {
    auto device = it.second.get();
    DumpDeviceExecutionState(device, dump_prologue);
    dump_prologue = false;
  }
}

void GfrContext::DumpDeviceExecutionState(VkDevice vk_device,
                                          bool dump_prologue = true) {
  std::lock_guard<std::mutex> lock(devices_mutex_);
  if (devices_.find(vk_device) != devices_.end()) {
    DumpDeviceExecutionState(devices_[vk_device].get(), {}, dump_prologue);
  }
}

void GfrContext::DumpDeviceExecutionState(const Device* device,
                                          bool dump_prologue = true) {
  DumpDeviceExecutionState(device, {}, dump_prologue);
}

void GfrContext::DumpDeviceExecutionState(const Device* device,
                                          std::string error_report,
                                          bool dump_prologue = true) {
  if (!device) {
    return;
  }

  std::stringstream os;
  if (dump_prologue) {
    DumpReportPrologue(os, device);
  }

  device->Print(os);

  if (track_semaphores_) {
    device->GetSubmitTracker()->DumpWaitingSubmits(os);
    os << "\n";
    device->GetSemaphoreTracker()->DumpWaitingThreads(os);
    os << "\n";
  }

  os << "\n";
  os << error_report;

  auto options = CommandBufferDumpOption::kDefault;
  if (debug_dump_all_command_buffers_)
    options |= CommandBufferDumpOption::kDumpAllCommands;

  if (debug_autodump_rate_ > 0 || debug_dump_all_command_buffers_) {
    device->DumpAllCommandBuffers(os, options);
  } else {
    device->DumpIncompleteCommandBuffers(os, options);
  }

  WriteReport(os);
}

void GfrContext::DumpDeviceExecutionStateValidationFailed(const Device* device,
                                                          std::ostream& os) {
  // We force all command buffers to dump here because validation can be
  // from a race condition and the GPU can complete work by the time we've
  // started writing the log. (Seen in practice, not theoretical!)
  auto dump_all = debug_dump_all_command_buffers_;
  debug_dump_all_command_buffers_ = true;
  std::stringstream error_report;
  error_report << os.rdbuf();
  DumpDeviceExecutionState(device, error_report.str(),
                           true /* dump_prologue */);
  debug_dump_all_command_buffers_ = dump_all;
}

void GfrContext::DumpReportPrologue(std::ostream& os, const Device* device) {
  os << "#----------------------------------------------------------------\n";
  os << "#-                    GRAPHICS FLIGHT RECORDER                  -\n";
  os << "#----------------------------------------------------------------\n";

  auto now = std::chrono::system_clock::now();
  auto in_time_t = std::chrono::system_clock::to_time_t(now);

  const char* t = "\n  ";
  const char* tt = "\n    ";
  os << "GFRInfo:" << t << "version: " << kGfrVersion << t << "date: \""
     << std::put_time(std::localtime(&in_time_t), "%Y-%m-%d %X") << "\"";
  if (log_configs_) {
    os << t << "envVars:";
    std::string configstr;
    for (auto& cstr : configs_) {
      os << tt << "- " << cstr;
    }
  }
  os << "\n";

  os << "\nInstance:" << device->GetObjectInfo((uint64_t)vk_instance_, t);
  if (application_info_) {
    os << t << "application: \"" << application_info_->applicationName << "\"";
    os << t << "applicationVersion: " << application_info_->applicationVersion;
    os << t << "engine: \"" << application_info_->engineName << "\"";
    os << t << "engineVersion: " << application_info_->engineVersion;

    auto majorVersion = VK_VERSION_MAJOR(application_info_->apiVersion);
    auto minorVersion = VK_VERSION_MINOR(application_info_->apiVersion);
    auto patchVersion = VK_VERSION_PATCH(application_info_->apiVersion);

    os << t << "apiVersion: \"" << std::dec << majorVersion << "."
       << minorVersion << "." << patchVersion << " (0x" << std::hex
       << std::setfill('0') << std::setw(8) << application_info_->apiVersion
       << std::dec << ")\"";
  }

  os << t << "instanceExtensions:";
  for (auto& ext : instance_extension_names_copy_) {
    os << tt << "- \"" << ext << "\"";
  }
  os << "\n";
}

void GfrContext::WriteReport(std::ostream& os) {
  // Make sure our output directory exists
  MakeOutputPath();

  // now write our log.
  std::stringstream ss_path;

  // Keep the first log as gfr.log then add a number if more than one log is
  // generated. Multiple logs are a new feature and we want to keep backward
  // compatiblity for now.
  std::string output_name = "gfr";
  if (output_name_.size() > 0) {
    output_name = output_name_;
  }
  if (total_logs_ > 0) {
    ss_path << output_path_ << output_name << "_" << total_submits_ << "_"
            << total_logs_ << ".log";
  } else {
    ss_path << output_path_ << output_name << ".log";
  }
  total_logs_++;

  std::string output_path = ss_path.str();
  std::ofstream fs(output_path.c_str());
  if (fs.is_open()) {
    std::stringstream ss;
    ss << os.rdbuf();
    fs << ss.str();
    fs.flush();
    fs.close();
  }

  std::stringstream ss;
  ss << "----------------------------------------------------------------\n";
  ss << "- GRAPHICS FLIGHT RECORDER - ERROR DETECTED                    -\n";
  ss << "----------------------------------------------------------------\n";
  ss << "\n";
  ss << "Output written to: " << output_path << "\n";
  ss << "\n";
  ss << "----------------------------------------------------------------\n";
#if defined(WIN32)
  OutputDebugString(ss.str().c_str());
#else
  std::cout << ss.str() << std::endl;
#endif
}

VkCommandPool GfrContext::GetHelperCommandPool(VkDevice vk_device,
                                               VkQueue vk_queue) {
  assert(track_semaphores_ == true);
  if (vk_device == VK_NULL_HANDLE || vk_queue == VK_NULL_HANDLE) {
    return VK_NULL_HANDLE;
  }
  std::lock_guard<std::mutex> lock(devices_mutex_);
  uint32_t queue_family_index =
      devices_[vk_device]->GetQueueFamilyIndex(vk_queue);
  return devices_[vk_device]->GetHelperCommandPool(queue_family_index);
}

SubmitInfoId GfrContext::RegisterSubmitInfo(
    VkDevice vk_device, QueueSubmitId queue_submit_id,
    const VkSubmitInfo* vk_submit_info) {
  assert(track_semaphores_ == true);
  std::lock_guard<std::mutex> lock(devices_mutex_);
  auto submit_info_id =
      devices_[vk_device]->GetSubmitTracker()->RegisterSubmitInfo(
          queue_submit_id, vk_submit_info);
  return submit_info_id;
}

void GfrContext::StoreSubmitHelperCommandBuffersInfo(
    VkDevice vk_device, SubmitInfoId submit_info_id, VkCommandPool vk_pool,
    VkCommandBuffer start_marker_cb, VkCommandBuffer end_marker_cb) {
  assert(track_semaphores_ == true);
  std::lock_guard<std::mutex> lock(devices_mutex_);
  devices_[vk_device]->GetSubmitTracker()->StoreSubmitHelperCommandBuffersInfo(
      submit_info_id, vk_pool, start_marker_cb, end_marker_cb);
}

void GfrContext::RecordSubmitStart(VkDevice vk_device, QueueSubmitId qsubmit_id,
                                   SubmitInfoId submit_info_id,
                                   VkCommandBuffer vk_command_buffer) {
  assert(track_semaphores_ == true);
  std::lock_guard<std::mutex> lock(devices_mutex_);
  devices_[vk_device]->GetSubmitTracker()->RecordSubmitStart(
      qsubmit_id, submit_info_id, vk_command_buffer);
}

void GfrContext::RecordSubmitFinish(VkDevice vk_device,
                                    QueueSubmitId qsubmit_id,
                                    SubmitInfoId submit_info_id,
                                    VkCommandBuffer vk_command_buffer) {
  assert(track_semaphores_ == true);
  std::lock_guard<std::mutex> lock(devices_mutex_);
  auto submit_tracker = devices_[vk_device]->GetSubmitTracker();
  submit_tracker->RecordSubmitFinish(qsubmit_id, submit_info_id,
                                     vk_command_buffer);
  submit_tracker->CleanupSubmitInfos();
}

void GfrContext::LogSubmitInfoSemaphores(VkDevice vk_device, VkQueue vk_queue,
                                         SubmitInfoId submit_info_id) {
  assert(track_semaphores_ == true);
  assert(trace_all_semaphores_ == true);
  std::lock_guard<std::mutex> lock(devices_mutex_);
  auto submit_tracker = devices_[vk_device]->GetSubmitTracker();
  if (submit_tracker->SubmitInfoHasSemaphores(submit_info_id)) {
    std::cout << submit_tracker->GetSubmitInfoSemaphoresLog(vk_device, vk_queue,
                                                            submit_info_id);
  }
}

void GfrContext::RecordBindSparseHelperSubmit(
    VkDevice vk_device, QueueBindSparseId qbind_sparse_id,
    const VkSubmitInfo* vk_submit_info, VkCommandPool vk_pool) {
  assert(track_semaphores_ == true);
  std::lock_guard<std::mutex> lock(devices_mutex_);
  auto submit_tracker = devices_[vk_device]->GetSubmitTracker();
  submit_tracker->CleanupBindSparseHelperSubmits();
  submit_tracker->RecordBindSparseHelperSubmit(qbind_sparse_id, vk_submit_info,
                                               vk_pool);
}

VkDevice GfrContext::GetQueueDevice(VkQueue queue) const {
  std::lock_guard<std::mutex> lock(queue_device_tracker_mutex_);
  auto it = queue_device_tracker_.find(queue);
  if (it == queue_device_tracker_.end()) {
    std::cerr << "GFR Warning: queue " << std::hex << (uint64_t)(queue)
              << std::dec << "cannot be linked to any device." << std::endl;
    return VK_NULL_HANDLE;
  }
  return it->second;
}

bool GfrContext::ShouldExpandQueueBindSparseToTrackSemaphores(
    PackedBindSparseInfo* packed_bind_sparse_info) {
  assert(track_semaphores_ == true);
  VkDevice vk_device = GetQueueDevice(packed_bind_sparse_info->queue);
  assert(vk_device != VK_NULL_HANDLE);
  std::lock_guard<std::mutex> lock(devices_mutex_);
  packed_bind_sparse_info->semaphore_tracker =
      devices_[vk_device]->GetSemaphoreTracker();
  return BindSparseUtils::ShouldExpandQueueBindSparseToTrackSemaphores(
      packed_bind_sparse_info);
}

void GfrContext::ExpandBindSparseInfo(
    ExpandedBindSparseInfo* bind_sparse_expand_info) {
  return BindSparseUtils::ExpandBindSparseInfo(bind_sparse_expand_info);
}

void GfrContext::LogBindSparseInfosSemaphores(
    VkQueue vk_queue, uint32_t bind_info_count,
    const VkBindSparseInfo* bind_infos) {
  assert(track_semaphores_ == true);
  assert(trace_all_semaphores_ == true);
  VkDevice vk_device = GetQueueDevice(vk_queue);
  if (vk_device == VK_NULL_HANDLE) {
    return;
  }
  std::lock_guard<std::mutex> lock(devices_mutex_);
  auto log = BindSparseUtils::LogBindSparseInfosSemaphores(
      devices_[vk_device].get(), vk_device, vk_queue, bind_info_count,
      bind_infos);
  std::cout << log;
}

VkResult GfrContext::PreCreateInstance(const VkInstanceCreateInfo* pCreateInfo,
                                       const VkAllocationCallbacks* pAllocator,
                                       VkInstance* pInstance) {
  auto logger = std::make_unique<Logger>();
  logger->LogToolVersion(kGfrVersion);

  // Setup debug flags
  GetEnvVal<bool>(k_env_var_debug_dump_on_begin, &debug_dump_on_begin_);
  GetEnvVal<int>(k_env_var_debug_autodump, &debug_autodump_rate_);
  GetEnvVal<bool>(k_env_var_dump_all_command_buffers,
                  &debug_dump_all_command_buffers_);
  GetEnvVal<bool>(k_env_var_track_semaphores, &track_semaphores_);
  GetEnvVal<bool>(k_env_var_trace_all_semaphores, &trace_all_semaphores_);
  GetEnvVal<bool>(k_env_var_instrument_all_commands, &instrument_all_commands_);
  GetEnvVal<bool>(k_env_var_validate_command_buffer_state,
                  &validate_command_buffer_state_);

  instance_extension_names_copy_.assign(pCreateInfo->ppEnabledExtensionNames,
                                        pCreateInfo->ppEnabledExtensionNames +
                                            pCreateInfo->enabledExtensionCount);
  return VK_SUCCESS;
}

VkResult GfrContext::PostCreateInstance(const VkInstanceCreateInfo* pCreateInfo,
                                        const VkAllocationCallbacks* pAllocator,
                                        VkInstance* pInstance,
                                        VkResult result) {
  vk_instance_ = *pInstance;

  if (pCreateInfo->pApplicationInfo) {
    application_info_ = std::make_unique<ApplicationInfo>();

    application_info_->applicationName =
        pCreateInfo->pApplicationInfo->pApplicationName
            ? pCreateInfo->pApplicationInfo->pApplicationName
            : "";
    application_info_->applicationVersion =
        pCreateInfo->pApplicationInfo->applicationVersion;

    application_info_->engineName =
        pCreateInfo->pApplicationInfo->pEngineName
            ? pCreateInfo->pApplicationInfo->pEngineName
            : "";
    application_info_->engineVersion =
        pCreateInfo->pApplicationInfo->engineVersion;
    application_info_->apiVersion = pCreateInfo->pApplicationInfo->apiVersion;
  }

  return result;
}

void GfrContext::PreDestroyInstance(VkInstance instance,
                                    AllocationCallbacks pAllocator) {}
void GfrContext::PostDestroyInstance(VkInstance instance,
                                     AllocationCallbacks pAllocator) {}

// TODO(b/141996712): extensions should be down at the intercept level, not
// pre/post OR intercept should always extend/copy list
VkResult GfrContext::PreCreateDevice(VkPhysicalDevice physicalDevice,
                                     const VkDeviceCreateInfo* pCreateInfo,
                                     const VkAllocationCallbacks* pAllocator,
                                     VkDevice* pDevice) {
  StringArray extension_name_copy;
  extension_name_copy.assign(pCreateInfo->ppEnabledExtensionNames,
                             pCreateInfo->ppEnabledExtensionNames +
                                 pCreateInfo->enabledExtensionCount);

  device_extension_names_copy_.push_back(std::move(extension_name_copy));
  return VK_SUCCESS;
}

VkResult GfrContext::PostCreateDevice(VkPhysicalDevice physicalDevice,
                                      const VkDeviceCreateInfo* pCreateInfo,
                                      const VkAllocationCallbacks* pAllocator,
                                      VkDevice* pDevice, VkResult callResult) {
  if (callResult != VK_SUCCESS) return callResult;

  bool has_buffer_marker = false;
  for (uint32_t i = 0; i < pCreateInfo->enabledExtensionCount; ++i) {
    const char* name = pCreateInfo->ppEnabledExtensionNames[i];
    has_buffer_marker =
        (strcmp(name, VK_AMD_BUFFER_MARKER_EXTENSION_NAME) == 0);
    if (has_buffer_marker) {
      break;
    }
  }

  VkDevice vk_device = *pDevice;
  DevicePtr device = std::make_unique<Device>(this, physicalDevice, *pDevice,
                                              has_buffer_marker);

  {
    std::lock_guard<std::mutex> lock(device_create_infos_mutex_);
    device->SetDeviceCreateInfo(std::move(device_create_infos_[pCreateInfo]));
    device_create_infos_.erase(pCreateInfo);
  }
  {
    std::lock_guard<std::mutex> lock(devices_mutex_);
    devices_[vk_device] = std::move(device);
  }

  if (track_semaphores_) {
    // Create a helper command pool per queue family index. This command pool
    // will be used for allocating command buffers that track the state of
    // submit and semaphores.
    auto dispatch_table = intercept::GetDeviceDispatchTable(vk_device);
    VkCommandPoolCreateInfo command_pool_create_info = {};
    command_pool_create_info.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;

    for (uint32_t i = 0; i < pCreateInfo->queueCreateInfoCount; i++) {
      auto queue_family_index =
          pCreateInfo->pQueueCreateInfos[i].queueFamilyIndex;
      command_pool_create_info.queueFamilyIndex = queue_family_index;
      VkCommandPool command_pool;
      auto res = dispatch_table->CreateCommandPool(
          vk_device, &command_pool_create_info, nullptr, &command_pool);
      if (res != VK_SUCCESS) {
        std::cerr
            << "GFR Warning: failed to create command pools for helper command "
               "buffers. VkDevice: 0x"
            << std::hex << (uint64_t)(vk_device) << std::dec
            << ", queueFamilyIndex: " << queue_family_index;
      }
      std::lock_guard<std::mutex> lock(devices_mutex_);
      devices_[vk_device]->RegisterHelperCommandPool(queue_family_index,
                                                     command_pool);
    }
  }

  return VK_SUCCESS;
}

void GfrContext::PreDestroyDevice(VkDevice device,
                                  const VkAllocationCallbacks* pAllocator) {
  if (track_semaphores_) {
    auto dispatch_table = intercept::GetDeviceDispatchTable(device);
    std::lock_guard<std::mutex> lock(devices_mutex_);
    auto command_pools = devices_[device]->ReturnAndEraseCommandPools();
    for (auto& command_pool : command_pools) {
      dispatch_table->DestroyCommandPool(device, command_pool, nullptr);
    }
  }
}

void GfrContext::PostDestroyDevice(VkDevice device,
                                   const VkAllocationCallbacks* pAllocator) {
  std::lock_guard<std::mutex> lock(devices_mutex_);

  auto it = devices_.find(device);
  if (it != devices_.end()) {
    devices_.erase(it);
  }
}

void GfrContext::PostGetDeviceQueue(VkDevice device, uint32_t queueFamilyIndex,
                                    uint32_t queueIndex, VkQueue* pQueue) {
  {
    std::lock_guard<std::mutex> lock(devices_mutex_);
    devices_[device]->RegisterQueueFamilyIndex(*pQueue, queueFamilyIndex);
  }
  std::lock_guard<std::mutex> lock(queue_device_tracker_mutex_);
  queue_device_tracker_[*pQueue] = device;
}

VkResult GfrContext::PreQueueSubmit(VkQueue queue, uint32_t submitCount,
                                    const VkSubmitInfo* pSubmits,
                                    VkFence fence) {
  last_submit_time_ =
      std::chrono::duration_cast<std::chrono::milliseconds>(
          std::chrono::high_resolution_clock::now().time_since_epoch())
          .count();

  for (uint32_t submit_index = 0; submit_index < submitCount; ++submit_index) {
    const auto& submit_info = pSubmits[submit_index];
    for (uint32_t command_buffer_index = 0;
         command_buffer_index < submit_info.commandBufferCount;
         ++command_buffer_index) {
      const auto wrapped_command_buffer =
          reinterpret_cast<WrappedVkCommandBuffer*>(
              submit_info.pCommandBuffers[command_buffer_index]);

      CommandBuffer* p_cmd = wrapped_command_buffer->custom_data;
      if (p_cmd != nullptr) {
        p_cmd->QueueSubmit(queue, fence);
      }
    }
  }

  return VK_SUCCESS;
}

// Return true if this is a VkResult that GFR considers an error.
bool IsVkError(VkResult result) {
  return result == VK_ERROR_DEVICE_LOST ||
         result == VK_ERROR_INITIALIZATION_FAILED;
}

VkResult GfrContext::PostQueueSubmit(VkQueue queue, uint32_t submitCount,
                                     const VkSubmitInfo* pSubmits,
                                     VkFence fence, VkResult result) {
  total_submits_++;

  bool dump =
      IsVkError(result) || (debug_autodump_rate_ > 0 &&
                            (total_submits_ % debug_autodump_rate_) == 0);

  if (dump) {
    DumpDeviceExecutionState(GetQueueDevice(queue));
  }
  return result;
}

VkResult GfrContext::PostDeviceWaitIdle(VkDevice device, VkResult result) {
  PostApiFunction("vkDeviceWaitIdle");

  if (IsVkError(result)) {
    DumpDeviceExecutionState(device);
  }

  return result;
}

VkResult GfrContext::PostQueueWaitIdle(VkQueue queue, VkResult result) {
  PostApiFunction("vkQueueWaitIdle");

  if (IsVkError(result)) {
    DumpDeviceExecutionState(GetQueueDevice(queue));
  }

  return result;
}

VkResult GfrContext::PostQueuePresentKHR(VkQueue queue,
                                         VkPresentInfoKHR const* pPresentInfo,
                                         VkResult result) {
  PostApiFunction("vkQueuePresentKHR");

  if (IsVkError(result)) {
    DumpDeviceExecutionState(GetQueueDevice(queue));
  }

  return result;
}

VkResult GfrContext::PostQueueBindSparse(VkQueue queue, uint32_t bindInfoCount,
                                         VkBindSparseInfo const* pBindInfo,
                                         VkFence fence, VkResult result) {
  PostApiFunction("vkQueueBindSparse");

  if (IsVkError(result)) {
    DumpDeviceExecutionState(GetQueueDevice(queue));
  }

  return result;
}

VkResult GfrContext::PostWaitForFences(VkDevice device, uint32_t fenceCount,
                                       VkFence const* pFences, VkBool32 waitAll,
                                       uint64_t timeout, VkResult result) {
  PostApiFunction("vkWaitForFences");

  if (IsVkError(result)) {
    DumpDeviceExecutionState(device);
  }

  return result;
}

VkResult GfrContext::PostGetFenceStatus(VkDevice device, VkFence fence,
                                        VkResult result) {
  PostApiFunction("vkGetFenceStatus");

  if (IsVkError(result)) {
    DumpDeviceExecutionState(device);
  }

  return result;
}

VkResult GfrContext::PostGetQueryPoolResults(
    VkDevice device, VkQueryPool queryPool, uint32_t firstQuery,
    uint32_t queryCount, size_val dataSize, void* pData, VkDeviceSize stride,
    VkQueryResultFlags flags, VkResult result) {
  PostApiFunction("vkGetQueryPoolResults");

  if (IsVkError(result)) {
    DumpDeviceExecutionState(device);
  }

  return result;
}

VkResult GfrContext::PostAcquireNextImageKHR(
    VkDevice device, VkSwapchainKHR swapchain, uint64_t timeout,
    VkSemaphore semaphore, VkFence fence, uint32_t* pImageIndex,
    VkResult result) {
  PostApiFunction("vkAcquireNextImageKHR");

  if (IsVkError(result)) {
    DumpDeviceExecutionState(device);
  }

  return result;
}

VkResult GfrContext::PreCreateShaderModule(
    VkDevice device, const VkShaderModuleCreateInfo* pCreateInfo,
    const VkAllocationCallbacks* pAllocator, VkShaderModule* pShaderModule) {
  return VK_SUCCESS;
}

VkResult GfrContext::PostCreateShaderModule(
    VkDevice device, const VkShaderModuleCreateInfo* pCreateInfo,
    const VkAllocationCallbacks* pAllocator, VkShaderModule* pShaderModule,
    VkResult callResult) {
  if (callResult == VK_SUCCESS) {
    std::lock_guard<std::mutex> lock(devices_mutex_);
    Device* p_device = devices_[device].get();
    p_device->CreateShaderModule(pCreateInfo, pShaderModule,
                                 shader_module_load_options_);
  }
  return callResult;
}

void GfrContext::PreDestroyShaderModule(
    VkDevice device, VkShaderModule shaderModule,
    const VkAllocationCallbacks* pAllocator) {}

void GfrContext::PostDestroyShaderModule(
    VkDevice device, VkShaderModule shaderModule,
    const VkAllocationCallbacks* pAllocator) {
  std::lock_guard<std::mutex> lock(devices_mutex_);
  devices_[device]->DeleteShaderModule(shaderModule);
}

VkResult GfrContext::PreCreateGraphicsPipelines(
    VkDevice device, VkPipelineCache pipelineCache, uint32_t createInfoCount,
    const VkGraphicsPipelineCreateInfo* pCreateInfos,
    const VkAllocationCallbacks* pAllocator, VkPipeline* pPipelines) {
  return VK_SUCCESS;
}

VkResult GfrContext::PostCreateGraphicsPipelines(
    VkDevice device, VkPipelineCache pipelineCache, uint32_t createInfoCount,
    const VkGraphicsPipelineCreateInfo* pCreateInfos,
    const VkAllocationCallbacks* pAllocator, VkPipeline* pPipelines,
    VkResult callResult) {
  if (callResult == VK_SUCCESS) {
    std::lock_guard<std::mutex> lock(devices_mutex_);
    Device* p_device = devices_[device].get();
    p_device->CreatePipeline(createInfoCount, pCreateInfos, pPipelines);
  }
  return callResult;
}

VkResult GfrContext::PreCreateComputePipelines(
    VkDevice device, VkPipelineCache pipelineCache, uint32_t createInfoCount,
    const VkComputePipelineCreateInfo* pCreateInfos,
    const VkAllocationCallbacks* pAllocator, VkPipeline* pPipelines) {
  return VK_SUCCESS;
}

VkResult GfrContext::PostCreateComputePipelines(
    VkDevice device, VkPipelineCache pipelineCache, uint32_t createInfoCount,
    const VkComputePipelineCreateInfo* pCreateInfos,
    const VkAllocationCallbacks* pAllocator, VkPipeline* pPipelines,
    VkResult callResult) {
  if (callResult == VK_SUCCESS) {
    std::lock_guard<std::mutex> lock(devices_mutex_);
    Device* p_device = devices_[device].get();
    p_device->CreatePipeline(createInfoCount, pCreateInfos, pPipelines);
  }
  return callResult;
}

void GfrContext::PreDestroyPipeline(VkDevice device, VkPipeline pipeline,
                                    const VkAllocationCallbacks* pAllocator) {}

void GfrContext::PostDestroyPipeline(VkDevice device, VkPipeline pipeline,
                                     const VkAllocationCallbacks* pAllocator) {
  std::lock_guard<std::mutex> lock(devices_mutex_);
  Device* p_device = devices_[device].get();
  p_device->DeletePipeline(pipeline);
}

void GfrContext::PreDestroyBuffer(VkDevice device, VkBuffer buffer,
                                  AllocationCallbacks pAllocator) {}

void GfrContext::PostDestroyBuffer(VkDevice device, VkBuffer buffer,
                                   AllocationCallbacks pAllocator) {
  // TODO(aellem) Remove object debug name.
}

VkResult GfrContext::PreCreateCommandPool(
    VkDevice device, VkCommandPoolCreateInfo const* pCreateInfo,
    AllocationCallbacks pAllocator, VkCommandPool* pCommandPool) {
  return VK_SUCCESS;
}

VkResult GfrContext::PostCreateCommandPool(
    VkDevice device, const VkCommandPoolCreateInfo* pCreateInfo,
    const VkAllocationCallbacks* pAllocator, VkCommandPool* pCommandPool,
    VkResult callResult) {
  if (callResult == VK_SUCCESS) {
    std::lock_guard<std::mutex> lock_devices(devices_mutex_);
    Device* p_device = devices_[device].get();
    CommandPoolPtr pool = std::make_unique<CommandPool>(
        *pCommandPool, pCreateInfo, p_device->GetVkQueueFamilyProperties(),
        p_device->HasBufferMarker());
    p_device->SetCommandPool(*pCommandPool, std::move(pool));
  }
  return callResult;
}

void GfrContext::PreDestroyCommandPool(
    VkDevice device, VkCommandPool commandPool,
    const VkAllocationCallbacks* pAllocator) {
  PreApiFunction("vkDestroyCommandPool");

  std::lock_guard<std::mutex> lock_devices(devices_mutex_);
  std::stringstream os;
  devices_[device]->ValidateCommandPoolState(commandPool, os);
  if (os.rdbuf()->in_avail()) {
    DumpDeviceExecutionStateValidationFailed(devices_[device].get(), os);
  }
}

void GfrContext::PostDestroyCommandPool(
    VkDevice device, VkCommandPool commandPool,
    const VkAllocationCallbacks* pAllocator) {
  PostApiFunction("vkDestroyCommandPool");

  std::lock_guard<std::mutex> lock_devices(devices_mutex_);
  devices_[device]->DeleteCommandPool(commandPool);
}

VkResult GfrContext::PreResetCommandPool(VkDevice device,
                                         VkCommandPool commandPool,
                                         VkCommandPoolResetFlags flags) {
  PreApiFunction("vkResetCommandPool");

  std::lock_guard<std::mutex> lock_devices(devices_mutex_);
  std::stringstream os;
  devices_[device]->ValidateCommandPoolState(commandPool, os);
  if (os.rdbuf()->in_avail()) {
    DumpDeviceExecutionStateValidationFailed(devices_[device].get(), os);
  }
  return VK_SUCCESS;
}

VkResult GfrContext::PostResetCommandPool(VkDevice device,
                                          VkCommandPool commandPool,
                                          VkCommandPoolResetFlags flags,
                                          VkResult callResult) {
  PostApiFunction("vkResetCommandPool");

  std::lock_guard<std::mutex> lock_devices(devices_mutex_);
  devices_[device]->ResetCommandPool(commandPool);

  return callResult;
}

VkResult GfrContext::PreAllocateCommandBuffers(
    VkDevice device, const VkCommandBufferAllocateInfo* pAllocateInfo,
    VkCommandBuffer* pCommandBuffers) {
  return VK_SUCCESS;
}

VkResult GfrContext::PostAllocateCommandBuffers(
    VkDevice device, const VkCommandBufferAllocateInfo* pAllocateInfo,
    VkCommandBuffer* pCommandBuffers, VkResult callResult) {
  if (callResult == VK_SUCCESS) {
    PostApiFunction("vkAllocateCommandBuffers");

    std::lock_guard<std::mutex> lock_devices(devices_mutex_);

    Device* p_device = devices_[device].get();
    auto vk_pool = pAllocateInfo->commandPool;
    p_device->AllocateCommandBuffers(vk_pool, pAllocateInfo, pCommandBuffers);
    auto has_buffer_markers =
        p_device->GetCommandPool(vk_pool)->HasBufferMarkers();

    // wrap command buffers and create our tracking data
    for (uint32_t i = 0; i < pAllocateInfo->commandBufferCount; ++i) {
      VkCommandBuffer vk_cmd = pCommandBuffers[i];

      auto wcb = std::make_unique<WrappedVkCommandBuffer>();
      pCommandBuffers[i] = reinterpret_cast<VkCommandBuffer>(wcb.get());

      auto cmd = std::make_unique<CommandBuffer>(
          p_device, vk_pool, vk_cmd, pCommandBuffers[i], pAllocateInfo,
          has_buffer_markers);
      cmd->SetInstrumentAllCommands(instrument_all_commands_);

      wcb->wrapped_object = vk_cmd;
      wcb->custom_data = cmd.get();

      // manually initialize the dispatch table of the wrapped CB
      // because the loader won't do it for us because it's hidden
      intercept::SetDeviceLoaderData(device, pCommandBuffers[i]);

      p_device->SetCommandBuffer(vk_cmd, std::move(cmd));
      p_device->SetWrappedCommandBuffer(vk_cmd, std::move(wcb));
    }
  }
  return callResult;
}

void GfrContext::PreFreeCommandBuffers(VkDevice device,
                                       VkCommandPool commandPool,
                                       uint32_t commandBufferCount,
                                       const VkCommandBuffer* pCommandBuffers) {
}

void GfrContext::PostFreeCommandBuffers(
    VkDevice device, VkCommandPool commandPool, uint32_t commandBufferCount,
    const VkCommandBuffer* pCommandBuffers) {
  PostApiFunction("vkFreeCommandBuffers");

  // Unwrap command buffers to validate and remove them from the pool.
  VkCommandBuffer* unwrapped_cbs =
      (VkCommandBuffer*)alloca(commandBufferCount * sizeof(VkCommandBuffer));

  std::lock_guard<std::mutex> lock_devices(devices_mutex_);
  std::stringstream os;
  bool all_cb_ok = true;
  for (uint32_t i = 0; i < commandBufferCount; ++i) {
    WrappedVkCommandBuffer* wcb =
        reinterpret_cast<WrappedVkCommandBuffer*>(pCommandBuffers[i]);
    auto vk_cmd = wcb->wrapped_object;
    unwrapped_cbs[i] = vk_cmd;
    all_cb_ok = all_cb_ok &&
                devices_[device]->ValidateCommandBufferNotInUse(vk_cmd, os);
  }
  if (!all_cb_ok) {
    DumpDeviceExecutionStateValidationFailed(devices_[device].get(), os);
  }

  devices_[device]
      ->GetCommandPool(commandPool)
      ->FreeCommandBuffers(commandBufferCount, unwrapped_cbs);

  // Free the command buffer objects.
  devices_[device]->DeleteCommandBuffers(unwrapped_cbs, commandBufferCount);
}

void GfrContext::PreUpdateDescriptorSets(
    VkDevice device, uint32_t descriptorWriteCount,
    VkWriteDescriptorSet const* pDescriptorWrites, uint32_t descriptorCopyCount,
    VkCopyDescriptorSet const* pDescriptorCopies) {}

void GfrContext::PostUpdateDescriptorSets(
    VkDevice device, uint32_t descriptorWriteCount,
    VkWriteDescriptorSet const* pDescriptorWrites, uint32_t descriptorCopyCount,
    VkCopyDescriptorSet const* pDescriptorCopies) {}

void GfrContext::MakeOutputPath() {
  if (!output_path_created_) {
    output_path_created_ = true;
    MakeDir(output_path_);
  }
}

VkResult GfrContext::PostCreateSemaphore(
    VkDevice device, VkSemaphoreCreateInfo const* pCreateInfo,
    AllocationCallbacks pAllocator, VkSemaphore* pSemaphore, VkResult result) {
  if (track_semaphores_ && result == VK_SUCCESS) {
    uint64_t s_value = 0;
    VkSemaphoreTypeKHR s_type = VK_SEMAPHORE_TYPE_BINARY_KHR;
    const VkSemaphoreTypeCreateInfoKHR* semaphore_info =
        FindOnChain<VkSemaphoreTypeCreateInfoKHR,
                    VK_STRUCTURE_TYPE_SEMAPHORE_TYPE_CREATE_INFO_KHR>(
            pCreateInfo->pNext);
    if (semaphore_info) {
      s_value = semaphore_info->initialValue;
      s_type = semaphore_info->semaphoreType;
    }
    {
      std::lock_guard<std::mutex> lock(devices_mutex_);
      devices_[device]->GetSemaphoreTracker()->RegisterSemaphore(
          *pSemaphore, s_type, s_value);
    }
    if (trace_all_semaphores_) {
      std::stringstream log;
      log << "[GFR] Semaphore created. VkDevice:"
          << GetObjectName(device, (uint64_t)device) << ", VkSemaphore: "
          << GetObjectName(device, (uint64_t)(*pSemaphore));
      if (s_type == VK_SEMAPHORE_TYPE_BINARY_KHR) {
        log << ", Type: Binary.\n";
      } else {
        log << ", Type: Timeline, Initial value: " << s_value << std::endl;
      }
      std::cout << log.str();
    }
  }
  return result;
}

void GfrContext::PostDestroySemaphore(VkDevice device, VkSemaphore semaphore,
                                      AllocationCallbacks pAllocator) {
  if (track_semaphores_) {
    std::lock_guard<std::mutex> lock(devices_mutex_);
    auto semaphore_tracker = devices_[device]->GetSemaphoreTracker();
    if (trace_all_semaphores_) {
      std::stringstream log;
      log << "[GFR] Semaphore destroyed. VkDevice:"
          << GetObjectName(device, (uint64_t)device)
          << ", VkSemaphore: " << GetObjectName(device, (uint64_t)(semaphore));
      if (semaphore_tracker->GetSemaphoreType(semaphore) ==
          VK_SEMAPHORE_TYPE_BINARY_KHR) {
        log << ", Type: Binary, ";
      } else {
        log << ", Type: Timeline, ";
      }
      uint64_t semaphore_value;
      if (semaphore_tracker->GetSemaphoreValue(semaphore, semaphore_value)) {
        log << "Latest value: " << semaphore_value << std::endl;
      } else {
        log << "Latest value: Unknonw.\n";
      }
      std::cout << log.str();
    }
    semaphore_tracker->EraseSemaphore(semaphore);
  }
}

VkResult GfrContext::PostSignalSemaphoreKHR(
    VkDevice device, const VkSemaphoreSignalInfoKHR* pSignalInfo,
    VkResult result) {
  if (track_semaphores_ && result == VK_SUCCESS) {
    {
      std::lock_guard<std::mutex> lock(devices_mutex_);
      devices_[device]->GetSemaphoreTracker()->SignalSemaphore(
          pSignalInfo->semaphore, pSignalInfo->value,
          {SemaphoreModifierType::kModifierHost});
    }
    if (trace_all_semaphores_) {
      std::cout << "[GFR] Timeline semaphore signaled from host. VkDevice: "
                << GetObjectName(device, (uint64_t)device) << ", VkSemaphore: "
                << GetObjectName(device, (uint64_t)(pSignalInfo->semaphore))
                << ", Signal value: " << pSignalInfo->value << std::endl;
    }
  }
  return result;
}

VkResult GfrContext::PreWaitSemaphoresKHR(
    VkDevice device, const VkSemaphoreWaitInfoKHR* pWaitInfo,
    uint64_t timeout) {
  if (track_semaphores_) {
    int tid = 0;
#ifdef SYS_gettid
    tid = syscall(SYS_gettid);
#endif  // SYS_gettid

#ifdef WIN32
    int pid = _getpid();
#else
    int pid = getpid();
#endif

    {
      std::lock_guard<std::mutex> lock(devices_mutex_);
      devices_[device]->GetSemaphoreTracker()->BeginWaitOnSemaphores(pid, tid,
                                                                     pWaitInfo);
    }
    if (trace_all_semaphores_) {
      std::stringstream log;
      log << "[GFR] Waiting for timeline semaphores on host. PID: " << pid
          << ", TID: " << tid
          << ", VkDevice: " << GetObjectName(device, (uint64_t)device)
          << std::endl;
      for (uint32_t i = 0; i < pWaitInfo->semaphoreCount; i++) {
        log << "[GFR]\tVkSemaphore: "
            << GetObjectName(device, (uint64_t)(pWaitInfo->pSemaphores[i]))
            << ", Wait value: " << pWaitInfo->pValues[i] << std::endl;
      }
      std::cout << log.str();
    }
  }
  return VK_SUCCESS;
}

VkResult GfrContext::PostWaitSemaphoresKHR(
    VkDevice device, const VkSemaphoreWaitInfoKHR* pWaitInfo, uint64_t timeout,
    VkResult result) {
  if (IsVkError(result)) {
    DumpDeviceExecutionState(device);
    return result;
  }
  if (track_semaphores_ && (result == VK_SUCCESS || result == VK_TIMEOUT)) {
    int tid = 0;
#ifdef SYS_gettid
    tid = syscall(SYS_gettid);
#endif  // SYS_gettid

#ifdef WIN32
    int pid = _getpid();
#else
    int pid = getpid();
#endif  // WIN32

    {
      // Update semaphore values
      uint64_t semaphore_value;
      auto dispatch_table = intercept::GetDeviceDispatchTable(device);
      std::lock_guard<std::mutex> lock(devices_mutex_);
      auto semaphore_tracker = devices_[device]->GetSemaphoreTracker();
      for (int i = 0; i < pWaitInfo->semaphoreCount; i++) {
        auto res = dispatch_table->GetSemaphoreCounterValueKHR(
            device, pWaitInfo->pSemaphores[i], &semaphore_value);
        if (res == VK_SUCCESS) {
          semaphore_tracker->SignalSemaphore(
              pWaitInfo->pSemaphores[i], semaphore_value,
              {SemaphoreModifierType::kModifierHost});
        }
      }
      semaphore_tracker->EndWaitOnSemaphores(pid, tid, pWaitInfo);
    }

    if (trace_all_semaphores_) {
      std::stringstream log;
      log << "[GFR] Finished waiting for timeline semaphores on host. PID: "
          << pid << ", TID: " << tid
          << ", VkDevice: " << GetObjectName(device, (uint64_t)device)
          << std::endl;
      for (uint32_t i = 0; i < pWaitInfo->semaphoreCount; i++) {
        log << "[GFR]\tVkSemaphore: "
            << GetObjectName(device, (uint64_t)(pWaitInfo->pSemaphores[i]))
            << ", Wait value: " << pWaitInfo->pValues[i] << std::endl;
      }
      std::cout << log.str();
    }
  }
  return result;
}

VkResult GfrContext::PostGetSemaphoreCounterValueKHR(VkDevice device,
                                                     VkSemaphore semaphore,
                                                     uint64_t* pValue,
                                                     VkResult result) {
  if (IsVkError(result)) {
    DumpDeviceExecutionState(device);
  }
  return result;
}

const std::string& GfrContext::GetOutputPath() const { return output_path_; }

VkResult GfrContext::PreDebugMarkerSetObjectNameEXT(
    VkDevice device, const VkDebugMarkerObjectNameInfoEXT* pNameInfo) {
  PreApiFunction("vkDebugMarkerSetObjectNameEXT");

  auto object_id = pNameInfo->object;

  auto name_info = std::make_unique<ObjectInfo>();
  name_info->object = pNameInfo->object;
  name_info->type = pNameInfo->objectType;
  name_info->name = pNameInfo->pObjectName;
  AddObjectInfo(device, object_id, std::move(name_info));

  return VK_SUCCESS;
};

VkResult GfrContext::PostDebugMarkerSetObjectNameEXT(
    VkDevice device, const VkDebugMarkerObjectNameInfoEXT* pNameInfo,
    VkResult result) {
  PostApiFunction("vkDebugMarkerSetObjectNameEXT");
  return result;
};

VkResult GfrContext::PreSetDebugUtilsObjectNameEXT(
    VkDevice device, const VkDebugUtilsObjectNameInfoEXT* pNameInfo) {
  PreApiFunction("vkSetDebugUtilsObjectNameEXT");

  auto object_id = pNameInfo->objectHandle;

  auto name_info = std::make_unique<ObjectInfo>();
  name_info->object = pNameInfo->objectHandle;
  name_info->type =
      (VkDebugReportObjectTypeEXT)pNameInfo->objectType;  // TODO(aellem): use
                                                          // VkObjectType as
                                                          // base enum, it's
                                                          // more future proof
  name_info->name = pNameInfo->pObjectName;
  AddObjectInfo(device, object_id, std::move(name_info));

  return VK_SUCCESS;
}

VkResult GfrContext::PostSetDebugUtilsObjectNameEXT(
    VkDevice device, const VkDebugUtilsObjectNameInfoEXT* pNameInfo,
    VkResult result) {
  PostApiFunction("vkSetDebugUtilsObjectNameEXT");
  return VK_SUCCESS;
}

// =============================================================================
// Include the generated implementation to forward commands to command buffer
// =============================================================================
#include "gfr_commands.cc.inc"

// =============================================================================
// Declare the custom intercepted commands
// =============================================================================
void GfrContext::PreCmdBindPipeline(WrappedVkCommandBuffer* commandBuffer,
                                    VkPipelineBindPoint pipelineBindPoint,
                                    VkPipeline pipeline) {
  WrappedVkCommandBuffer* wrappedCommandBuffer =
      reinterpret_cast<WrappedVkCommandBuffer*>(commandBuffer);
  VkCommandBuffer unwrappedCommandBuffer = wrappedCommandBuffer->wrapped_object;

  if (DumpShadersOnBind()) {
    commandBuffer->custom_data->GetDevice()->DumpShaderFromPipeline(pipeline);
  }

  commandBuffer->custom_data->PreCmdBindPipeline((VkCommandBuffer)commandBuffer,
                                                 unwrappedCommandBuffer,
                                                 pipelineBindPoint, pipeline);
}

void GfrContext::PostCmdBindPipeline(WrappedVkCommandBuffer* commandBuffer,
                                     VkPipelineBindPoint pipelineBindPoint,
                                     VkPipeline pipeline) {
  WrappedVkCommandBuffer* wrappedCommandBuffer =
      reinterpret_cast<WrappedVkCommandBuffer*>(commandBuffer);
  VkCommandBuffer unwrappedCommandBuffer = wrappedCommandBuffer->wrapped_object;
  commandBuffer->custom_data->PostCmdBindPipeline(
      (VkCommandBuffer)commandBuffer, unwrappedCommandBuffer, pipelineBindPoint,
      pipeline);
}

VkResult GfrContext::PreBeginCommandBuffer(
    WrappedVkCommandBuffer* commandBuffer,
    VkCommandBufferBeginInfo const* pBeginInfo) {
  WrappedVkCommandBuffer* wrappedCommandBuffer =
      reinterpret_cast<WrappedVkCommandBuffer*>(commandBuffer);
  VkCommandBuffer unwrappedCommandBuffer = wrappedCommandBuffer->wrapped_object;

  {
    std::lock_guard<std::mutex> lock(devices_mutex_);
    auto device = commandBuffer->custom_data->GetDevice();
    auto vk_cmd = commandBuffer->custom_data->GetVkCommandBuffer();
    std::stringstream os;
    if (!device->ValidateCommandBufferNotInUse(vk_cmd, os)) {
      DumpDeviceExecutionStateValidationFailed(device, os);
    }
  }

  return commandBuffer->custom_data->PreBeginCommandBuffer(
      (VkCommandBuffer)commandBuffer, unwrappedCommandBuffer, pBeginInfo);
}

VkResult GfrContext::PostBeginCommandBuffer(
    WrappedVkCommandBuffer* commandBuffer,
    VkCommandBufferBeginInfo const* pBeginInfo, VkResult result) {
  WrappedVkCommandBuffer* wrappedCommandBuffer =
      reinterpret_cast<WrappedVkCommandBuffer*>(commandBuffer);
  VkCommandBuffer unwrappedCommandBuffer = wrappedCommandBuffer->wrapped_object;
  return commandBuffer->custom_data->PostBeginCommandBuffer(
      (VkCommandBuffer)commandBuffer, unwrappedCommandBuffer, pBeginInfo,
      result);
}

VkResult GfrContext::PreResetCommandBuffer(
    WrappedVkCommandBuffer* commandBuffer, VkCommandBufferResetFlags flags) {
  WrappedVkCommandBuffer* wrappedCommandBuffer =
      reinterpret_cast<WrappedVkCommandBuffer*>(commandBuffer);
  VkCommandBuffer unwrappedCommandBuffer = wrappedCommandBuffer->wrapped_object;

  {
    std::lock_guard<std::mutex> lock(devices_mutex_);
    auto device = commandBuffer->custom_data->GetDevice();
    auto vk_cmd = commandBuffer->custom_data->GetVkCommandBuffer();
    std::stringstream os;
    if (!device->ValidateCommandBufferNotInUse(vk_cmd, os)) {
      DumpDeviceExecutionStateValidationFailed(device, os);
    }
  }

  return commandBuffer->custom_data->PreResetCommandBuffer(
      (VkCommandBuffer)commandBuffer, unwrappedCommandBuffer, flags);
}

VkResult GfrContext::PostResetCommandBuffer(
    WrappedVkCommandBuffer* commandBuffer, VkCommandBufferResetFlags flags,
    VkResult result) {
  WrappedVkCommandBuffer* wrappedCommandBuffer =
      reinterpret_cast<WrappedVkCommandBuffer*>(commandBuffer);
  VkCommandBuffer unwrappedCommandBuffer = wrappedCommandBuffer->wrapped_object;
  return commandBuffer->custom_data->PostResetCommandBuffer(
      (VkCommandBuffer)commandBuffer, unwrappedCommandBuffer, flags, result);
}

}  // namespace gfr

// =============================================================================
// Custom Vulkan entry points
// =============================================================================

// Because we're wrapping VkCommandBuffer objects we need to unwrap them
// when calling down the layer stack.  When the object is a direct parameter to
// a Vulkan API function the unwrapping code is generated automatically.
//
// However not all code can be automatically unwrapped currently so these
// methods handle those cases.

// TODO(b/153649107): instead of using base interceptor, we want to
// allow list, deny list or may be regex the intercepted functions.
gfr::GfrContext* g_interceptor = new gfr::GfrContext();
intercept::BaseInterceptor* GetInterceptor() { return g_interceptor; }

namespace intercept {
VKAPI_ATTR void VKAPI_CALL FreeCommandBuffers(
    VkDevice device, VkCommandPool commandPool, uint32_t commandBufferCount,
    VkCommandBuffer const* pCommandBuffers) {
  auto dispatch_table = intercept::GetDeviceDispatchTable(device);

  g_interceptor->PreFreeCommandBuffers(device, commandPool, commandBufferCount,
                                       pCommandBuffers);

  // unwrap command buffers
  VkCommandBuffer* unwrapped_cbs =
      (VkCommandBuffer*)alloca(commandBufferCount * sizeof(VkCommandBuffer));

  for (uint32_t i = 0; i < commandBufferCount; ++i) {
    WrappedVkCommandBuffer* wcb =
        reinterpret_cast<WrappedVkCommandBuffer*>(pCommandBuffers[i]);
    unwrapped_cbs[i] = wcb->wrapped_object;
  }

  if (dispatch_table->FreeCommandBuffers) {
    dispatch_table->FreeCommandBuffers(device, commandPool, commandBufferCount,
                                       unwrapped_cbs);
  }

  g_interceptor->PostFreeCommandBuffers(device, commandPool, commandBufferCount,
                                        pCommandBuffers);
}

VKAPI_ATTR VkResult VKAPI_CALL QueueSubmit(VkQueue queue, uint32_t submitCount,
                                           VkSubmitInfo const* pSubmits,
                                           VkFence fence) {
  auto dispatch_table = intercept::GetDeviceDispatchTable(queue);

  g_interceptor->PreQueueSubmit(queue, submitCount, pSubmits, fence);

  // We need to modify the submit info to reference our unwrapped command
  // buffers, then submit the modified submit info down the layer chain.
  auto unwrapped_submits = reinterpret_cast<VkSubmitInfo*>(
      alloca(sizeof(VkSubmitInfo) * submitCount));

  bool track_semaphores = g_interceptor->TrackingSemaphores();
  bool trace_all_semaphores = g_interceptor->TracingAllSemaphores();
  auto queue_submit_id = g_interceptor->GetNextQueueSubmitId();
  for (uint32_t submit_index = 0; submit_index < submitCount; ++submit_index) {
    unwrapped_submits[submit_index] = pSubmits[submit_index];

    auto cb_count = pSubmits[submit_index].commandBufferCount;
    VkCommandBuffer* unwrapped_cbs = nullptr;
    if (track_semaphores) {
      // We need two extra command buffers to track the state of the submit and
      // its semaphores.
      unwrapped_cbs =
          (VkCommandBuffer*)alloca((cb_count + 2) * sizeof(VkCommandBuffer));
      unwrapped_submits[submit_index].pCommandBuffers = unwrapped_cbs;
      unwrapped_submits[submit_index].commandBufferCount = cb_count + 2;
    } else {
      unwrapped_cbs =
          (VkCommandBuffer*)alloca((cb_count) * sizeof(VkCommandBuffer));
      unwrapped_submits[submit_index].pCommandBuffers = unwrapped_cbs;
      unwrapped_submits[submit_index].commandBufferCount = cb_count;
    }

    auto reserve_index_for_semaphores_cb = track_semaphores ? 1 : 0;
    for (uint32_t cb_index = 0; cb_index < cb_count; ++cb_index) {
      WrappedVkCommandBuffer* wcb = reinterpret_cast<WrappedVkCommandBuffer*>(
          pSubmits[submit_index].pCommandBuffers[cb_index]);
      unwrapped_cbs[cb_index + reserve_index_for_semaphores_cb] =
          wcb->wrapped_object;
    }

    if (track_semaphores) {
      // Create the two extra CBs used to track the state of submits and
      // semaphores. We create the extra CBs from the same pool used to create
      // the original CBs of the submit. These extra CBs are used to record
      // vkCmdWriteBufferMarkerAMD commands into.
      VkDevice vk_device = g_interceptor->GetQueueDevice(queue);
      VkCommandPool vk_pool =
          g_interceptor->GetHelperCommandPool(vk_device, queue);
      if (vk_pool == VK_NULL_HANDLE) {
        break;
      }
      auto dispatch_table = intercept::GetDeviceDispatchTable(vk_device);

      VkCommandBufferAllocateInfo cb_allocate_info = {};
      cb_allocate_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
      cb_allocate_info.pNext = nullptr;
      cb_allocate_info.commandPool = vk_pool;
      cb_allocate_info.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
      cb_allocate_info.commandBufferCount = 2;

      // TODO b/152057973: Recycle state tracking CBs
      VkCommandBuffer* new_buffers = new VkCommandBuffer[2];
      auto result = dispatch_table->AllocateCommandBuffers(
          vk_device, &cb_allocate_info, new_buffers);
      assert(result == VK_SUCCESS);
      unwrapped_cbs[0] = new_buffers[0];
      unwrapped_cbs[cb_count + 1] = new_buffers[1];
      intercept::SetDeviceLoaderData(vk_device, unwrapped_cbs[0]);
      intercept::SetDeviceLoaderData(vk_device, unwrapped_cbs[cb_count + 1]);

      VkCommandBufferBeginInfo commandBufferBeginInfo = {};
      commandBufferBeginInfo.sType =
          VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
      commandBufferBeginInfo.flags = 0;

      auto submit_info_id = g_interceptor->RegisterSubmitInfo(
          vk_device, queue_submit_id, &unwrapped_submits[submit_index]);
      g_interceptor->StoreSubmitHelperCommandBuffersInfo(
          vk_device, submit_info_id, vk_pool, unwrapped_cbs[0],
          unwrapped_cbs[cb_count + 1]);
      for (uint32_t cb_index = 0; cb_index < cb_count; ++cb_index) {
        WrappedVkCommandBuffer* wcb = reinterpret_cast<WrappedVkCommandBuffer*>(
            pSubmits[submit_index].pCommandBuffers[cb_index]);
        wcb->custom_data->SetSubmitInfoId(submit_info_id);
      }
      result = dispatch_table->BeginCommandBuffer(unwrapped_cbs[0],
                                                  &commandBufferBeginInfo);
      assert(result == VK_SUCCESS);
      g_interceptor->RecordSubmitStart(vk_device, queue_submit_id,
                                       submit_info_id, unwrapped_cbs[0]);
      result = dispatch_table->EndCommandBuffer(unwrapped_cbs[0]);
      assert(result == VK_SUCCESS);

      result = dispatch_table->BeginCommandBuffer(unwrapped_cbs[cb_count + 1],
                                                  &commandBufferBeginInfo);
      assert(result == VK_SUCCESS);
      g_interceptor->RecordSubmitFinish(vk_device, queue_submit_id,
                                        submit_info_id,
                                        unwrapped_cbs[cb_count + 1]);
      result = dispatch_table->EndCommandBuffer(unwrapped_cbs[cb_count + 1]);
      assert(result == VK_SUCCESS);

      if (trace_all_semaphores) {
        g_interceptor->LogSubmitInfoSemaphores(vk_device, queue,
                                               submit_info_id);
      }
    }
  }

  VkResult res = VK_SUCCESS;
  if (dispatch_table->QueueSubmit) {
    res = dispatch_table->QueueSubmit(queue, submitCount, unwrapped_submits,
                                      fence);
  }

  g_interceptor->PostQueueSubmit(queue, submitCount, pSubmits, fence, res);

  return res;
}

VKAPI_ATTR VkResult VKAPI_CALL
QueueBindSparse(VkQueue queue, uint32_t bindInfoCount,
                const VkBindSparseInfo* pBindInfo, VkFence fence) {
  auto dispatch_table = intercept::GetDeviceDispatchTable(queue);
  if (!dispatch_table->QueueBindSparse) {
    return VK_ERROR_FEATURE_NOT_PRESENT;
  }

  auto qbind_sparse_id = g_interceptor->GetNextQueueBindSparseId();
  bool track_semaphores = g_interceptor->TrackingSemaphores();
  bool trace_all_semaphores = g_interceptor->TracingAllSemaphores();

  if (track_semaphores && trace_all_semaphores) {
    g_interceptor->LogBindSparseInfosSemaphores(queue, bindInfoCount,
                                                pBindInfo);
  }

  // If semaphore tracking is not requested, pass the call to the dispatch table
  // as is.
  if (!track_semaphores) {
    return dispatch_table->QueueBindSparse(queue, bindInfoCount, pBindInfo,
                                           fence);
  }

  // Ensure the queue is registered before and we know which command pool use
  // for this queue. If not, pass the call to dispatch table.
  VkDevice vk_device = g_interceptor->GetQueueDevice(queue);
  VkCommandPool vk_pool = g_interceptor->GetHelperCommandPool(vk_device, queue);
  if (vk_device == VK_NULL_HANDLE || vk_pool == VK_NULL_HANDLE) {
    std::cerr << "GFR Warning: device handle not found for queue " << std::hex
              << (uint64_t)queue << std::dec
              << ", Ignoring "
                 "semaphore signals in vkQueueBindSparse call."
              << std::endl;
    return dispatch_table->QueueBindSparse(queue, bindInfoCount, pBindInfo,
                                           fence);
  }

  // If we don't need to expand the bind sparse info, pass the call to dispatch
  // table.
  gfr::PackedBindSparseInfo packed_bind_sparse_info(queue, bindInfoCount,
                                                    pBindInfo);
  if (!g_interceptor->ShouldExpandQueueBindSparseToTrackSemaphores(
          &packed_bind_sparse_info)) {
    return dispatch_table->QueueBindSparse(queue, bindInfoCount, pBindInfo,
                                           fence);
  }

  gfr::ExpandedBindSparseInfo expanded_bind_sparse_info(
      &packed_bind_sparse_info);
  g_interceptor->ExpandBindSparseInfo(&expanded_bind_sparse_info);

  // For each VkSubmitInfo added to the expanded vkQueueBindSparse, check if
  // pNext should point to a VkTimelineSemaphoreSubmitInfoKHR struct.
  size_t tsinfo_it = 0;
  for (int i = 0; i < expanded_bind_sparse_info.submit_infos.size(); i++) {
    if (expanded_bind_sparse_info.has_timeline_semaphore_info[i]) {
      expanded_bind_sparse_info.submit_infos[i].pNext =
          &expanded_bind_sparse_info.timeline_semaphore_infos[tsinfo_it++];
    }
  }

  // For each VkSubmitInfo added to the expanded vkQueueBindSparse, reserve a
  // command buffer and put in the submit.
  // Allocate the required command buffers
  auto num_submits = (uint32_t)expanded_bind_sparse_info.submit_infos.size();
  VkCommandBuffer* helper_cbs =
      (VkCommandBuffer*)alloca((num_submits) * sizeof(VkCommandBuffer));

  VkCommandBufferAllocateInfo cb_allocate_info = {};
  cb_allocate_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
  cb_allocate_info.pNext = nullptr;
  cb_allocate_info.commandPool = vk_pool;
  cb_allocate_info.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
  cb_allocate_info.commandBufferCount = num_submits;
  // TODO b/152057973: Recycle state tracking CBs
  VkCommandBuffer* new_buffers = new VkCommandBuffer[num_submits];
  auto result = dispatch_table->AllocateCommandBuffers(
      vk_device, &cb_allocate_info, new_buffers);
  assert(result == VK_SUCCESS);
  for (uint32_t i = 0; i < num_submits; i++) {
    helper_cbs[i] = new_buffers[i];
  }

  VkCommandBufferBeginInfo commandBufferBeginInfo = {};
  commandBufferBeginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
  commandBufferBeginInfo.flags = 0;

  uint32_t next_wait_helper_submit = 0;
  for (uint32_t i = 0; i < num_submits; i++) {
    expanded_bind_sparse_info.submit_infos[i].pCommandBuffers = &helper_cbs[i];
    expanded_bind_sparse_info.submit_infos[i].commandBufferCount = 1;
    intercept::SetDeviceLoaderData(vk_device, helper_cbs[i]);

    result = dispatch_table->BeginCommandBuffer(helper_cbs[i],
                                                &commandBufferBeginInfo);
    assert(result == VK_SUCCESS);
    g_interceptor->RecordBindSparseHelperSubmit(
        vk_device, qbind_sparse_id, &expanded_bind_sparse_info.submit_infos[i],
        vk_pool);
    result = dispatch_table->EndCommandBuffer(helper_cbs[i]);
    assert(result == VK_SUCCESS);

    if (expanded_bind_sparse_info.submit_infos[i].signalSemaphoreCount > 0) {
      // Rip out semaphore signal operations from signal helper submit. We
      // needed this info to correctly record the signal semaphore markers, but
      // we don't need the helper submits to signal the semaphores that are
      // already signalled in a bind sparse info.
      expanded_bind_sparse_info.submit_infos[i].signalSemaphoreCount = 0;
      expanded_bind_sparse_info.submit_infos[i].pSignalSemaphores = nullptr;
      expanded_bind_sparse_info.submit_infos[i].pNext = nullptr;
    } else {
      // This is a wait helper submit. We need to signal the wait binary
      // semaphores that the helper submit is waiting on.
      expanded_bind_sparse_info.submit_infos[i].signalSemaphoreCount =
          (uint32_t)expanded_bind_sparse_info
              .wait_binary_semaphores[next_wait_helper_submit]
              .size();
      expanded_bind_sparse_info.submit_infos[i].pSignalSemaphores =
          expanded_bind_sparse_info
              .wait_binary_semaphores[next_wait_helper_submit]
              .data();
      next_wait_helper_submit++;
    }
  }

  uint32_t next_bind_sparse_info_index = 0;
  uint32_t available_bind_sparse_info_counter = 0;
  uint32_t next_submit_info_index = 0;
  VkResult last_bind_result = VK_SUCCESS;
  for (int i = 0; i < expanded_bind_sparse_info.queue_operation_types.size();
       i++) {
    if (expanded_bind_sparse_info.queue_operation_types[i] ==
        gfr::kQueueSubmit) {
      // Send all the available bind sparse infos before submit info. Signal the
      // fence only if the last bind sparse info is included.
      if (available_bind_sparse_info_counter) {
        VkFence bind_fence = VK_NULL_HANDLE;
        if (bindInfoCount ==
            next_bind_sparse_info_index + available_bind_sparse_info_counter) {
          bind_fence = fence;
        }
        result = dispatch_table->QueueBindSparse(
            queue, available_bind_sparse_info_counter,
            &pBindInfo[next_bind_sparse_info_index], bind_fence);
        if (result != VK_SUCCESS) {
          last_bind_result = result;
          break;
        }
        next_bind_sparse_info_index += available_bind_sparse_info_counter;
        available_bind_sparse_info_counter = 0;
      }
      // Send the submit info
      result = dispatch_table->QueueSubmit(
          queue, 1,
          &expanded_bind_sparse_info.submit_infos[next_submit_info_index],
          VK_NULL_HANDLE);
      if (result != VK_SUCCESS) {
        std::cerr
            << "GFR Warning: helper vkQueueSubmit failed while tracking "
               "semaphores in a vkQueueBindSparse call. Semaphore values in "
               "the final report might be wrong. Result: "
            << result << std::endl;
        break;
      }
      next_submit_info_index++;
    } else {
      available_bind_sparse_info_counter++;
    }
  }
  if (last_bind_result != VK_SUCCESS) {
    std::cerr << "GFR Warning: QueueBindSparse: Unexpected VkResult = "
              << last_bind_result
              << " after "
                 "submitting "
              << next_bind_sparse_info_index << " bind sparse infos and "
              << next_submit_info_index
              << " helper submit infos to the "
                 "queue. Submitting the remained bind sparse infos at once."
              << std::endl;
    return dispatch_table->QueueBindSparse(
        queue, bindInfoCount - next_bind_sparse_info_index,
        &pBindInfo[next_bind_sparse_info_index], fence);
  }
  // If any remaining bind sparse infos, submit them all.
  if (bindInfoCount >
      next_bind_sparse_info_index + available_bind_sparse_info_counter) {
    return dispatch_table->QueueBindSparse(
        queue, bindInfoCount - next_submit_info_index,
        &pBindInfo[next_bind_sparse_info_index], fence);
  }
  return last_bind_result;
}

VKAPI_ATTR void VKAPI_CALL
CmdExecuteCommands(VkCommandBuffer commandBuffer, uint32_t commandBufferCount,
                   VkCommandBuffer const* pCommandBuffers) {
  auto& dispatch_table = *GetDeviceDispatchTable(commandBuffer);
  WrappedVkCommandBuffer* wrapped_command_buffer =
      reinterpret_cast<WrappedVkCommandBuffer*>(commandBuffer);

  g_interceptor->PreCmdExecuteCommands(wrapped_command_buffer,
                                       commandBufferCount, pCommandBuffers);

  if (dispatch_table.CmdExecuteCommands && commandBufferCount > 0) {
    // Unwrap command buffers before passing them down the layer chain.
    VkCommandBuffer unwrapped_command_buffer =
        wrapped_command_buffer->wrapped_object;
    auto unwrapped_cbs = reinterpret_cast<VkCommandBuffer*>(
        alloca(sizeof(VkCommandBuffer) * commandBufferCount));
    for (uint32_t i = 0; i < commandBufferCount; ++i) {
      WrappedVkCommandBuffer* wcb =
          reinterpret_cast<WrappedVkCommandBuffer*>(pCommandBuffers[i]);
      unwrapped_cbs[i] = wcb->wrapped_object;
    }
    dispatch_table.CmdExecuteCommands(unwrapped_command_buffer,
                                      commandBufferCount, unwrapped_cbs);
  }

  g_interceptor->PostCmdExecuteCommands(wrapped_command_buffer,
                                        commandBufferCount, pCommandBuffers);
}

}  // namespace intercept
