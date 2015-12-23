#ifndef MING_COROUTINE_SCHEDULER_HPP
#define MING_COROUTINE_SCHEDULER_HPP

#include <map>
#include <memory>
#include <ucontext.h>

#include "coroutine.hpp"

namespace ming {
namespace coroutine {

/**
 * Singleton scheduler.
 */
class CoroutineScheduler{
public:
    CoroutineScheduler() = default;
    CoroutineScheduler(const CoroutineScheduler&) = delete;

    void Create(co_function_t fn);
    
    void Resume(co_id_t id);
    
    void Yield(co_id_t id);

private:

    std::map<co_id_t, std::shared_ptr<Coroutine>> coroutines_;
    ucontext_t main_ctx_;
    static co_id_t count_;
};

CoroutineScheduler& get_scheduler() {
    static CoroutineScheduler cs;
    return cs;
}

} /* end of namespace coroutine */
} /* end of namespace ming */
#endif
