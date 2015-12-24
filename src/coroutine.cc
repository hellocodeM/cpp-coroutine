#include "coroutine.hpp"

namespace ming {
namespace coroutine {

Coroutine::Coroutine(co_function_t fn) : fn_(fn) {
    ctx_.uc_stack.ss_sp = &stack_;
    ctx_.uc_stack.ss_size = kStackSize;
    ctx_.uc_stack.ss_flags = 0;
}

void Coroutine::Resume() {
    RESUME_UPPER_HALF
    RESUME_LOWER_HALF
}

void Coroutine::Yield() {
    state_ = kYield;
    swapcontext(&ctx_, &main_ctx_);
}

void Coroutine::CoroutineGuard() {
    this->fn_();
    setcontext(&this->main_ctx_);
}

void yield() {
    assert(g_current && "no coroutine is running");
    g_current->Yield();
}

Coroutine* g_current;

} /* end of namespace coroutine */
} /* end of namespace ming */
