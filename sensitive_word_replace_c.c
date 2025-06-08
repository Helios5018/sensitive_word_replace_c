#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>

long get_file_size(FILE* file_ptr) {
    // 获取文件大小
    long file_size = 0;
    fseek(file_ptr, 0, SEEK_END);
    file_size = ftell(file_ptr);
    //printf("file_size: %ld\n", file_size);
    fseek(file_ptr, 0, SEEK_SET);
    return file_size;
}

int main() {
    // 打开文件
    FILE* file_ptr = _wfopen(L"test_novel.txt", L"rb");
    if (file_ptr == NULL) {
        printf("File open failed.\n");
        return 1;
    }

    // 读取文件内容
    long file_size = get_file_size(file_ptr);

    // 定义字符串
    char novel_content[file_size+10];

    // 读取文件内容（字符串）到内存中
    int i = 0;
    char get_char;
    while((get_char = fgetc(file_ptr)) != EOF && i < file_size) {
        novel_content[i++] = (char)get_char;
    }
    novel_content[i] = '\0';

    // 关闭文件
    fclose(file_ptr);

    // 输出文件内容
    printf("%s\n", novel_content);

    return 0;
}
