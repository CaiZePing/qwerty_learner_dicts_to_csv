# 计划：JSON 转 CSV GUI 应用程序

## 任务概述

修改 `/Users/mac/Desktop/learn/Qwerty_word2csv/src/main.mm`，添加文件输入和转换按钮，并集成 JSON 转 CSV 转换功能。

## 实现步骤

### 1. 添加所需头文件

* 添加 `#include "all_func.h"` 以包含转换函数

* 添加必要的 ImGui 头文件用于输入文本和按钮

### 2. 添加状态变量

在 main 函数中添加：

* `char input_path[512] = {0}` - 存储 JSON 文件路径

* `char status_message[256] = {0}` - 存储转换结果消息

### 3. 修改 UI 布局

将简单的 "Hello, world!" 窗口内容替换为：

* 单行布局，包含输入框和"转换"按钮

* 输入框：`ImGui::InputText()` 显示文件路径，支持拖放

* 按钮：`ImGui::Button("转换")` 触发转换

* 在下方显示状态消息

### 4. 实现文件拖放功能

使用 SDL2 事件处理检测文件拖放：

* 监听 `SDL_DROPFILE` 事件

* 当文件被拖入时，将文件路径复制到 `input_path`

* 验证拖放的文件是否具有 `.json` 扩展名

### 5. 集成转换逻辑

点击"转换"按钮时：

* 验证 `input_path` 不为空且具有 `.json` 扩展名

* 通过将 `.json` 替换为 `.csv` 生成输出 CSV 路径

* 调用 `process(input_path, output_csv_path)` 进行转换

* 显示成功/失败消息

### 6. 构建配置

CMakeLists.txt 已在 IMGUI\_SRC 中包含 `all_func.c` 和 `cJSON.c`，无需更改构建配置。

## 修改的关键文件

* `/Users/mac/Desktop/learn/Qwerty_word2csv/src/main.mm` - 主 UI 和集成逻辑

## UI 流程

1. 用户将 JSON 文件拖到窗口上 → 路径自动填充到输入框
2. 用户点击"转换"按钮
3. 执行转换，显示结果消息（✅ succeed / ❌ failure）
4. 在输入文件相同目录创建输出 CSV 文件

