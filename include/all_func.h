#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cJSON/cJSON.h"


/**
 * handle file
 */

// Read the entire file content into the memory
char* read_file(const char* filename);

// Concatenate file names and extensions
void join_filename_extension(char* out, int size, const char* output_name,const char* extension);

// Get file dir
void get_file_info(char* out_dir, char*out_name,int size, const char* filepath);

// is json file
// int is_json_file(const char* filename);

// compare extension
int file_has_suffix(const char* path, const char* suffix);

// Change the JSON extension to CSV
int extension_json2csv(char*out,const char*filename);

/**
 * handle JSON to CSV
 */

// Concatenate the trans array into a row string within a cell
void concat_trans(cJSON* trans_array, char* buf, int buf_size);

// Main logic: JSON parsing + CSV export
int process(const char* in_json, const char* out_csv);