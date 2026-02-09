// HelloClient.cpp
#include "IHello.h"

#include <binder/IServiceManager.h>
#include <binder/IBinder.h>
#include <utils/String16.h>
#include <iostream>

using android::sp;
using android::String16;

int main()
{
    sp<android::IServiceManager> sm = android::defaultServiceManager();
    sp<android::IBinder> binder = sm->getService(String16("demo.hello"));
    if (binder == nullptr)
    {
        std::cerr << "service demo.hello not found\n";
        return 1;
    }

    // interface_cast 会根据 META 信息创建 BpHello（Proxy）
    sp<demo::IHello> hello = android::interface_cast<demo::IHello>(binder);

    int32_t r = hello->add(7, 35);
    std::cout << "7 + 35 = " << r << "\n";
    return 0;
}
