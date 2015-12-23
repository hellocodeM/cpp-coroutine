#include <cassert>

#include "coroutine.hpp"

namespace co = ming::coroutine;

void foo() {
    puts("marry");
    co::yield();
    puts("christmas");
}

int main() {
    auto& c = co::coroutine(foo);
    c();
    c();
    return 0;
}
