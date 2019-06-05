assign_ptr
==========
[![CircleCI](https://circleci.com/gh/zachstruck/assign_ptr.svg?style=svg)](https://circleci.com/gh/zachstruck/assign_ptr)
![AppVeyor CI Build Status](https://ci.appveyor.com/api/projects/status/32r7s2skrgm9ubva?svg=true)

`assign_ptr` is a utility for interfacing C++ smart pointers with C-style APIs that allocate resources through output parameters.

## Illustration of the problem ##

```c++
// Allocates memory to output parameter 'p' via 'new'
// Caller responsible for freeing the allocated memory via 'delete'
void foo_alloc(int*& p);
```

How to move the allocated raw pointer into a smart pointer for safety?

### Temporary raw pointer ###

```c++
int* p_ = nullptr;
foo_alloc(p_);
std::unique_ptr<int> p(p_);
```

Doable, but pollutes the scope with a temporary variable.

### Temporary raw pointer with limited scope ###

```c++
std::unique_ptr<int> p;
{
    int* p_ = nullptr;
    foo_alloc(p_);
    p.reset(p_);
}
```

Requires more lines of boilerplate code.

### `assign_ptr` utility function ###

```c++
std::unique_ptr<int> p;
foo_alloc(zpp::assign_ptr(p));
```

Done.

## Caveat

There is one unintuitive edge case to be aware of.

```c++
int* p = nullptr;
if (foo_alloc(p) && p != nullptr)
{
    // Do something with `p`
}
delete p;
```

is not equivalent to

```c++
std::unique_ptr<int> p;
if (foo_alloc(zpp::assign_ptr(p)) && p != nullptr)
{
    // Impossible to enter this branch
    // because `p` has not yet been set
}
```

C++17 offers a solution via if-statement initialization.

```c++
std::unique_ptr<int> p;
if (foo_alloc(zpp::assign); p != nullptr)
{
    // Do something with `p`
}
```

## Supported smart pointers ##

### Defined requirements ###

`assign_ptr` should work with any class/struct that defines the following:

* `element_type` type
* `reset(element_type*)` member function

### Tested smart pointers ###

* std::unique_ptr
* std::shared_ptr
* std::auto_ptr

### Compiler support ###

Tested with Visual Studio 2017 15.3.

C++14 (but this could be lowered to C++03 by conditionally removing some safety).
