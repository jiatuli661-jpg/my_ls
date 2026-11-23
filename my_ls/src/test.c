#include<stdio.h>
#include<stdlib.h>
int compare(const void*a,const void*b)
{
    int x=*(const int *)a;
    int y=*(const int *)b;
    return (x<y)-(x>y);
}
int main(void)
{
    int arr[]={2,5,2,6,7,4,2,4,6,12};
    int len=sizeof(arr)/sizeof(arr[0]);
    qsort(arr,len,sizeof(arr[0]),compare);
    for(int i=0;i<len;i++)
    {
        printf("%d ",arr[i]);
    }
    printf("\n");
    return 0;
}