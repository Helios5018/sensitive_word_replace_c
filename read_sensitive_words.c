#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>
#include <string.h>

int main(){
    FILE* sensitive_words_file_ptr; 
    char line_content[256];
    char** sensitive_words = NULL;
    int sensitive_word_count = 0;

    // 打开敏感词文件
    sensitive_words_file_ptr = fopen("./sensitive_words/sw_all.txt", "rb");
    if (sensitive_words_file_ptr == NULL) {
        printf("File open failed.\n");
        return 1;
    }
    
    // 读取敏感词文件内容
    while (fgets(line_content, sizeof(line_content), sensitive_words_file_ptr) != NULL) {
        // 去除换行符
        line_content[strcspn(line_content, "\n")] = '\0';
        //printf("line_content: %s\n", line_content);

        // 动态扩展敏感词数组大小
        sensitive_words = (char**)realloc(sensitive_words, (sensitive_word_count + 1) * sizeof(char*));
        if (sensitive_words == NULL) {
            printf("Memory re-allocation failed.\n");
            return 1;
        }

        // 为当前词语分配内存
        sensitive_words[sensitive_word_count] = (char*)malloc(strlen(line_content) + 1); 
        if (sensitive_words[sensitive_word_count] == NULL) {
            printf("Memory allocation failed.\n");
            return 1;
        }    

        // 复制敏感词到数组中
        strcpy(sensitive_words[sensitive_word_count], line_content);

        // 敏感词统计数量加一
        sensitive_word_count++;
    }

    // 关闭敏感词文件
    fclose(sensitive_words_file_ptr);

    // 输出敏感词数组
    for (int i =0; i < sensitive_word_count; i++) {
        printf("%s\n", sensitive_words[i]);
    }

    // 释放敏感词数组内存
    for (int i = 0; i < sensitive_word_count; i++) {
        free(sensitive_words[i]);
    }
    free(sensitive_words);

    return 0;

}
