#include <cstdio>

#include "coroutine.hpp"

namespace ming {
namespace coroutine {

/* class Coroutine */
Coroutine::Coroutine(co_function_t fn) : fn_(fn) {
    stack_top_ = g_common_stack + kStackSize;
    ctx_.uc_stack.ss_sp = g_common_stack;
    ctx_.uc_stack.ss_size = kStackSize;
    ctx_.uc_stack.ss_flags = 0;
}

void Coroutine::Resume() {
    RESUME_UPPER_HALF
    RESUME_LOWER_HALF
}

void Coroutine::Yield() { YIELD_COMMON }

void Coroutine::CoroutineGuard() {
    this->fn_();
    setcontext(&this->main_ctx_);
}

void Coroutine::PrepareStack() {
    if (stack_used()) {
        std::memcpy(common_stack_top(), stack(), stack_used());
    }
}

void Coroutine::RestoreStack() {
    if (common_stack_used() > stack_used()) {
        // reallocate stack
        stack_top_ = common_stack_top();
        stack_.reset(new co_stack_t[stack_used()]);
    }
    // store stack
    memcpy(stack(), common_stack_top(), common_stack_used());
}

/* free functions */
void yield() {
    assert(g_current && "no coroutine is running");
    g_current->Yield();
}

Coroutine* g_current;
co_stack_t g_common_stack[kStackSize];

} /* end of namespace coroutine */
} /* end of namespace ming */
