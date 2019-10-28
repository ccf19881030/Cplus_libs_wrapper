## [sqlite3的基本操作（插入，删除，更新，查询）](http://blog.chinaunix.net/uid-20665047-id-3936284.html)
```cpp
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sqlite3.h"

sqlite3 * db = NULL;

static int sn = 0;

void create_table(char * filename)
{
    char * sql;
    char * zErrMsg = 0;
    int rc;

    rc = sqlite3_open(filename,&db);
    //rc = sqlite3_open_v2(filename,&db,SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, NULL);
    if (rc)
    {
        fprintf(stderr,"can't open database%s\n",sqlite3_errmsg(db));
        sqlite3_close(db);
    }
    sql =  "CREATE TABLE save_data(num integer primary key, id int, data text, time text)";
    sqlite3_exec(db,sql,0,0,&zErrMsg);
}

void close_table(void)
{
    sqlite3_close(db);
}


void insert_record(char * table,int id,char * data,char * time)
{
    char * sql;
    char * zErrMsg = NULL;

    sql = sqlite3_mprintf("insert into %s values(null,%d,'%s','%s')",table,id,data,time);
    sqlite3_exec(db,sql,0,0,&zErrMsg);
    sqlite3_free(sql);
}

int sqlite_callback(void * userData,int numCol,char ** colData,char **colName)
{
    int i,offset = 0;
    char * buf,* tmp;

    buf = (char *)malloc(40 * sizeof(char));
    tmp = buf;
    memset(buf,0,40);

    //printf("%d %d\n",sizeof(buf),strlen(buf));
    for (i = 1;i < numCol;i++)
    {
        buf = buf + offset;
        sprintf(buf,"%s ",colData[i]);
        offset = strlen(colData[i]) + 1; //it's need one place for put a blank so the lenght add 1
    //    printf("i %d offset %d\n",i, offset);
    }
    printf("%.4d. %s \n",++sn,tmp);

    free(tmp);
    tmp = NULL;
    buf = NULL;

    return 0;
}


void search_all(char * table)
{
    char * sql;
    char * zErrMsg = 0;

    sn = 0;

    sql = sqlite3_mprintf("select * from %s",table);
    sqlite3_exec(db,sql,&sqlite_callback,0,&zErrMsg);
    sqlite3_free(sql);

}

void search_by_id(char * table,char * id)
{
    char * sql;
    char * zErrMsg = 0;

    sn = 0;

    sql = sqlite3_mprintf("select * from %s where id=%s",table,id);
    sqlite3_exec(db,sql,&sqlite_callback,0,&zErrMsg);
    sqlite3_free(sql);
}

void delete_by_id(char * table,char * id)
{
    int rc ;
    char * sql;
    char * zErrMsg = 0;
    sql = sqlite3_mprintf("delete from %s where id=%s",table,id);
    rc = sqlite3_exec(db,sql,0,0,&zErrMsg);
    sqlite3_free(sql);
}

void delete_all(char * table)
{
    char * sql;
    char * zErrMsg = 0;

    sql = sqlite3_mprintf("delete from %s",table);
    sqlite3_exec(db,sql,0,0,&zErrMsg);
    sqlite3_free(sql);
}

int main(int agrc,char * argv[])
{
    char * filename = "data.db";
    int i ;

    create_table(filename);

    for (i = 0 ;i < 10;i++)
    {
        insert_record("save_data",2000,"5678","2012-03-12 09:43:56");
        insert_record("save_data",2001,"5678","2012-03-12 09:43:56");
        insert_record("save_data",2002,"5678","2012-03-12 09:43:56");
        insert_record("save_data",2003,"5678","2012-03-12 09:43:56");
        insert_record("save_data",2004,"5678","2012-03-12 09:43:56");
        insert_record("save_data",2005,"5678","2012-03-12 09:43:56");
        insert_record("save_data",2006,"5678","2012-03-12 09:43:56");
        insert_record("save_data",2007,"5678","2012-03-12 09:43:56");
    }

    search_all("save_data");

    close_table();

    return 0;
}
```
