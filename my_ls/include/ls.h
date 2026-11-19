//统一声明结构体函数选项
#ifndef LS_H
#define LS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>
#include <errno.h>

/* 命令行参数选项 */
typedef struct
{
    int show_all;       // -a：显示隐藏文件
    int long_format;   // -l：详细信息
    int recursive;     // -R：递归显示目录
    int sort_by_time;  // -t：按修改时间排序
    int reverse;       // -r：反向排序
    int show_inode;    // -i：显示 inode 编号
    int show_blocks;   // -s：显示占用块数
} Options;
int parse_options(int argc, char *argv[], Options *options);
typedef struct
{
    char name[256];
    struct stat file_stat;
}FileInfo;
void read_directory(const char *path,int show_all,int show_inode,int show_blocks,int show_long,int sort_by_time,int reverse,int rec);
#endif