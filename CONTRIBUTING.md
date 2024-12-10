# C++ Guidelines

Selection of [Hyrise Guidelines](https://github.com/hyrise/hyrise/blob/master/CONTRIBUTING.md) that should be considered in this project.

* Use automatic memory management (RAII, smart pointers). Avoid `new` and `malloc` if not absolutely necessary.
* Be mindful of ownership. Not everything needs to be a smart pointer. Consider passing around references (&) to the object or references to a shared_ptr instead of copying the shared_ptr.
Use `const` whenever possible. Consider variables, methods, pointers, and their pointees.

* Header files
  * Reduce the size of your hpp files, both in terms of the number of lines and the code complexity. This keeps the compilation duration low.
  * Code in hpp files is compiled for every cpp file that includes the header. As such, move code to cpp files where possible. This often includes templated classes, where it is sometimes possible to implement their code in cpp files.
  * Use forward declarations instead of full header includes wherever possible.
  * Stick to the include-what-you-use (IWYU) convention and include all headers providing classes you access in the code.

* Loops
  * Use range-based `for` loops when possible: `for (const auto& item : items) {...}`.
  * If you have to use old-style loops, keep in mind that the loop condition is evaluated every time: Instead of `for (auto offset = size_t{0}; offset < something.size(); ++offset)`, the size should be retrieved just once. See also this document.

* Data structures
  * When creating a vector where you know the size beforehand, use `reserve` to avoid unnecessary resizes and allocations.
  * Hash-based data structures are usually faster than tree-based data structures. Unless you have a reason to use the latter, prefer `unordered_(map|set)` over `map` and `set`.
* Copies
  * Avoid unnecessary copies, C++ makes it too easy to inadvertently copy objects.
  * For larger elements (e.g., vectors), pass a (`const`) reference instead.
  * If your implemented class does not need to be copied (e.g., a `Table` should never exist more than once), delete the copy constructor and copy assignment operator.

* Miscellaneous
  * Be explicit with types: Use `[u]int(8|16|32|64)_t` instead of `int, long, uint` and prefer `uint32_t{0}` over `0u`.
  * Be aware of auto-to-stick:
    `auto x = int64_t{17};`
     or
	`auto y = std::vector<size_t>{};`
    If you decide to use it, use it consistently.
  * Namespaces: Do not import namespaces in header files.
  * Prefer pre-increment over post-increment. See the LLVM Coding Standards
  * Consider structured bindings:
    `const auto& [iterator, added] = unordered_map.emplace(...);`
* Formatting and Naming
  * clang-tidy config: see CLion, but each project can have its own configuration, see Hyrise, DuckDB.
  * Consider using clang-format to format your code.
  * Choose clear and concise names, and avoid, e.g., `i`, `j`, `ch_ptr`.
  * Prefer declaring (member) variables of the same type in a new line each (isolated declaration):

    ```C++
    class Foo
    {
      int32_t a;
      int32_t b;
    };
    ```
    vs.
    ```C++
    class Foo
    {
       int32_t a, b;
    };
    ```
