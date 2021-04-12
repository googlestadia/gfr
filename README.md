# Graphics Flight Recorder

The Graphics Flight Recorder (GFR)  is a Vulkan layer to help trackdown and identify the cause of GPU hangs and crashes.  It works by instrumenting command buffers with completion tags.  When an error is detected a log file containing incomplete command buffers is written.  Often the last complete or incomplete commands are responsible for the crash.

This is not an officially supported Google product.

## Prerequisites

- **Set the VULKAN_SDK environment variable**.
  This variable should point to the base include folder, the setup script in the VulkanSDK will setup proper environment (e.g. `setup-env.sh`).
- Install [CMake](https:/cmake.org).
- Confirm support for the `VK_AMD_buffer_marker` device extension.

## Build the Layer

### Building on Windows

Run cmake:
```
> cmake -Bbuild -DCMAKE_GENERATOR_PLATFORM=x64 -H.
```
Open the solution: `build\GFR.sln`

DLL will be output here: `build\lib\`

JSON files can be found here: `manifest\windows`

### Building on Linux

```
$ cmake -Bbuild -H.
$ cd build
$ make -j 8
```

SO will be output here: `build\lib\`

JSON files can be found here: `manifest\linux`

## Register the Layer

GFR is an implicit layer. The loader's documentation describes [the difference between implicit and explicit layers](https://github.com/KhronosGroup/Vulkan-LoaderAndValidationLayers/blob/master/loader/LoaderAndLayerInterface.md#implicit-vs-explicit-layers),
but the relevant bit here is that implicit layers are meant to be available to all applications on the system, even if the application doesn't explicitly enable the layer.

In order to be discovered by the Vulkan loader at runtime, implicit layers must be registered. The registration process is platform-specific.
In all cases, it is the layer manifest (the .json file) that is registered; the manifest contains a relative path to the layer
library, which can be in a separate directory (but usually isn't).

If you prefer, it is possible to override the loader's usual layer discovery process by setting `VK_LAYER_PATH` to the directory(s) to search for layer manifest files.

### Registering on Windows

On Windows, implicit layers must be added to the registry in order to be found by the Vulkan loader. See the loader's
documentation on [Windows Layer Discovery](https://github.com/KhronosGroup/Vulkan-Loader/blob/master/loader/LoaderAndLayerInterface.md#windows-layer-discovery) for more information.

Using regedit, open one of the following keys:
- `HKEY_LOCAL_MACHINE\SOFTWARE\Khronos\Vulkan\ImplicitLayers`
- `HKEY_CURRENT_USER\SOFTWARE\Khronos\Vulkan\ImplicitLayers`

Add a new DWORD value to this key:
- Name: full path to the `gfr.json` file, `manifest/windows/gfr.json`.
- Value: 0

### Registering on Linux

On Linux, implicit layer manifests can be copied into any of the following directories:
- /usr/local/etc/vulkan/implicit_layer.d
- /usr/local/share/vulkan/implicit_layer.d
- /etc/vulkan/implicit_layer.d
- /usr/share/vulkan/implicit_layer.d
- $HOME/.local/share/vulkan/implicit_layer.d

The Linux manifest is found in `manifest/linux/gfr.json`.

See the loader's documentation on [Linux Layer Discovery](https://github.com/KhronosGroup/Vulkan-Loader/blob/master/loader/LoaderAndLayerInterface.md#linux-layer-discovery) for more information.

## Basic Usage

GFR is disabled by default. To enable the layer's functionality, set the `GFR_ENABLE` environment variable to 1.

Once enabled, if `vkQueueSubmit()` or other Vulkan functions returns a fatal error code (VK_ERROR_DEVICE_LOST or similar), a log of the command buffers that failed to execute are written to disk.  The default log file location on Linux is `/var/log/gfr/gfr.log` and on Windows is `%USERPROFILE%\gfr\gfr.log`.

## Regenerating the layer

GFR uses GAPID to generate much of it's layer binding. To rebuild the source files in the `generated` folder you
need to have GAPID built: (https://github.com/google/gapid)

On Linux, set `GAPID_DIR` to the root of the GAPID repository and make sure `apic` from GAPID is in your path (you
will likely need to build GAPID yourself to have access to this utility).

Then regnerate the source files:
`$ ./scripts/generate_layer.sh`

## Advanced Configuration

Some additional environment variables are supported, mainly intended for debugging the layer itself.
- `GFR_OUTPUT_PATH` can be set to override the directory where log files and shader binaries are written.
- If `GFR_TRACE_ON` is set to 1, all Vulkan API calls intercepted by the layer will be logged.
- If `GFR_DUMP_ALL_COMMAND_BUFFERS` is set to 1, all command buffers will be output when a log is created, even if they are determined to be complete.
- If `GFR_WATCHDOG_TIMEOUT_MS` is set to a non-zero number, a watchdog thread will be created. This will trigger if the application fails to submit new commands within a set time (in milliseconds) and a log will be created as if the a lost device error was encountered.

- If `GFR_SHADERS_DUMP` is set to 1, all shaders will be dumped to disk when created.
- If `GFR_SHADERS_DUMP_ON_BIND` is set to 1, shaders will be dumped to disk when they are bound.  This can reduce the number of shaders dumped to those referenced by the application.
- If `GFR_SHADERS_DUMP_ON_CRASH` is set to 1, bound shaders will be dumped to disk when a crash is detected.  This will use more memory as shader code will be kept residient in case of a crash.

