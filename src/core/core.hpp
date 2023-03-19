#ifndef CORE_HPP
#define CORE_HPP

#include <cassert>

#define ASSERT_RETURN(expression)                                                                                      \
    {                                                                                                                  \
        if (!bool(expression))                                                                                         \
        {                                                                                                              \
            assert(expression);                                                                                        \
            return;                                                                                                    \
        }                                                                                                              \
    }
#define ASSERT_RETURN_VALUE(expression, value)                                                                         \
    {                                                                                                                  \
        if (!bool(expression))                                                                                         \
        {                                                                                                              \
            assert(expression);                                                                                        \
            return value;                                                                                              \
        }                                                                                                              \
    }
#define ASSERT_CONTINUE(expression)                                                                                    \
    {                                                                                                                  \
        if (!bool(expression))                                                                                         \
        {                                                                                                              \
            assert(expression);                                                                                        \
            continue;                                                                                                  \
        }                                                                                                              \
    }

#endif