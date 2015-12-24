#include "coroutine.hpp"

namespace ming {
namespace coroutine {

void Coroutine::CoroutineGuard(Coroutine* co) {
    co->fn_();
    setcontext(&co->main_ctx_);
}

void yield() {
    g_current->Yield();
}

Coroutine* g_current;

} /* end of namespace coroutine */
} /* end of namespace ming */
