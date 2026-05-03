# 将 Qwerty Learner 库中的单词 json 文件转为 csv 格式

[Qwerty Learner 官网](https://qwertylearner.cn/)</br>
[Qwerty Learner github 仓库](https://github.com/RealKai42/qwerty-learner)

Qwerty Learner 单词 `json` 文件在 github 仓库 `public/dicts` 目录下

使用的 [cJSON](https://github.com/DaveGamble/cJSON) 库 解析 json 文件


## 编译

cmake 编译

```shell
mkdir build
cd build
cmake ..
make
```

## 使用

使用终端

需要到 `bin` 目录下使用

```shell
./json2csv target_file_name 
```