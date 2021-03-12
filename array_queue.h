
#ifndef __ARRAY_QUEUE__
#define __ARRAY_QUEUE__

#define  MEM_SIZE(type, member)   sizeof(((type*)0)->member)
#define aq_malloc(sz)      malloc(sz)
#define aq_calloc(sz,num)  calloc(sz,num)
#define aq_free(ptr)       free(ptr)

typedef struct
{
    unsigned int f;
    unsigned int l;
    unsigned int qlen;
    unsigned int da[0];
}array_queue_t;

static inline array_queue_t *get_a_init_array_queue(unsigned int qlen)
{
    /*qlen means da len not include the head*/
    array_queue_t *aq = aq_calloc(sizeof(array_queue_t)+(qlen+1)*sizeof(unsigned int),1);
    if(NULL == aq)
    {
        return NULL;
    }
    aq->qlen = qlen+1;
    return aq;
}
static inline unsigned int init_array_queue(void *base,unsigned int base_len)
{
    unsigned int qlen = (base_len - sizeof(array_queue_t))/sizeof(unsigned int);
    array_queue_t *aq = base;
    aq->qlen = qlen;
    aq->f = 0;
    aq->l = 0;
    memset(aq->da,0,qlen*sizeof(unsigned int));
    return qlen-1;
}
static inline int aq_put(array_queue_t *aq,unsigned int d)
{
    unsigned int l = (aq->l+1)%aq->qlen;
    if(l == aq->f)
    {
        /*full*/
        return -1;
    }
    aq->da[aq->l] = d;
    aq->l = l;
    return 0;
}
static inline int aq_peek(array_queue_t *aq,unsigned int *out)
{
    if(aq->l == aq->f)
    {
        /*empty*/
        return -1;
    }
    *out = aq->da[aq->f];
    return 0;
}
static inline int aq_get(array_queue_t *aq,unsigned int *out)
{
    if(aq_peek(aq,out)<0)
    {
        return -1;
    }
    aq->f = (aq->f+1)%aq->qlen;
    return 0;
}

#endif
