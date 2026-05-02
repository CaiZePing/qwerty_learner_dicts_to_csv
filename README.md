# 将 Qwerty Learner 库中的单词 json 文件转为 csv 格式

[Qwerty Learner 官网](https://qwertylearner.cn/)</br>
[Qwerty Learner github 仓库](https://github.com/RealKai42/qwerty-learner)

使用的 [cJSON](https://github.com/DaveGamble/cJSON) 库 解析 json 文件

**编译**

```shell
gcc cJSON.c json2csv.c -o json2csv
```

使用

```shell
./json2csv target_file_name 
```
