#!/bin/bash

# Copyright (C) 2020 Google Inc.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

# Set the layer path and template names here. You shouldn't need to touch
# the following sections if you use the default layergen directory structure.
LAYERGEN=gputools/gfr/layergen
TEMPLATES=( GFR.cc.tmpl command_recorder.cc.tmpl )

# Build apic and required runtime libraries
bazel build @gapid_tools//:apic @yeti_crosstool_debian//:dynamic-runtime-libs-k8

cd "$(dirname "$0")"
YETI_PATH=${PWD%"/$LAYERGEN"}
LAYERGEN=$YETI_PATH/$LAYERGEN
APIC=$YETI_PATH/bazel-yeti/external/gapid_tools/apic
VULKAN_API=$YETI_PATH/bazel-yeti/external/gapid_tools/vulkan_api

# Add sysroot to path so apic can find stdc++.lib
SYSROOT=$YETI_PATH/bazel-yeti/external/yeti_crosstool_debian/sysroot/lib
export LD_LIBRARY_PATH=$SYSROOT

cd ..
rm -rf generated
mkdir generated
cd generated

# Run APIC and generate the layer
for template in "${TEMPLATES[@]}"
do
  echo RUNNING: $APIC template --search .,$VULKAN_API,$LAYERGEN $VULKAN_API/vulkan.api $LAYERGEN/$template
  $APIC template --search .,$VULKAN_API,$LAYERGEN $VULKAN_API/vulkan.api $LAYERGEN/$template
done
