#include <cassert>
#include <cstdio>

#include "coroutine.hpp"

namespace co = ming::coroutine;

void foo() {
    puts("marry");
    co::yield();
    puts("christmas");
}

void bar() {
    puts("hello");
    co::yield();
    puts("world");
}

int main() {
    auto& c1 = co::coroutine(foo);
    auto& c2 = co::coroutine(bar);
    c1();
    c2();
    c1();
    c2();
    return 0;
}
