"""Generates a layer that prints the Vulkan commands and their parameters to console.
"""

import collections
import os
from layergen import layergen_utils
import jinja2
from third_party.vkspecgen import vkapi


def main() -> None:
    # Vulkan registry parameters and filters
    platforms = ['', 'ggp']
    authors = ['', 'KHR', 'EXT', 'AMD', 'GGP']  # default: ['', 'KHR', 'EXT']
    allowed_extensions = [
        'VK_EXT_debug_report',
        'VK_EXT_debug_utils',
        'VK_EXT_debug_marker',
    ]
    blocked_extensions = None
    # generated layer parameters
    layer_prefix = 'GFR'
    common_files = [
        'dispatch.h',
        'dispatch.cc',
        'layer_base.h',
        'layer_base.cc',
    ]
    layer_files = [
        'command_common.h',
        'command_common.cc',
        'command_printer.h',
        'command_printer.cc',
        'command_recorder.h',
        'command_recorder.cc',
        'command_tracker.h',
        'command_tracker.cc',
        'command.h.inc',
        'command.cc.inc',
        'gfr_commands.h.inc',
        'gfr_commands.cc.inc',
        'gfr_intercepts.cc.inc',
    ]

    # Create registry
    registry = vkapi.Registry(
        layergen_utils.get_vk_xml_path(),
        platforms=platforms,
        authors=authors,
        allowed_extensions=allowed_extensions,
        blocked_extensions=blocked_extensions)

    # Figure out which types we need to understand based on our command set.
    types = layergen_utils.referenced_types(registry.commands.values())

    # Create Jinja2 templating environment
    layer_templates_dir = os.path.join(os.path.dirname(
        os.path.realpath(__file__)), 'templates')
    env = vkapi.JinjaEnvironment(
        registry,
        loader=jinja2.FileSystemLoader(searchpath=[
            layer_templates_dir,
            layergen_utils.get_common_templates_dir()
        ]),
        keep_trailing_newline=True)

    platform_structs = {
        k: [t for t in v.types.values() if isinstance(t, vkapi.Struct)]
        for (k, v) in registry.platforms.items()
    }

    # Set the layergen parameters here.
    custom_entry_points = ['vkCreateDevice',
                           'vkQueueSubmit', 'vkQueueBindSparse']
    intercept_groups = []
    intercept_pre_groups = ['all_vk_cmds']
    intercept_post_groups = ['all_vk_cmds']
    intercept_pre_post_commands = [
        'vkCreateInstance', 'vkDestroyDevice', 'vkQueueSubmit',
        'vkDestroyCommandPool', 'vkResetCommandPool', 'vkWaitSemaphoresKHR',
        'vkDebugMarkerSetObjectNameEXT', 'vkSetDebugUtilsObjectNameEXT',
        'vkCmdBindPipeline', 'vkBeginCommandBuffer', 'vkResetCommandBuffer',
        'vkEndCommandBuffer',
    ]
    intercept_pre_commands = intercept_pre_post_commands + []
    intercept_post_commands = intercept_pre_post_commands + [
        'vkCreateDevice', 'vkGetDeviceQueue',
        'vkDeviceWaitIdle', 'vkQueueWaitIdle', 'vkQueuePresentKHR',
        'vkQueueBindSparse', 'vkWaitForFences', 'vkGetFenceStatus',
        'vkGetQueryPoolResults', 'vkAcquireNextImageKHR', 'vkCreateShaderModule',
        'vkDestroyShaderModule', 'vkCreateGraphicsPipelines', 'vkDestroyPipeline',
        'vkCreateComputePipelines', 'vkCreateCommandPool', 'vkCreateSemaphore',
        'vkAllocateCommandBuffers', 'vkFreeCommandBuffers', 'vkDestroySemaphore',
        'vkSignalSemaphoreKHR', 'vkGetSemaphoreCounterValueKHR',
    ]

    layer_file_format_version = '1.1.0'
    layer_name = 'VK_LAYER_GOOGLE_graphics_flight_recorder'
    layer_type = 'GLOBAL'
    layer_library_path_linux = 'libVkLayer_gfr.so'
    layer_library_path_windows = '.\\\\VkLayer_gfr.dll'
    layer_implementation_version = '1'
    enforced_api_version = None
    # layer description ends up in the JSON manifest file, and JSON does not
    # support multi-line strings.
    layer_description = 'Google Graphics Flight Recorder is a crash/hang debugging tool that helps determines GPU progress in a Vulkan application.'  # pylint: disable=line-too-long
    env_var_prefix = 'GFR'

    InstanceExtension = collections.namedtuple(
        'InstanceExtension', ['name', 'version'])
    debug_report_ext = InstanceExtension(
        name='VK_EXT_debug_report', version=1)
    debug_utils_ext = InstanceExtension(
        name='VK_EXT_debug_utils', version=1)
    implemented_instance_extensions = [debug_report_ext, debug_utils_ext]

    DeviceExtension = collections.namedtuple(
        'DeviceExtension', ['name', 'version', 'entry_points'])
    debug_marker_ext = DeviceExtension(name='VK_EXT_debug_marker',
                                       version=4,
                                       entry_points=[
                                           'vkDebugMarkerSetObjectTagEXT',
                                           'vkDebugMarkerSetObjectNameEXT',
                                           'vkCmdDebugMarkerBeginEXT',
                                           'vkCmdDebugMarkerEndEXT',
                                           'vkCmdDebugMarkerInsertEXT'
                                       ])
    implemented_device_extensions = [debug_marker_ext]

    layergen_parameters = {
        'enums': [t for t in types.values() if isinstance(t, vkapi.Enum)],
        'platform_structs': platform_structs,
        'layer_prefix': layer_prefix,
        'modify_instance_create_info': 'GetModifiedInstanceCreateInfo',
        'modify_device_create_info': 'GetModifiedDeviceCreateInfo',
        'intercept_groups': intercept_groups,
        'intercept_pre_groups': intercept_pre_groups,
        'intercept_post_groups': intercept_post_groups,
        'intercept_commands': custom_entry_points,
        'intercept_pre_commands': intercept_pre_commands,
        'intercept_post_commands': intercept_post_commands,
        'name': layer_name,
        'implementation_version': layer_implementation_version,
        'description': layer_description,
        'instance_extensions': implemented_instance_extensions,
        'device_extensions': implemented_device_extensions,
        'copyright_notice': """/*
 * Copyright (C) 2021 Google Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * THIS FILE IS GENERATED BY VkSpecGen. DO NOT EDIT.
 */
      """
    }

    output_dir = os.path.join(os.path.dirname(
        os.path.realpath(__file__)), 'generated')
    if not os.path.isdir(output_dir):
        os.mkdir(output_dir)

    # Generate base layer files
    for f in common_files:
        layergen_utils.generate_file(f, output_dir, env, layergen_parameters)

    # Generate layer specific files
    for f in layer_files:
        layergen_utils.generate_file(f, output_dir, env, layergen_parameters)

    # Generate layer manifest
    manifest_parameters = {
        'prefix': layer_prefix,
        'file_format_version': layer_file_format_version,
        'name': layer_name,
        'type': layer_type,
        'library_path': layer_library_path_linux,
        'enforced_api_version': enforced_api_version,
        'implementation_version': layer_implementation_version,
        'description': layer_description,
        'env_var_prefix': env_var_prefix,
        'instance_extensions': implemented_instance_extensions,
        'device_extensions': implemented_device_extensions,
    }
    manifest_name = 'gfr.json'
    manifest_output = os.path.join(output_dir, "../manifest/linux")
    layergen_utils.generate_manifest_file(manifest_name, manifest_output, env,
                                          manifest_parameters)

    manifest_parameters['library_path'] = layer_library_path_windows
    manifest_output = os.path.join(output_dir, "../manifest/windows")
    layergen_utils.generate_manifest_file(manifest_name, manifest_output, env,
                                          manifest_parameters)


if __name__ == '__main__':
    main()
