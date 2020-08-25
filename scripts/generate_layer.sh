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

if [[ -z "${GAPID_DIR}" ]]; then
  echo "GAPID_DIR is undefined, please set to the root of GAPID (https://github.com/google/gapid)"
  exit 1
else
  GAPID_ROOT="${GAPID_DIR}"
fi

# Check for proper environment setup
APIC_PATH=$(which apic)
if [[ -z "$APIC_PATH" ]]; then
  echo "GAPID 'apic' compiler not in path, did you build GAPID? (https://github.com/google/gapid)"
  exit 1
fi

if [[ ! -f ${GAPID_ROOT}/gapis/api/vulkan/vulkan.api ]]; then
  echo "GAPID vulkan api files not foundnot found: ${GAPID_ROOT}/gapis/api/vulkan/vulkan.api"
  exit 1
fi

# Generate files
cd "$(dirname "$0")"
cd ../generated
echo "Building layers"
apic template ${GAPID_ROOT}/gapis/api/vulkan/vulkan.api ../templates/GFR.cc.tmpl
echo "Building command recording"
apic template ${GAPID_ROOT}/gapis/api/vulkan/vulkan.api ../templates/command_recorder.cc.tmpl
echo "Done"
