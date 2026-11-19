//读取目录，处理目录项
#include<stdio.h>
#include<dirent.h>
#include<stdlib.h>
#include<errno.h>
#include<string.h>
#include"ls.h"
#include<pwd.h>
#include<grp.h>
#include<time.h>
int compare_time(const void*a,const void*b)
{
    const FileInfo *file_a=(const FileInfo *)a;
    const FileInfo *file_b=(const FileInfo *)b;
    if(file_a->file_stat.st_mtime>file_b->file_stat.st_mtime)
    {
        return -1;
    }
    if(file_a->file_stat.st_mtime<file_b->file_stat.st_mtime)
    {
        return 1;
    }
    return 0;
}
int compare_name(const void*a,const void*b)
{
    const FileInfo *file_a=(const FileInfo *)a;
    const FileInfo *file_b=(const FileInfo *)b;
    return strcmp(file_a->name,file_b->name);
}
void reverse_file(FileInfo *files,int count)
{
    for(int i=0;i<count/2;i++)
    {
        FileInfo temp=files[i];
        files[i]=files[count-i-1];
        files[count-i-1]=temp;
    }
}
void read_directory(const char *path,int show_all,int show_inode,int show_blocks,int show_long,int sort_by_time,int reverse,int rec)
{
    DIR *dir=opendir(path);
    if(dir==NULL)
    {
        perror("opendir");
        return;
    }
    if(rec)
    {
        printf("%s:\n",path);
    }
    FileInfo *files=NULL;//动态数组的起始地址
    int count=0;//当前已经保存的文件数量
    int capacity=0;//当前数组能保存多少个文件

    char **subdirs=NULL;//保存子目录路径的动态数组
    int dir_count=0;//当前保存了多少个子目录
    int dir_capacity=0;//当前数组容量

    //char newpath[1024];
    struct dirent *entry;
    struct stat file_lstat;
    while(1)
    {
        errno=0;
        entry=readdir(dir);
        if(entry==NULL)
        {
            if(errno!=0)
            {
                perror("readdir");
            }
            break;
        }
        if(show_all==0 && entry->d_name[0]=='.')
        {
            continue;
        }
        size_t path_len=strlen(path);
        size_t name_len=strlen(entry->d_name);
        char *newpath=malloc(path_len+name_len+2);
        if(newpath==NULL)
        {
        perror("malloc");
        goto cleanup;
        }
        int n=snprintf(newpath,path_len+name_len+2,"%s/%s",path,entry->d_name);
        if(n<0)
        {
            fprintf(stderr,"路径格式化失败\n");
            free(newpath);
            continue;
        }
        if(path_len+name_len+2<=(size_t)n)
        {
            fprintf(stderr,"路径太长，无法处理\n");
            free(newpath);
            continue;
        }
        if(lstat(newpath,&file_lstat)==-1)
        {
            perror("lstat");
            free(newpath);
            continue;
        }
        if(S_ISDIR(file_lstat.st_mode))
        {
            if(strcmp(entry->d_name,".")!=0&&strcmp(entry->d_name,"..")!=0&&rec)
            {
                if(dir_count==dir_capacity)
                {
                    int new_capacity=dir_capacity==0?8:dir_capacity*2;
                    char **temp=realloc(subdirs,(size_t)new_capacity*sizeof(*subdirs));
                    if(temp==NULL)
                    {
                        perror("realloc");
                        free(newpath);
                        goto cleanup;
                    }else{
                        subdirs=temp;
                        dir_capacity=new_capacity;
                    }
                }
                if(dir_count<dir_capacity)
                {
                    subdirs[dir_count]=malloc(strlen(newpath)+1);
                    if(subdirs[dir_count]==NULL)
                    {
                        perror("malloc");
                    }else{
                        strcpy(subdirs[dir_count],newpath);
                        dir_count++;
                    }
                }
            }
        }
        if(count==capacity)
        {
            int new_capacity=(capacity==0)?16:capacity*2;
            FileInfo *temp=realloc(files,new_capacity*sizeof(FileInfo));
        
        if(temp==NULL)
        {
            perror("realloc");
            closedir(dir);
            goto cleanup;
        }
        files=temp;
        capacity=new_capacity;
        } 
        strcpy(files[count].name,entry->d_name);
        files[count].file_stat=file_lstat;
        count++;
        free(newpath);
        /*if(show_blocks)
        {
            printf("%ld ",(long)file_lstat.st_blocks);
        }
        if(show_inode)
        {
            printf("%lu ",(unsigned long)file_lstat.st_ino);
        }
        if(show_long)
        {
        if(S_ISREG(file_lstat.st_mode))
        {
            printf("-");
        }else if(S_ISDIR(file_lstat.st_mode))
        {
            printf("d");
        }else if(S_ISLNK(file_lstat.st_mode))
        {
            printf("l");
        }else if(S_ISCHR(file_lstat.st_mode))
        {
            printf("c");
        }else if(S_ISBLK(file_lstat.st_mode))
        {
            printf("b");
        }else if(S_ISFIFO(file_lstat.st_mode))
        {
            printf("p");
        }else if(S_ISSOCK(file_lstat.st_mode))
        {
            printf("s");
        }else{
            printf("?");
        }
        if(file_lstat.st_mode & S_IRUSR)
        {
            printf("r");
        }else{
            printf("-");
        }
        if(file_lstat.st_mode & S_IWUSR)
        {
            printf("w");
        }else{
            printf("-");
        }
        if(file_lstat.st_mode & S_IXUSR)
        {
            printf("x");
        }else{
            printf("-");
        }
        if(file_lstat.st_mode & S_IRGRP)
        {
            printf("r");
        }else{
            printf("-");
        }
        if(file_lstat.st_mode & S_IWGRP)
        {
            printf("w");
        }else{
            printf("-");
        }
        if(file_lstat.st_mode & S_IXGRP)
        {
            printf("x");
        }else{
            printf("-");
        }
        if(file_lstat.st_mode & S_IROTH)
        {
            printf("r");
        }else{
            printf("-");
        }
        if(file_lstat.st_mode & S_IWOTH)
        {
            printf("w");
        }else{
            printf("-");
        }
        if(file_lstat.st_mode & S_IXOTH)
        {
            printf("x");
        }else{
            printf("-");
        }
         printf(" %lu ",(unsigned long)file_lstat.st_nlink);
        struct passwd *pw=getpwuid(file_lstat.st_uid);
        if(pw!=NULL)
        {
            printf("%s ",pw->pw_name);
        }else {
            printf("%lu ",(unsigned long)file_lstat.st_uid);
        }
        struct group *gr=getgrgid(file_lstat.st_gid);
        if(gr!=NULL)
        {
            printf("%s ",gr->gr_name);
        }else {
            printf("%lu ",(unsigned long)file_lstat.st_gid);
        }
        printf("%ld ",(long)file_lstat.st_size);
        struct tm *time_info=localtime(&file_lstat.st_mtime);
        if(time_info==NULL)
        {
            perror("localtime");
            continue;
        }
        char time_buf[64];
        if(strftime(time_buf,sizeof(time_buf),"%b %d %H:%M",time_info)==0)
        {
            fprintf(stderr,"格式化失败\n");
            continue;
        }
        printf("%s ",time_buf);
    }
        printf("%s\n",entry->d_name);
    */}
    if(sort_by_time)
    {
        qsort(files,count,sizeof(FileInfo),compare_time);
    }else{
        qsort(files,count,sizeof(FileInfo),compare_name);
    }
    if(reverse)
    {
        reverse_file(files,count);
    }
    /*for(int i=0;i<count;i++)
    {
        printf("%ld %s\n",(long)files[i].file_stat.st_mtime,files[i].name);
    }*/
   for(int i=0;i<count;i++)
   {
    if(show_inode)
    {
        printf("%lu ",(unsigned long)files[i].file_stat.st_ino);
    }
    if(show_blocks)
    {
        printf("%ld ",(long)files[i].file_stat.st_blocks);
    }
    char qx[11];
    if(show_long)
    {
        mode_t mode=files[i].file_stat.st_mode;
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
        printf("%ld ",(long)files[i].file_stat.st_nlink);
        struct passwd *pw=getpwuid(files[i].file_stat.st_uid);
        struct group *group=getgrgid(files[i].file_stat.st_gid);
        if(pw!=NULL)
        {
            printf("%s ",pw->pw_name);
        }else {
            printf("%u ",(unsigned int)files[i].file_stat.st_uid);
        }
        if(group!=NULL)
        {
            printf("%s ",group->gr_name);
        }else {
            printf("%u ",(unsigned int)files[i].file_stat.st_gid);
        }
        printf("%ld ",(long)files[i].file_stat.st_size);
    struct tm *time_inof=localtime(&files[i].file_stat.st_mtime);
    char time_buf[64];
    if(time_inof==NULL)
    {
        fprintf(stderr,"localtime转换失败");
    }else if(strftime(time_buf,sizeof(time_buf),"%b %d %H:%M",time_inof)==0)
    {
        fprintf(stderr,"格式化失败");
    }else{
        printf("%s ",time_buf);
    }
    }
    
    printf("%s\n",files[i].name);
   }
   if(rec)
   {
    for(int i=0;i<dir_count;i++)
    {
        read_directory(subdirs[i],show_all,show_inode,show_blocks,show_long,sort_by_time,reverse,rec);
    }
   }
   cleanup:
   for(int i=0;i<dir_count;i++)
   {
    free(subdirs[i]);
   }
   free(subdirs);
    free(files);
    if(closedir(dir)==-1)
    {
        perror("closedir");
        return;
    }
    return;
}