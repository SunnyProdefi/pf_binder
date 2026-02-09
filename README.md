# pf_binder

一个基于 Android libbinder 的极简 Binder 示例，包含：

- `hello_service`：注册 `demo.hello` 服务，提供 `add(a, b)`。
- `hello_client`：通过 Binder 调用 `add(7, 35)`。

## 依赖

在 WSL 中使用 Binder，需要具备以下条件：

1. **libbinder 头文件与库**  
   可通过以下两种方式之一提供：
   - **系统包**（如果发行版提供）：例如 `libbinder-dev`、`libutils-dev` 等。
   - **AOSP 源码编译**：从 AOSP 构建 `libbinder`、`libutils`、`liblog`，并设置 `ANDROID_ROOT` 指向 AOSP 源码根目录。

2. **Binder 内核驱动**  
   WSL2 默认不一定启用 Binder 驱动。若没有 `/dev/binder` 或 `binderfs`，服务端会启动失败。

## 构建

```bash
cmake -S . -B build
cmake --build build -j
```

如需显式指定头文件/库路径：

```bash
cmake -S . -B build \
  -DANDROID_ROOT=/path/to/aosp \
  -DBINDER_INCLUDE_DIR=/path/to/include \
  -DUTILS_INCLUDE_DIR=/path/to/include
cmake --build build -j
```

## 运行

先启动服务端（会阻塞等待请求）：

```bash
./build/hello_service
```

另开一个终端运行客户端：

```bash
./build/hello_client
```

预期输出：

```
7 + 35 = 42
```

如果看到类似 `service demo.hello not found` 或服务端无法启动，通常是因为 **Binder 驱动未就绪**。请确认 WSL 内核是否启用了 Binder 支持，并已挂载 `binderfs` 或创建 `/dev/binder` 设备。 
