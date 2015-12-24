# cpp-coroutine
A simple cpp coroutine implementation.

# features
- the coroutine could be manually yield and resume
- when yielded, the coroutine could transmit a variable to the caller
- the coroutine could transmit any type of variable

# demo
```c++
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
    return 0;
}
```
 
