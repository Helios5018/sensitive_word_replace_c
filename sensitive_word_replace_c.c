#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>
#include <string.h>

/**
 * 读取敏感词列表
 * @return 敏感词字符串数组，以NULL结尾，失败返回NULL
 */
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

/**
 * 获取文件大小
 * @param novel_address 小说文件路径
 * @return 文件大小（字节数），失败返回-1
 */
long get_file_size(const char* novel_address) {
    // 打开文件
    FILE* file_ptr = fopen(novel_address, "rb");
    if (file_ptr == NULL) {
        printf("File open failed.\n");
        return -1;
    }
    // 获取文件大小
    long file_size = 0;
    fseek(file_ptr, 0, SEEK_END);
    file_size = ftell(file_ptr);
    //printf("file_size: %ld\n", file_size);
    fclose(file_ptr);
    return file_size;
}

/**
 * 读取小说文件内容
 * @param novel_address 小说文件路径
 * @return 文件内容字符串，失败返回NULL
 */
char* get_novel_content(const char* novel_address) {
    // 打开文件
    FILE* file_ptr = fopen(novel_address, "rb");
    if (file_ptr == NULL) {
        printf("File open failed.\n");
        return NULL;
    }

    // 获取文件大小
    long file_size = get_file_size(novel_address);
    if (file_size == -1) {
        fclose(file_ptr);
        return NULL;
    }

    // 使用malloc动态分配内存（在堆上分配）
    char* novel_content = (char*)malloc((file_size + 10) * sizeof(char));
    if (novel_content == NULL) {
        printf("Memory allocation failed.\n");
        fclose(file_ptr);
        return NULL;
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

    return novel_content;
}

/**
 * 主函数 - 敏感词替换程序入口
 */
int main() {
    // 输入小说地址
    const char* novel_address = "重生80.txt";
    
    // 获取小说字符串
    char* novel_content = get_novel_content(novel_address);
    //printf("%s\n", novel_content);


    // 获取敏感词列表
    char** sensitive_words = get_sensitive_words_list();
    /*
    for (int i =0; sensitive_words[i] != NULL; i++) {
        printf("%s\n", sensitive_words[i]);
    }
    */

    // 敏感词替换
    // 遍历小说字符串
    for (int j = 0; novel_content[j] != '\0'; j++) {
        // 遍历敏感词列表
        for (int k = 0; sensitive_words[k] != NULL; k++) {
            char novel_word[256];
            int word_len = strlen(sensitive_words[k]);
            
            // 检查是否有足够的字符进行比较
            if (j + word_len <= strlen(novel_content)) {
                strncpy(novel_word, novel_content + j, word_len);
                novel_word[word_len] = '\0';  // 确保字符串正确终止
            }
            // 使用strcmp进行字符串比较
            if (strcmp(novel_word, sensitive_words[k]) == 0) {
                printf("sensitive word: %s\n", sensitive_words[k]);
            }
        }
    }

    // 释放小说字符串内存
    free(novel_content);

    // 释放敏感词数组内存
    for (int i =0; sensitive_words[i] != NULL; i++) {
        free(sensitive_words[i]);
    }
    free(sensitive_words);
    
    return 0;
    
}
