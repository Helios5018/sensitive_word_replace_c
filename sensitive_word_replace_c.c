#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>
#include <string.h>

char** get_sensitive_words_list(){
    FILE* sensitive_words_file_ptr; 
    char** sensitive_words = NULL;
    char line_content[256];
    int sensitive_word_count = 0;

    // 打开敏感词文件
    sensitive_words_file_ptr = fopen("./sensitive_words/sw_all_test.txt", "rb");
    if (sensitive_words_file_ptr == NULL) {
        printf("File open failed.\n");
        return NULL;
    }
    
    // 读取敏感词文件内容
    while (fgets(line_content, sizeof(line_content), sensitive_words_file_ptr) != NULL) {
        // 去除换行符
        line_content[strcspn(line_content, "\n")] = '\0';
        //printf("line_content: %s\n", line_content);

        // 动态扩展敏感词数组大小
        sensitive_words = (char**)realloc(sensitive_words, (sensitive_word_count + 2) * sizeof(char*));
        if (sensitive_words == NULL) {
            printf("Memory re-allocation failed.\n");
            free(sensitive_words);
            fclose(sensitive_words_file_ptr);
            return NULL;
        }

        // 为当前词语分配内存
        sensitive_words[sensitive_word_count] = (char*)malloc(strlen(line_content) + 1); 
        if (sensitive_words[sensitive_word_count] == NULL) {
            printf("Memory allocation failed.\n");
            free(sensitive_words);
            fclose(sensitive_words_file_ptr);
            return NULL;
        }    

        // 复制敏感词到数组中
        strcpy(sensitive_words[sensitive_word_count], line_content);

        // 敏感词统计数量加一
        sensitive_word_count++;
    }

    // 为敏感词数组添加结束标志
    sensitive_words[sensitive_word_count] = NULL;

    // 关闭敏感词文件
    fclose(sensitive_words_file_ptr);



    return sensitive_words;
}

long get_file_size(const wchar_t* novel_address) {
    // 打开文件
    FILE* file_ptr = _wfopen(novel_address, L"rb");
    if (file_ptr == NULL) {
        printf("File open failed.\n");
        return 1;
    }
    // 获取文件大小
    long file_size = 0;
    fseek(file_ptr, 0, SEEK_END);
    file_size = ftell(file_ptr);
    //printf("file_size: %ld\n", file_size);
    fseek(file_ptr, 0, SEEK_SET);
    return file_size;
}

int get_novel_content(const wchar_t* novel_address, char* novel_content, long file_size) {
    // 打开文件
    FILE* file_ptr = _wfopen(novel_address, L"rb");
    if (file_ptr == NULL) {
        printf("File open failed.\n");
        return 1;
    }

    // 读取文件内容（字符串）到内存中
    int i = 0;
    char get_char;
    while((get_char = fgetc(file_ptr)) != EOF && i < file_size) {
        novel_content[i++] = (char)get_char;
    }
    novel_content[i] = '\0';

    // 关闭文件
    fclose(file_ptr);

    return 0;
}

int main() {
    // 读取小说长度
    long file_size = get_file_size(L"重生80.txt");

    // 获取小说字符串
    char novel_content[file_size+10];
    get_novel_content(L"重生80.txt", novel_content, file_size);
    printf("%s\n", novel_content);

    // 读取敏感词列表
    char** sensitive_words = get_sensitive_words_list();
    for (int i =0; sensitive_words[i] != NULL; i++) {
        printf("%s\n", sensitive_words[i]);
    }

    // 释放敏感词数组内存
    for (int i =0; sensitive_words[i] != NULL; i++) {
        free(sensitive_words[i]);
    }
    free(sensitive_words);
    
    return 0;
    
}
