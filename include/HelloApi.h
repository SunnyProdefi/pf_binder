// HelloApi.h
#pragma once

#include <cstdint>

namespace demo
{
    class HelloApi
    {
    public:
        virtual ~HelloApi() = default;
        virtual int32_t add(int32_t a, int32_t b) = 0;
    };
}  // namespace demo
