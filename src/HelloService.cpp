// 放在某个 .cpp 里（比如 HelloService.cpp 顶部）
#include "IHello.h"

namespace demo
{
    IMPLEMENT_META_INTERFACE(Hello, "demo.IHello");
}  // namespace demo

// HelloService.cpp
#include "IHello.h"

#include <binder/IServiceManager.h>
#include <binder/ProcessState.h>
#include <binder/IPCThreadState.h>
#include <utils/Log.h>

using android::sp;
using android::String16;

namespace demo
{

    // 真正的业务实现：继承 BnHello（Stub），覆写 add()
    class HelloService : public BnHello
    {
    public:
        int32_t add(int32_t a, int32_t b) override { return a + b; }
    };

}  // namespace demo

int main()
{
    // 1) 启 binder（创建 /dev/binder 映射、线程池等）
    sp<android::ProcessState> ps = android::ProcessState::self();
    ps->startThreadPool();

    // 2) 注册服务到 ServiceManager（名字就是客户端查询的 key）
    sp<android::IServiceManager> sm = android::defaultServiceManager();
    sm->addService(String16("demo.hello"), new demo::HelloService());

    // 3) 进入 binder 循环，等待客户端 transact
    android::IPCThreadState::self()->joinThreadPool();
    return 0;
}
