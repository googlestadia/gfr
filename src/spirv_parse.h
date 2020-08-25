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

#ifndef GFR_PARSE_H
#define GFR_PARSE_H

#include <cassert>
#include <cstdint>
#include <map>

#if defined(SPIRV_PARSE_INCLUDE_VULKAN_SPIRV_HPP)
#ifdef WIN32
#include <spirv-headers/spirv.hpp>
#else
#include <SPIRV/spirv.hpp>
#endif
#endif

namespace gfr {

/**
 * @brief SpirvParse
 *
 */
class SpirvParse {
 public:
  enum {
    kSpirvWordSize = 4,
    kMinimumSpirvWordCount = 5,
    kInstructionStartOffset = kMinimumSpirvWordCount * kSpirvWordSize
  };

  enum Result {
    kSuccess = 0,
    kFailed = -1,
    kInvalidSpirvWordCount = -2,
    kInvalidSpirvMagicNumber = -3,
    kUnexpectedEof = -4,
    kInvalidOperandWordCount = -5,
    kInvalidIdReference = -6,
  };

  SpirvParse() {}
  virtual ~SpirvParse() {}

  Result Parse(size_t size_in_bytes, const char* p_bytes) {
    // Word count check
    const uint32_t spirv_word_count =
        static_cast<uint32_t>(size_in_bytes) / SpirvParse::kSpirvWordSize;

    assert(spirv_word_count > 0);
    if (spirv_word_count < SpirvParse::kMinimumSpirvWordCount) {
      parse_result_ = SpirvParse::Result::kInvalidSpirvWordCount;
      return parse_result_;
    }
    // Data check
    assert(p_bytes != nullptr);
    if (p_bytes == nullptr) {
      parse_result_ = SpirvParse::Result::kUnexpectedEof;
      return parse_result_;
    }

    // start
    const uint32_t* p_words = reinterpret_cast<const uint32_t*>(p_bytes);

    // skip the header words
    current_spirv_word_index_ = SpirvParse::kMinimumSpirvWordCount;

    while (current_spirv_word_index_ < spirv_word_count) {
      uint32_t word = p_words[current_spirv_word_index_];

      // Parse instruction
      spv::Op op = static_cast<spv::Op>(word & 0xFFFF);

      // Parse instruction word count
      uint32_t instruction_word_count = (word >> 16) & 0xFFFF;
      uint32_t operand_word_count = instruction_word_count - 1;

      // Get pointer to operand words
      const uint32_t* p_operand_words = nullptr;
      if (operand_word_count > 0) {
        uint32_t operand_word_index = current_spirv_word_index_ + 1;
        assert(operand_word_index < spirv_word_count);
        if (operand_word_index >= spirv_word_count) {
          parse_result_ = SpirvParse::Result::kUnexpectedEof;
          return parse_result_;
        }
        p_operand_words = &p_words[operand_word_index];
      }
      // Parse op and operands
      SpirvParse::Result op_result =
          ParseOperands(op, operand_word_count, p_operand_words);
      if (op_result != SpirvParse::Result::kSuccess) {
        parse_result_ = op_result;
        return parse_result_;
      }
      // Exit parse loop if parse complete is set
      if (early_out_flag_) {
        break;
      }
      // Increment current word index
      current_spirv_word_index_ += instruction_word_count;
    }

    ParseCompleteEvent();

    parse_result_ = SpirvParse::Result::kSuccess;
    return parse_result_;
  }

  SpirvParse::Result GetParseResult() const { return parse_result_; }

 protected:
  void SetEarlyOutFlag(bool value = true) { early_out_flag_ = true; }

  virtual SpirvParse::Result ParseOperands(spv::Op op,
                                           uint32_t operand_word_count,
                                           const uint32_t* p_words) = 0;
  virtual void ParseCompleteEvent() {}

  template <typename T>
  SpirvParse::Result ReadOperandWord(uint32_t word_index, uint32_t word_count,
                                     const uint32_t* p_words,
                                     T* p_out_word) const {
    if (word_index >= word_count) {
      return SpirvParse::Result::kInvalidOperandWordCount;
    }
    uint32_t word = p_words[word_index];
    *p_out_word = static_cast<T>(word);
    return SpirvParse::Result::kSuccess;
  }

  SpirvParse::Result ReadString(uint32_t word_index, uint32_t word_count,
                                const uint32_t* p_words,
                                std::string* p_out_str) const {
    if (word_index >= word_count) {
      return SpirvParse::Result::kInvalidOperandWordCount;
    }
    const char* c_str = reinterpret_cast<const char*>(&p_words[word_index]);
    *p_out_str = c_str;
    return SpirvParse::Result::kSuccess;
  }

 protected:
  SpirvParse::Result parse_result_ = SpirvParse::Result::kFailed;
  bool early_out_flag_ = false;
  uint32_t current_spirv_word_index_ = static_cast<uint32_t>(~0);
  spv::ExecutionModel execution_model_ = static_cast<spv::ExecutionModel>(~0);
};

/**
 * @brief SpirvParse
 *
 */
class BasicSpirvParse : public SpirvParse {
 public:
  BasicSpirvParse() {}

  virtual ~BasicSpirvParse() {}

  spv::ExecutionModel GetExecutionModel() const { return execution_model_; }

  const std::string& GetEntryPointName() const { return entry_point_name_; }

  const std::string& GetSourceFile() const { return source_file_; }

 protected:
  virtual SpirvParse::Result ParseOperands(spv::Op op,
                                           uint32_t operand_word_count,
                                           const uint32_t* p_words) {
    // printf("OP %08x\n", (uint32_t)op);
    switch (op) {
      default:
        break;

      case spv::OpString: {
        if (operand_word_count < 2) {
          return SpirvParse::Result::kInvalidOperandWordCount;
        }
        // Result id
        uint32_t result_id = UINT32_MAX;
        SpirvParse::Result result = ReadOperandWord<uint32_t>(
            0, operand_word_count, p_words, &result_id);
        if (result != SpirvParse::Result::kSuccess) {
          return result;
        }
        // String
        std::string str;
        result = ReadString(1, operand_word_count, p_words, &str);
        if (result != SpirvParse::Result::kSuccess) {
          return result;
        }
        // Map it
        strings_[result_id] = str;
      } break;

      case spv::OpSource: {
        if (operand_word_count < 2) {
          return SpirvParse::Result::kInvalidOperandWordCount;
        }
        // Source Language
        SpirvParse::Result result = ReadOperandWord<spv::SourceLanguage>(
            0, operand_word_count, p_words, &source_language_);
        if (result != SpirvParse::Result::kSuccess) {
          return result;
        }
        // Version
        result = ReadOperandWord<uint32_t>(1, operand_word_count, p_words,
                                           &version_);
        if (result != SpirvParse::Result::kSuccess) {
          return result;
        }
        // File
        if (operand_word_count > 2) {
          result = ReadOperandWord<uint32_t>(2, operand_word_count, p_words,
                                             &file_id_);
          if (result != SpirvParse::Result::kSuccess) {
            return result;
          }
          // Find the file name
          auto it = strings_.find(file_id_);
          if (it == strings_.end()) {
            return SpirvParse::Result::kInvalidIdReference;
          }
          source_file_ = it->second;
          // Early out the parse
          SetEarlyOutFlag(true);
        }
      } break;

      case spv::OpEntryPoint: {
        if (operand_word_count < 3) {
          return SpirvParse::Result::kInvalidOperandWordCount;
        }
        // Execution Model
        SpirvParse::Result result = ReadOperandWord<spv::ExecutionModel>(
            0, operand_word_count, p_words, &execution_model_);
        if (result != SpirvParse::Result::kSuccess) {
          return result;
        }
        // Name...skipping the <id> for entry point
        const char* c_str = reinterpret_cast<const char*>(p_words + 2);
        assert(c_str != nullptr);
        if (c_str != nullptr) {
          entry_point_name_ = c_str;
        }
      } break;
    }
    return SpirvParse::Result::kSuccess;
  }

 private:
  spv::SourceLanguage source_language_ =
      spv::SourceLanguage::SourceLanguageUnknown;
  uint32_t version_ = 0;
  std::map<uint32_t, std::string> strings_;
  uint32_t file_id_ = UINT32_MAX;
  std::string entry_point_name_;
  std::string source_file_;
};

}  // namespace gfr

#endif  // GFR_PARSE_H
