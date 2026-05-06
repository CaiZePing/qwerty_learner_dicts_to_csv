# 计划：修复拖放功能并优化窗口布局

## 问题分析

### 问题1：拖入功能不灵敏

* SDL 默认可能未启用拖放功能

* 需要设置 SDL\_HINT\_DROPFILE\_ENABLE 提示

### 问题2：工作目录显示

* 用户要求在 ImGui 窗口最上面显示工作目录

### 问题3：主窗口与 ImGui 窗口合并

* 优化窗口布局，只显示必要内容

## 实现步骤

### 1. 修复拖放功能

* 添加 SDL\_SetHint(SDL\_HINT\_DROPFILE\_ENABLE, "1") 启用拖放

* **拖放时接受任何文件，不在拖放时检查扩展名**

### 2. 添加工作目录显示（窗口最上面）

* 使用 getcwd() 获取当前工作目录

* 在 ImGui 窗口最顶部显示工作目录信息

### 3. 在转换时检查文件类型

* 点击"转换"按钮时检查文件是否为 .json 扩展名

* 如果不是 .json 文件，显示错误消息

### 4. 优化窗口布局

* 将 SDL 窗口标题改为 "JSON to CSV"

* 调整初始窗口大小

## 修改的关键文件

* `/Users/mac/Desktop/learn/Qwerty_word2csv/src/main.mm`

## 具体修改点

1. **SDL 初始化后添加拖放提示**

   ```cpp
   SDL_SetHint(SDL_HINT_DROPFILE_ENABLE, "1");
   ```

2. **添加工作目录变量**

   ```cpp
   char cwd[512] = {0};
   getcwd(cwd, sizeof(cwd));
   ```

3. **修改拖放逻辑，接受任何文件（不在拖放时检查类型）**

   ```cpp
   if (event.type == SDL_DROPFILE) {
       char* dropped_file = event.drop.file;
       if (dropped_file != nullptr) {
           strncpy(input_path, dropped_file, sizeof(input_path) - 1);
           SDL_free(dropped_file);
       }
   }
   ```

4. **在 ImGui 窗口最上面显示工作目录**

   ```cpp
   ImGui::Text("当前工作目录: %s", cwd);
   ```

5. **在转换按钮点击时检查文件类型**（已有，保留）

   ```cpp
   if (file_has_suffix(input_path, ".json") != 0) {
       strncpy(status_message, "❌ 文件扩展名不是 .json", sizeof(status_message) - 1);
   }
   ```

6. **修改窗口标题和大小**

   ```cpp
   SDL_CreateWindow("JSON to CSV", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 200, SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);
   ```

