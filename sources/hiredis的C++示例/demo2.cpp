#include<stdlib.h>
#include<time.h>
#include <stdio.h>
#include "redis.h"

int main()
{
    long i = 10000000L;
    clock_t start, finish;
    double duration;
    /* 测量一个事件持续的时间*/
    printf( "Time to do %ld empty loops is ", i );
    start = clock();
    while( i-- ) ;
    finish = clock();
    duration = (double)(finish - start) / CLOCKS_PER_SEC;
    printf( "%f seconds\n", duration );
 
    Redis *r = new Redis();
    if(!r->connect("127.0.0.1", 6379))
    {
            printf("connect error!\n");
            return 0;
    }
 
    ////////////////////////////////
    clock_t begin_tick_counts = clock();
    long object_nums = 1000000;
    char buff[32];
    for(int j=0;j<=object_nums;++j)
    {
        sprintf(buff, "key-%d",j);
        r->set(buff, buff);
    }
    clock_t end_tick_counts = clock();
    printf("object_nums:%ld, it costs %f seconds\n", object_nums, (double)(end_tick_counts-begin_tick_counts)/CLOCKS_PER_SEC);
    r->set("name", "json");
    printf("Get the name is %s\n", r->get("name").c_str());
    delete r;

    return 0;
}

