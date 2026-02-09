// IHello.h
#pragma once

#include "HelloApi.h"

#include <binder/IInterface.h>
#include <binder/IBinder.h>
#include <binder/Parcel.h>
#include <utils/String16.h>

namespace demo
{

    class IHello : public android::IInterface, public HelloApi
    {
    public:
        DECLARE_META_INTERFACE(Hello);

        // 远程接口：你想暴露给客户端的方法
        virtual int32_t add(int32_t a, int32_t b) = 0;
    };

    // -------------------- Proxy（客户端侧） --------------------
    // Bp = Binder Proxy：把函数调用 -> Parcel -> transact
    class BpHello : public android::BpInterface<IHello>
    {
    public:
        explicit BpHello(const android::sp<android::IBinder>& impl) : android::BpInterface<IHello>(impl) {}

        int32_t add(int32_t a, int32_t b) override
        {
            android::Parcel data, reply;
            data.writeInterfaceToken(IHello::getInterfaceDescriptor());
            data.writeInt32(a);
            data.writeInt32(b);

            // transact：跨进程进入 binder driver
            android::status_t st = remote()->transact(TRANSACTION_add, data, &reply);
            if (st != android::OK)
            {
                // 生产代码里通常会更精细处理 DEAD_OBJECT 等错误
                return -1;
            }
            return reply.readInt32();
        }

    private:
        enum
        {
            TRANSACTION_add = android::IBinder::FIRST_CALL_TRANSACTION + 0,
        };
    };

    // -------------------- Stub（服务端侧） --------------------
    // Bn = Binder Native：接收 transact -> onTransact -> 解包 -> 调用实现
    class BnHello : public android::BnInterface<IHello>
    {
    public:
        android::status_t onTransact(uint32_t code, const android::Parcel& data, android::Parcel* reply, uint32_t flags = 0) override
        {
            switch (code)
            {
            case TRANSACTION_add:
            {
                // 校验接口 token，防止误调用/攻击
                CHECK_INTERFACE(IHello, data, reply);

                int32_t a = data.readInt32();
                int32_t b = data.readInt32();
                int32_t out = add(a, b);  // 调到真正实现（子类覆写）
                reply->writeInt32(out);
                return android::NO_ERROR;
            }
            default:
                return android::BnInterface<IHello>::onTransact(code, data, reply, flags);
            }
        }

    private:
        enum
        {
            TRANSACTION_add = android::IBinder::FIRST_CALL_TRANSACTION + 0,
        };
    };

}  // namespace demo
