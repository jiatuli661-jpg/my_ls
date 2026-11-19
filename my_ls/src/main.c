//程序入口，组织各种模块
#include "ls.h"
#include<locale.h>

int main(int argc,char *argv[])
{
    int show_long=0;
    int show_innode=0;
    int show_all=0;
    int show_blocks=0;
    int sort_by_time=0;
    int reverse=0;
    int rec=0;
    int opt;
    while((opt=getopt(argc,argv,"ailstrR"))!=-1)
    {
        switch(opt)
        {
            case'a':
            show_all=1;
            break;
            case'i':
            show_innode=1;
            break;
            case'l':
            show_long=1;
            break;
            case's':
            show_blocks=1;
            break;
            case't':
            sort_by_time=1;
            break;
            case'r':
            reverse=1;
            break;
            case'R':
            rec=1;
            break;
            default:
            fprintf(stderr,"输入错误 -%c\n",opt);
            return 1;
        }
    }
    const char*path=".";
    if(optind<argc)
    {
        path=argv[optind];
    }

    read_directory(path,show_all,show_innode,show_blocks,show_long,sort_by_time,reverse,rec);

    return 0;
}