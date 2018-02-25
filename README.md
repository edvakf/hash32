This is my first Rust program.

Hash32 benchmark

```
hash32/src% rustc -C opt-level=3 main.rs && time ./main
format only: 0.067616282
computing hash: 0.093828037
final output: 3848838207
./main  0.16s user 0.00s system 96% cpu 0.166 total
```

```
% rustc -V
rustc 1.24.0
```

For comparison with C++

```
hash32/c++% g++ -O3 main.cc && time ./a.out
format only: 0.597912
computing hash: 0.681272
final output: 3848838207
./a.out  1.27s user 0.00s system 99% cpu 1.283 total
```
