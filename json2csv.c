#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cJSON/cJSON.h"

// 读取整个文件内容到内存
static char* read_file(const char* filename)
{
    FILE* f = fopen(filename, "rb");
    if (!f) return NULL;

    fseek(f, 0, SEEK_END);
    long len = ftell(f);
    rewind(f);

    char* buf = (char*)malloc(len + 1);
    if (!buf)
    {
        fclose(f);
        return NULL;
    }

    fread(buf, 1, len, f);
    buf[len] = 0;
    fclose(f);
    return buf;
}

// 生成完整输出文件名：输出名.csv
static void make_output_filename(char* out, int size, const char* output_name,const char* extension)
{
    snprintf(out, size, "%s.%s", output_name,extension);
}

// 把 trans 数组拼接成 单元格内分行字符串（\n）
static void concat_trans(cJSON* trans_array, char* buf, int buf_size)
{
    buf[0] = 0;
    if (!cJSON_IsArray(trans_array)) return;

    int line = 0;
    cJSON* item;
    cJSON_ArrayForEach(item, trans_array)
    {
        if (!cJSON_IsString(item)) continue;
        if (line > 0) strncat(buf, "\n", buf_size - strlen(buf) - 1);
        strncat(buf, item->valuestring, buf_size - strlen(buf) - 1);
        line++;
    }
}

// 主逻辑：JSON解析 + 导出CSV
int process(const char* in_json, const char* out_csv)
{
    char* json_buf = read_file(in_json);
    if (!json_buf)
    {
        printf("错误：无法读取输入文件 %s\n", in_json);
        return -1;
    }

    // 解析JSON
    cJSON* root = cJSON_Parse(json_buf);
    if (!root || !cJSON_IsArray(root))
    {
        printf("错误：JSON格式无效\n");
        cJSON_Delete(root);
        free(json_buf);
        return -1;
    }

    // 打开CSV文件
    FILE* fp = fopen(out_csv, "w");
    if (!fp)
    {
        printf("错误：无法创建输出文件 %s\n", out_csv);
        cJSON_Delete(root);
        free(json_buf);
        return -1;
    }

    // 写入表头
    fprintf(fp, "单词,翻译\n");

    // 遍历数组每一项
    cJSON* word_obj;
    cJSON_ArrayForEach(word_obj, root)
    {
        cJSON* name = cJSON_GetObjectItemCaseSensitive(word_obj, "name");
        cJSON* trans = cJSON_GetObjectItemCaseSensitive(word_obj, "trans");

        if (!cJSON_IsString(name) || !cJSON_IsArray(trans))
            continue;

        // 拼接多行 trans
        char trans_buf[2048] = {0};
        concat_trans(trans, trans_buf, sizeof(trans_buf));

        // CSV规则：带换行/逗号的字段必须用双引号包裹
        fprintf(fp, "\"%s\",\"%s\"\n", name->valuestring, trans_buf);
    }

    fclose(fp);
    cJSON_Delete(root);
    free(json_buf);
    return 0;
}

int main(int argc, char* argv[])
{
    // 检查参数：必须输入2个参数
    if (argc < 2)
    {
        printf("用法：%s <输入JSON文件> [<输出文件名(无后缀)>]\n", argv[0]);
        printf("示例：%s xiao.json result\n", argv[0]);
        printf("将生成 result.csv\n");
        return 1;
    }
    char input_json[256];
    make_output_filename(input_json, sizeof(input_json), argv[1],"json");
    
    char out_filename[256];
    make_output_filename(out_filename, sizeof(out_filename), argc==3?argv[2]:argv[1],"csv");

    printf("开始转换...\n");
    printf("输入：%s\n", argv[1]);
    printf("输出：%s\n", out_filename);

    int ret = process(input_json, out_filename);
    if (ret == 0)
        printf("✅ 转换成功！\n");
    else
        printf("❌ 转换失败！\n");

    return ret;
}