This is my first Rust program.

Hash32 benchmark

```
% cargo run --release
    Finished release [optimized] target(s) in 0.0 secs
     Running `target/release/hash32`
format only: 0.007419103
computing hash: 0.007816120
final output: 947784508
```

```
% rustc -V
rustc 1.24.0
```

For comparison with C++

```
% g++ c++/main.cc && ./a.out
format only: 0.000050207
computing hash: 0.000051971
final output: 947784508
```
