
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <errno.h>
#include "mem_debug.h"

static int get_rwlock_read(pthread_rwlock_t *rwlock)
{
    return pthread_rwlock_rdlock(rwlock);
}
static int get_rwlock_write(pthread_rwlock_t *rwlock)
{
    return pthread_rwlock_wrlock(rwlock);
}
static int free_rwlock_rw(pthread_rwlock_t *rwlock)
{
    return pthread_rwlock_unlock(rwlock);
}

struct mem_debug_head
{
    int init_flag;
    struct mem_debug_node *phead;
    pthread_rwlock_t   *rwlock;
    pthread_rwlock_t __rwlock;
    int (*get_rlock)(pthread_rwlock_t *rwlock);
    int (*get_wlock)(pthread_rwlock_t *rwlock);
    int (*free_rwlock)(pthread_rwlock_t *rwlock);
};

int init_mem_debug_head(struct mem_debug_head *head)
{
    int rc = 0;
    head->phead = NULL;
    rc = pthread_rwlock_init(&head->__rwlock,NULL);
    if(0 != rc)
    {
        printf("init_rwlock_fail=%d errno=%d\n",rc,errno);
        return -1;
    }
    head->rwlock = &head->__rwlock;
    head->get_rlock = get_rwlock_read;
    head->get_wlock = get_rwlock_write;
    head->free_rwlock = free_rwlock_rw;
    head->init_flag = 1;
    return 0;
}

static int __show_node(struct mem_debug_node *node,FILE *fp)
{
    return fprintf(fp,"key(0x%lx)==>%s\n",(unsigned long)node->ptr,node->info);
}
static struct mem_debug_head __head;
int mem_debug_show_all(FILE *fp)
{
    int count = 0;
    if(NULL == fp)
    {
        __head.get_rlock(__head.rwlock);
        count = HASH_COUNT(__head.phead);
        __head.free_rwlock(__head.rwlock);
        return count;
    }
    struct mem_debug_node *s, *tmp;
    fprintf(fp,"---------------------------\n");
    __head.get_rlock(__head.rwlock);
    HASH_ITER(hh,__head.phead,s,tmp) 
    {
        count++;
        __show_node(s,fp);
    }
    __head.free_rwlock(__head.rwlock);
    fprintf(fp,"###########################\n");
    return count;
}
void* __dg_op_alloc_free(unsigned int len,unsigned int num,void *ptr,int flag,const char *file,int line,const char *func)
{
    int rc = 0;
    if(0 == __head.init_flag)
    {
        rc = init_mem_debug_head(&__head);
        if(rc < 0)
        {
            printf("init_mem_debug_head_fail_rc=%d\n",rc);
            return NULL;
        }
    }
    int add = 0;
    int temp = 0;
    struct mem_debug_node *find_n = NULL;
    switch(flag)
    {
        case __e_flag_malloc:
            add = 1;
            num = 1;
            ptr = real_malloc(len);
            if(NULL == ptr)
            {
                return NULL;
            }
            break;
        case __e_flag_calloc:
            add = 1;
            ptr = real_calloc(len,num);
            if(NULL == ptr)
            {
                return NULL;
            }
            break;
        case __e_flag_free:
            add = 0;
            break;
        default:
            break;
    }
    if(1 == add)
    {
        __head.get_rlock(__head.rwlock);    
        HASH_FIND_PTR(__head.phead,&ptr,find_n);
        __head.free_rwlock(__head.rwlock);
        if(NULL != find_n)
        {
            printf("error occur the key have:0x%lx\n",(unsigned long)ptr);
            __show_node(find_n,stdout);
            real_free(ptr);
            return NULL;
        }
        find_n = real_malloc(sizeof(struct mem_debug_node));
        if(NULL == find_n)
        {
            printf("can't alloc node\n");
            real_free(ptr);
            return NULL;
        }
        find_n->ptr = ptr;
        find_n->len = len*num;
        temp = snprintf(find_n->info,MEM_DEBUG_MAX_INFO_LEN-1,"len(%d)file(%s)line(%d)flag(%d)func(%s)",find_n->len,file,line,flag,func);
        find_n->info[temp] = '\0';
        __head.get_wlock(__head.rwlock);
        HASH_ADD_PTR(__head.phead,ptr,find_n);
        __head.free_rwlock(__head.rwlock);
        //show_all();
        return ptr;
    }
    else
    {
            //show_all();
            __head.get_wlock(__head.rwlock);    
            HASH_FIND_PTR(__head.phead,&ptr,find_n);
            if(NULL == find_n)
            {
                printf("error occur free the key :0x%lx\n",(unsigned long)ptr);
                real_free(ptr);
                __head.free_rwlock(__head.rwlock);
                return NULL;
            }
            else
            {
                HASH_DEL(__head.phead,find_n);
            }
            __head.free_rwlock(__head.rwlock);
            real_free(ptr);
            return ptr;
    }
}

