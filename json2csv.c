#include "all_func.h"

int main(int argc, char* argv[])
{
    if (argc < 2)
    {
        printf("use: %s <JSON file name> [<out file name(no extension)>]\n", argv[0]);
        printf("example: %s words result\n", argv[0]);
        printf("out result.csv\n");
        return 1;
    }

    if(file_has_suffix(argv[1],".json"))
    {
        printf("The file extension is not .json\n");
        return 1;
    }
    // Get the file path
    char input_dir[256] = {0};
    char input_name[256] = {0};
    get_file_info(input_dir, input_name ,sizeof(input_dir), argv[1]);
    // Get the out file
    char out_filename[256];
    if(argc == 3)
        snprintf(out_filename, sizeof(out_filename), "%s%s.csv", input_dir, argv[2]);
    else
    {
        if(!extension_json2csv(input_name,input_name))
            snprintf(out_filename, sizeof(out_filename), "%s%s", input_dir,input_name);
        else
        {
            printf("The file does not have a suffix name");
            return 1;
        }
    }

    printf("begin...\n");
    printf("in: %s\n", argv[1]);
    printf("out: %s\n", out_filename);

    int ret = process(argv[1], out_filename);
    
    printf("%s\n",ret?"❌ failure":"✅ succeed");

    return ret;
}