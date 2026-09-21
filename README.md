# SPR

C++26 locally, with one C++11-compatible file exported for Online Judge.

Each assignment lives in `include/spr/<problem>.hpp` and exposes:

```cpp
namespace spr::<problem> {
void solve(std::istream& in, std::ostream& out);
}
```

Its GoogleTest file is `tests/<problem>.cpp`. Register the assignment at the
bottom of `CMakeLists.txt`:

```cmake
spr_add_problem(p100)
```

Build and test normally:

```sh
cmake --preset debug
cmake --build --preset debug
ctest --preset debug
```

Create a submission with either the per-problem or aggregate target:

```sh
cmake --build build/debug --target p100_export
cmake --build build/debug --target export
```

The result is `build/debug/submissions/p100.cpp`. The exporter recursively
inlines quoted headers below `include/`, removes `#pragma once`, adds a small
`main()`, and syntax-checks the result as C++11. Standard-library includes stay
unchanged. If the solution uses C++26-only syntax, export fails instead of
silently changing the implementation.
