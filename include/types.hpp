#ifndef MING_COROUTINE_TYPES_HPP
#define MING_COROUTINE_TYPES_HPP

#include <functional>
#include <stdint.h>

namespace ming {
namespace coroutine {

constexpr size_t kStackSize = 10240;

using co_function_t = std::function<void()>;
using co_function_ptr = void (*)();
using co_id_t = size_t;
using co_stack_t = uint8_t[kStackSize];

enum CoroutineState { kReady, kRunning, kYield, kFinish };

} /* end of namespace coroutine */
} /* end of namespace ming */
#endif
