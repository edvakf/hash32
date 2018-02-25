// https://github.com/google/cld3/blob/fa5974a4d3b5e7934fcb166ff26ed6bfce68b18a/src/utils.cc#L123-L183

/* Copyright 2016 Google Inc. All Rights Reserved.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
==============================================================================*/

#include <string>
#include <sstream>
#include <stdio.h>
#include <sys/time.h>

typedef uint32_t uint32;

// Lower-level versions of Get... that read directly from a character buffer
// without any bounds checking.
inline uint32 DecodeFixed32(const char *ptr) {
  return ((static_cast<uint32>(static_cast<unsigned char>(ptr[0]))) |
          (static_cast<uint32>(static_cast<unsigned char>(ptr[1])) << 8) |
          (static_cast<uint32>(static_cast<unsigned char>(ptr[2])) << 16) |
          (static_cast<uint32>(static_cast<unsigned char>(ptr[3])) << 24));
}

// 0xff is in case char is signed.
static inline uint32 ByteAs32(char c) { return static_cast<uint32>(c) & 0xff; }

uint32 Hash32(const char *data, size_t n, uint32 seed) {
  // 'm' and 'r' are mixing constants generated offline.
  // They're not really 'magic', they just happen to work well.
  const uint32 m = 0x5bd1e995;
  const int r = 24;

  // Initialize the hash to a 'random' value
  uint32 h = static_cast<uint32>(seed ^ n);
  //printf("h=%u\n", h);

  // Mix 4 bytes at a time into the hash
  while (n >= 4) {
    uint32 k = DecodeFixed32(data);
    k *= m;
    k ^= k >> r;
    k *= m;
    h *= m;
    h ^= k;
    data += 4;
    n -= 4;
  }

  // Handle the last few bytes of the input array
  if (n == 3) {
    h ^= ByteAs32(data[2]) << 16;
    h ^= ByteAs32(data[1]) << 8;
    h ^= ByteAs32(data[0]);
    h *= m;
  } else if (n == 2) {
    h ^= ByteAs32(data[1]) << 8;
    h ^= ByteAs32(data[0]);
    h *= m;
  } else if (n == 1) {
    h ^= ByteAs32(data[0]);
    h *= m;
  }

  // Do a few final mixes of the hash to ensure the last few
  // bytes are well-incorporated.
  h ^= h >> 13;
  h *= m;
  h ^= h >> 15;
  return h;
}

uint32 Hash32WithDefaultSeed(const std::string &input) {
  return Hash32(input.data(), input.size(), 0xBEEF);
}

double duration(timeval start, timeval end) {
  return (static_cast<double>(end.tv_sec) + static_cast<double>(end.tv_usec) * 1.0e-6) -
    (static_cast<double>(start.tv_sec) + static_cast<double>(start.tv_usec) * 1.0e-6);
}

int main(int argc, char** argv) {
  struct timeval start, end;
  if (0 != gettimeofday(&start, 0)) { return 1; }

  uint32 out;

  for (int i = 0; i < 1000000; i++) {
    std::ostringstream ostr;
    ostr << "123456789012345678901234567890" << i;
    ostr.str();
  }

  if (0 != gettimeofday(&end, 0)) { return 1; }

  printf("format only: %f\n", duration(start, end));

  if (0 != gettimeofday(&start, 0)) { return 1; }

  for (int i = 0; i < 1000000; i++) {
    std::ostringstream ostr;
    ostr << "123456789012345678901234567890" << i;
    out = Hash32WithDefaultSeed(ostr.str());
  }

  if (0 != gettimeofday(&end, 0)) { return 1; }

  printf("computing hash: %f\n", duration(start, end));
  printf("final output: %u\n", out);
  return 0;
}


