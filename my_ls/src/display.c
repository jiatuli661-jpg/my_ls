//控制输出格式和颜色
#include"ls.h"
/*void display_file(const FileInfo *file,const char *path)
{
    mode_t mode=file->file_stat.st_mode;
    if(S_ISDIR(mode))
    {
        printf("\033[34m");
    }else if(S_ISLNK(mode))
    {
        //printf("\033[36m");
        struct stat tar_stat;
        if(stat(path,&tar_stat)==0)
        {
            if(S_ISDIR(tar_stat.st_mode))
            {
                printf("\033[34m");
            }else if(tar_stat.st_mode&(S_IXGRP| S_IXOTH| S_IXUSR))
            {
                printf("\033[32m");
            }else{
                printf("\033[36m");
            }
        }
        else {
            printf("\033[36m");
        }
    }else if(mode&(S_IXGRP|S_IXOTH|S_IXUSR))
    {
        printf("\033[32m");
    }
    printf("%s\033[0m",file->name);
}*/

void display_file(const FileInfo *file, const char *path)
{
    mode_t mode = file->file_stat.st_mode;

    if (S_ISDIR(mode))
    {
        printf("\033[1;34m");  // 目录：蓝色
    }
    else if (S_ISLNK(mode))
    {
        struct stat tar_stat;

        if (stat(path, &tar_stat) == 0)
        {
            if (S_ISDIR(tar_stat.st_mode))
            {
                printf("\033[1;34m");  // 指向目录：蓝色
            }
            else if (tar_stat.st_mode & (S_IXUSR | S_IXGRP | S_IXOTH))
            {
                printf("\033[1;32m");  // 指向可执行文件：绿色
            }
            else
            {
                printf("\033[1;36m");  // 普通链接：青色
            }
        }
        else
        {
            printf("\033[1;36m");
        }
    }
    else if (mode & (S_IXUSR | S_IXGRP | S_IXOTH))
    {
        printf("\033[1;32m");  // 可执行文件：绿色
    }

    printf("%s\033[0m", file->name);
}

void display_per(const FileInfo *file)
{
    mode_t mode=file->file_stat.st_mode;
    char qx[11];
    if(S_ISREG(mode))qx[0]='-';
        else if(S_ISDIR(mode))qx[0]='d';
        else if(S_ISLNK(mode))qx[0]='l';
        else if(S_ISCHR(mode))qx[0]='c';
        else if(S_ISBLK(mode))qx[0]='b';
        else if(S_ISFIFO(mode))qx[0]='p';
        else if(S_ISSOCK(mode))qx[0]='s';
        else qx[0]='?';
        qx[1]=(mode & S_IRUSR)?'r':'-';
        qx[2]=(mode & S_IWUSR)?'w':'-';
        qx[3]=(mode & S_IXUSR)?'x':'-';

        qx[4]=(mode & S_IRGRP)?'r':'-';
        qx[5]=(mode & S_IWGRP)?'w':'-';
        qx[6]=(mode & S_IXGRP)?'x':'-';

        qx[7]=(mode & S_IROTH)?'r':'-';
        qx[8]=(mode & S_IWOTH)?'w':'-';
        qx[9]=(mode & S_IXOTH)?'x':'-';
        qx[10]='\0';
        printf("%s ",qx);
}