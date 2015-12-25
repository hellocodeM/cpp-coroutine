#include <cassert>
#include <cstdio>
#include <cstring>
#include <iostream>

#include "coroutine.hpp"

namespace co = ming::coroutine;

void foo() {
    puts("marry");
    co::yield(1);
    puts("christmas");
    co::yield("foo");
    puts("foo");
}

void bar() {
    puts("hello");
    co::yield(1);
    puts("world");
    co::yield("bar");
    puts("bar");
}

int main() {
    co::Coroutine co_foo(foo);
    co::Coroutine co_bar(bar);
    assert(co_foo.Resume<int>() == 1);
    assert(co_bar.Resume<int>() == 1);
    assert(strcmp(co_foo.Resume<const char*>(), "foo") == 0);
    assert(strcmp(co_bar.Resume<const char*>(), "bar") == 0);
    co_foo.Resume();
    co_bar.Resume();
    std::cout << "coroutine size: "<< sizeof(co::Coroutine) << "\n";
    std::cout << "ucontext_t size: "<< sizeof(ucontext_t) << " * 2\n";
    std::cout << "std::function size: "<< sizeof(std::function<void()>) << "\n";
    std::cout << "std::unique_ptr size: "<< sizeof(std::unique_ptr<co::co_stack_t>) << "\n";
    return 0;
}
