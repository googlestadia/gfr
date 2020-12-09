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

#ifndef GFR_LINEAR_ALLOCATOR_HEADER
#define GFR_LINEAR_ALLOCATOR_HEADER

#include <array>
#include <cassert>
#include <cstddef>
#include <iostream>
#include <memory>
#include <new>
#include <vector>

//
// LinearAllocator is a block based linear allocator for fast allocation of
// simple data structures.
//
// Unlike using a std::vector as a linear allocator the block based approach
// means that re-allocation will not occur when resizing the vector.  This is
// needed so that pointers may be serialized internally from this allocator.
//
// Memory is managed in fixed sized blocks, when a block is full a new block
// will be created. The block will be the size of the larger of the requested
// allocation (+ alignment padding) and the default block size.
//
// Destructors will not be called when Reset.
//
template <size_t kDefaultBlockSize = 1024 * 32, size_t kAlignment = 8>
class LinearAllocator {
  static_assert(kAlignment > 0 && 0 == (kAlignment & (kAlignment - 1)),
                "Power of 2 required");

 public:
  LinearAllocator() : active_block_(0) {
    blocks_.push_back(std::make_unique<Block>(kDefaultBlockSize));
  }

  void* Alloc(const size_t size) {
    assert(blocks_.size() > 0);

    // Try and alloc from the active block.
    auto& block = blocks_[active_block_];
    auto alloc = block->Alloc(size);
    if (alloc) {
      return alloc;
    }

    // The current block is full, try another.
    do {
      // No free blocks, allocate a new one.
      if (active_block_ == blocks_.size() - 1) {
        auto new_block_size = std::max(size + kAlignment, kDefaultBlockSize);
        blocks_.push_back(std::make_unique<Block>(new_block_size));
      }

      active_block_++;
      alloc = blocks_[active_block_]->Alloc(size);
    } while (alloc == nullptr);

    assert(alloc);
    return alloc;
  }

  void Reset() {
    for (auto& block : blocks_) {
      block->Reset();
    }

    active_block_ = 0;
  }

  size_t NumBlocksAllocated() const { return blocks_.size(); }
  size_t GetDefaultBlockSize() const { return kDefaultBlockSize; }

 private:
  class Block {
   public:
    Block(size_t blocksize) {
      blocksize_ = blocksize;
      std::set_new_handler(NewHandler);
      data_ = new char[blocksize_];
      Reset();
    }

    ~Block() { delete[] data_; }

    void* Alloc(const size_t size) {
      auto room = blocksize_ - (head_ - data_);

      uintptr_t h = (uintptr_t)head_;

      // Round up to alignment and check if we fit.
      char* alloc = (char*)((h + kAlignment - 1) & ~(kAlignment - 1));
      if (alloc + size > data_ + blocksize_) {
        return nullptr;
      }

      head_ = alloc + size;
      return alloc;
    }

    void Reset() { head_ = data_; }

   private:
    static void NewHandler() {
      std::cout << "GFR: Memory allocation failed!" << std::endl;
      std::cerr << "GFR: Memory allocation failed!" << std::endl;
      std::set_new_handler(nullptr);
    };
    size_t blocksize_;
    char* head_;
    char* data_;
  };

 private:
  int active_block_;
  std::vector<std::unique_ptr<Block>> blocks_;
};

#endif  // GFR_LINEAR_ALLOCATOR_HEADER
