#include "all_func.h"

// Read the entire file content into the memory
char* read_file(const char* filename)
{
    FILE* f = fopen(filename, "rb");
    if (!f)
        return NULL;

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

// Concatenate file names and extensions
void join_filename_extension(char* out, int size, const char* output_name,const char* extension)
{
    snprintf(out, size, "%s.%s", output_name,extension);
}

// Get file dir
void get_file_info(char* out_dir, char*out_name,int size, const char* filepath)
{
    strncpy(out_dir, filepath, size);
    char* last_slash = strrchr(out_dir, '/');
#ifdef _WIN32
    if (!last_slash) last_slash = strrchr(out_dir, '\\');
#endif
    if (last_slash) {
        strncpy(out_name,last_slash+1,size);
        *(last_slash + 1) = '\0';
    } else {
        strncpy(out_name, filepath, size);
        out_dir[0] = '\0';
    }
}
// is json file
int is_json_file(const char* filename)
{
    if (!filename) return 0;

    size_t len = strlen(filename);

    if (len < 5) return 0;

    return strcmp(&filename[len - 5], ".json") == 0;
}
// compare extension
int file_has_suffix(const char* path, const char* suffix)
{
    if (path == NULL || suffix == NULL)
        return 1;

    size_t path_len = strlen(path);
    size_t suf_len  = strlen(suffix);

    if (path_len < suf_len)
        return 1;

    const char* start = path + (path_len - suf_len);
    return strcmp(start, suffix);
}

// Change the JSON extension to CSV
int extension_json2csv(char*out,const char*filename)
{
    size_t len = strlen(filename);
    memcpy(out,filename,len);
    out[len] = '\0';
    char *dot = strrchr(out, '.');
    if(dot != NULL)
        memcpy(dot,".csv\0",5);
    else 
        return 1;
    return 0;
}

// Concatenate the trans array into a row string within a cell
void concat_trans(cJSON* trans_array, char* buf, int buf_size)
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

// Main logic: JSON parsing + CSV export
int process(const char* in_json, const char* out_csv)
{
    char* json_buf = read_file(in_json);
    if (!json_buf)
    {
        printf("Error: Unable to read the input file %s\n", in_json);
        return -1;
    }

    // JSON parsing
    cJSON* root = cJSON_Parse(json_buf);
    if (!root || !cJSON_IsArray(root))
    {
        printf("Error: Invalid JSON format\n");
        cJSON_Delete(root);
        free(json_buf);
        return -1;
    }

    // open CSV file
    FILE* fp = fopen(out_csv, "w");
    if (!fp)
    {
        printf("Error: Unable to create output file %s\n", out_csv);
        cJSON_Delete(root);
        free(json_buf);
        return -1;
    }

    // Enter the table header
    fprintf(fp, "word,trans\n");

    cJSON* word_obj;
    cJSON_ArrayForEach(word_obj, root)
    {
        cJSON* name = cJSON_GetObjectItemCaseSensitive(word_obj, "name");
        cJSON* trans = cJSON_GetObjectItemCaseSensitive(word_obj, "trans");

        if (!cJSON_IsString(name) || !cJSON_IsArray(trans))
            continue;

        char trans_buf[2048] = {0};
        concat_trans(trans, trans_buf, sizeof(trans_buf));

        fprintf(fp, "\"%s\",\"%s\"\n", name->valuestring, trans_buf);
    }

    fclose(fp);
    cJSON_Delete(root);
    free(json_buf);
    return 0;
}
