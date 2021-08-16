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

#ifndef GFR_UTIL_H
#define GFR_UTIL_H

#include <algorithm>
#include <iomanip>
#include <sstream>
#include <string>

namespace gfr {

inline void ToUpper(std::string& s) {
  std::transform(std::begin(s), std::end(s), std::begin(s), ::toupper);
}

inline std::string Uint32ToStr(uint32_t value) {
  std::stringstream ss;
  ss << std::setw(8) << std::setfill('0') << std::hex << value;
  std::string s = ss.str();
  ToUpper(s);
  s = "0x" + s;
  return s;
}

inline std::string Uint64ToStr(uint64_t value) {
  std::stringstream ss;
  ss << std::setw(16) << std::setfill('0') << std::hex << value;
  std::string s = ss.str();
  ToUpper(s);
  s = "0x" + s;
  return s;
}

template <typename T>
std::string PtrToStr(const T* ptr) {
  uintptr_t value = (uintptr_t)ptr;
  return Uint64ToStr(value);
}

inline std::string Indent(uint32_t indent) {
  return "\n" + std::string(indent, ' ');
}

inline std::string IncreaseIndent(const std::string& indent,
                                  const uint32_t num_indents = 2) {
  return indent + std::string(num_indents, ' ');
}

template <typename T, uint32_t sType>
const T* FindOnChain(const void* pNext) {
  if (!pNext) return nullptr;

  auto p = reinterpret_cast<const T*>(pNext);
  if (p->sType == sType) {
    return p;
  }

  return FindOnChain<T, sType>(p->pNext);
}

}  // namespace gfr

#endif  // GFR_UTIL_H
