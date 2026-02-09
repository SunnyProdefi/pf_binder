// HelloLocal.cpp
#include "HelloApi.h"

#include <iostream>

namespace demo
{
    class HelloLocal final : public HelloApi
    {
    public:
        int32_t add(int32_t a, int32_t b) override { return a + b; }
    };
}  // namespace demo

int main()
{
    demo::HelloLocal hello;
    int32_t r = hello.add(7, 35);
    std::cout << "7 + 35 = " << r << "\n";
    return 0;
}
