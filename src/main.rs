
use std::time;

fn main() {
    let mut start = time::Instant::now();

    for i in 0..1000000 {
        format!("123456789012345678901234567890{}", i);
    }

    let mut elapsed = start.elapsed();
    println!("format only: {}", elapsed.as_secs() as f64 + elapsed.subsec_nanos() as f64 * 1.0e-9);

    let mut out: u32 = 0;
    start = time::Instant::now();

    for i in 0..1000000 {
        out = has32_with_default_seed(&format!("123456789012345678901234567890{}", i));
    }

    elapsed = start.elapsed();
    println!("computing hash: {}", elapsed.as_secs() as f64 + elapsed.subsec_nanos() as f64 * 1.0e-9);
    println!("final output: {}", out);
}

// rewrite of https://github.com/google/cld3/blob/fa5974a4d3b5e7934fcb166ff26ed6bfce68b18a/src/utils.cc#L123-L183

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

pub fn has32_with_default_seed(input: &str) -> u32 {
    return hash32(input.as_bytes(), input.len(), 0xBEEF);
}

#[inline(always)]
fn decode_fixed32(a: u8, b: u8, c: u8, d: u8) -> u32 {
    return a as u32 |
        ((b as u32) << 8) |
        ((c as u32) << 16) |
        ((d as u32) << 24);
}

#[inline(always)]
fn byte_as_32(c: u8) -> u32 {
    return (c as u32) & 0xff;
}

#[allow(dead_code)]
fn hash32(data: &[u8], n: usize, seed: u32) -> u32 {
    let mut nn = n as u32;

    // 'm' and 'r' are mixing constants generated offline.
    // They're not really 'magic', they just happen to work well.
    let m: u32 = 0x5bd1e995;
    let r = 24;

    // Initialize the hash to a 'random' value
    let mut h: u32 = seed ^ nn;

    // Mix 4 bytes at a time into the hash
    let mut i = 0;
    while nn >= 4 {
        let mut k: u32 = decode_fixed32(
            data[i],
            data[i + 1],
            data[i + 2],
            data[i + 3]
        );
        k = k.wrapping_mul(m);
        k ^= k >> r;
        k = k.wrapping_mul(m);
        h = h.wrapping_mul(m);
        h ^= k;
        nn -= 4;
        i += 4;
    }

    // Handle the last few bytes of the input array
    if nn == 3 {
        h ^= byte_as_32(data[i + 2]) << 16;
        h ^= byte_as_32(data[i + 1]) << 8;
        h ^= byte_as_32(data[i]);
        h = h.wrapping_mul(m);
    } else if nn == 2 {
        h ^= byte_as_32(data[i + 1]) << 8;
        h ^= byte_as_32(data[i]);
        h = h.wrapping_mul(m);
    } else if nn == 1 {
        h ^= byte_as_32(data[i]);
        h = h.wrapping_mul(m);
    }

    // Do a few final mixes of the hash to ensure the last few
    // bytes are well-incorporated.
    h ^= h >> 13;
    h = h.wrapping_mul(m);
    h ^= h >> 15;
    return h;
}
