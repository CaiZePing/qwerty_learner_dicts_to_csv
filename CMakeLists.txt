cmake_minimum_required(VERSION 3.10)

# 项目名称
project(json2csv)

# C / C++ / OBJC 标准
set(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_OSX_ARCHITECTURES "x86_64")  # 通用二进制（Intel + Apple Silicon）
set(CMAKE_OSX_DEPLOYMENT_TARGET "10.15" CACHE STRING "Minimum macOS version")

# 头文件路径
include_directories(
    ${PROJECT_SOURCE_DIR}
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/cJSON
)

# ==============================================================================
# 1. 构建命令行工具 json2csv
# ==============================================================================
set(CLI_SRC
    src/json2csv.c
    src/all_func.c
    src/cJSON.c
)
add_executable(json2csv_cli ${CLI_SRC})
set_target_properties(json2csv_cli PROPERTIES OUTPUT_NAME json2csv)

# ==============================================================================
# 2. 构建 ImGui + SDL2 + Metal GUI 主程序
# ==============================================================================
set(IMGUI_SRC
    src/imgui.cpp
    src/imgui_draw.cpp
    src/imgui_widgets.cpp
    src/imgui_tables.cpp
    src/imgui_demo.cpp
    src/imgui_impl_sdl2.cpp          # SDL2 后端
    src/imgui_impl_metal.mm          # Metal 后端
    src/main.mm                      # 主程序（Objective-C++）
    src/cJSON.c
    src/all_func.c
)

# GUI 可执行文件（macOS .app）
add_executable(${PROJECT_NAME} MACOSX_BUNDLE ${IMGUI_SRC})

# 给 .mm 文件开启 ARC（自动引用计数，必须）
set_source_files_properties(
    src/imgui_impl_metal.mm
    src/main.mm
    PROPERTIES COMPILE_FLAGS "-fobjc-arc"
)

# ==============================================================================
# macOS 框架 + SDL2 依赖
# ==============================================================================
if(APPLE)
    # 系统框架
    find_library(COCOA_LIBRARY Cocoa)
    find_library(METAL_LIBRARY Metal)
    find_library(QUARTZCORE_LIBRARY QuartzCore)
    find_library(MTALKIT_LIBRARY MetalKit)
    find_library(COREGRAPHICS_LIBRARY CoreGraphics)
    find_library(IOKIT_LIBRARY IOKit)

    # 查找 SDL2
    find_package(SDL2 REQUIRED)
    include_directories(${SDL2_INCLUDE_DIRS})
    set_target_properties(${PROJECT_NAME} PROPERTIES
        XCODE_ATTRIBUTE_LD_RUNPATH_SEARCH_PATHS "/usr/local/lib /opt/homebrew/lib"
    )
    # 链接所有库
    target_link_libraries(${PROJECT_NAME}
        ${COCOA_LIBRARY}
        ${METAL_LIBRARY}
        ${QUARTZCORE_LIBRARY}
        ${MTALKIT_LIBRARY}
        ${COREGRAPHICS_LIBRARY}
        ${IOKIT_LIBRARY}
        ${SDL2_LIBRARIES}
    )
endif()

# ==============================================================================
# macOS .app 打包配置
# ==============================================================================

set_target_properties(${PROJECT_NAME} PROPERTIES
    MACOSX_BUNDLE TRUE
    MACOSX_BUNDLE_GUI_IDENTIFIER "com.json2csv.app"
    MACOSX_BUNDLE_BUNDLE_VERSION "1.0"
    MACOSX_BUNDLE_SHORT_VERSION_STRING "1.0"
    MACOSX_BUNDLE_ICON_FILE "AppIcon.icns"

    XCODE_ATTRIBUTE_CODE_SIGNING_REQUIRED "NO"
    XCODE_ATTRIBUTE_CODE_SIGNING_ALLOWED "NO"
    XCODE_ATTRIBUTE_CODE_SIGN_IDENTITY ""
    XCODE_ATTRIBUTE_PRODUCT_BUNDLE_IDENTIFIER "com.json2csv.app"
)

# 图标资源
if(EXISTS ${PROJECT_SOURCE_DIR}/AppIcon.icns)
    target_sources(${PROJECT_NAME} PRIVATE AppIcon.icns)
    set_source_files_properties(AppIcon.icns PROPERTIES
        MACOSX_PACKAGE_LOCATION Resources
    )
endif()

# ==============================================================================
# 输出目录
# ==============================================================================
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)
