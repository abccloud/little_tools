
#ifndef __EASY_BITS_OPS__
#define __EASY_BITS_OPS__

#define  MEM_SIZE(type, member)   sizeof(((type*)0)->member)
#define ebp_malloc(sz)      malloc(sz)
#define ebp_calloc(sz,num)  calloc(sz,num)
#define ebp_free(ptr)       free(ptr)

typedef struct
{
    unsigned int bytes_len;
    unsigned char da[0];
}easy_bits_pool_t;

static inline easy_bits_pool_t *get_a_init_ebp(unsigned int bits_len)
{
    /*if len is multiples of 8, we use len/8; 
     * if not we should len/8 + 1,so*/
    unsigned int bytes_len = (bits_len+7)/8;
    easy_bits_pool_t *p_ebp = ebp_calloc(sizeof(easy_bits_pool_t)+bytes_len,1);
    if(NULL == p_ebp)
    {
        return NULL;
    }
    p_ebp->bytes_len = bytes_len;
    return p_ebp;
}
static inline unsigned int init_ebp(void *base,unsigned int base_len)
{
    unsigned int bytes_len = (base_len-MEM_SIZE(easy_bits_pool_t,bytes_len));
    easy_bits_pool_t *p_ebp = base;
    p_ebp->bytes_len = bytes_len;
    memset(p_ebp->da,0,bytes_len);
    return bytes_len*8;
}
static inline int ebp_set_bit(easy_bits_pool_t *p_ebp,unsigned int pos)
{
    if(pos >= p_ebp->bytes_len*8)
    {
        return -1;
    }
    int pos_0 = pos/8;
    int pos_1 = pos%8;
    p_ebp->da[pos_0] |= (1<<pos_1);
    return 0;
}
static inline int ebp_clear_bit(easy_bits_pool_t *p_ebp,unsigned int pos)
{
    if(pos >= p_ebp->bytes_len*8)
    {
        return -1;
    }
    int pos_0 = pos/8;
    int pos_1 = pos%8;
    p_ebp->da[pos_0] &= (~(1<<pos_1));
    return 0;
}
static inline int ebp_get_bit(easy_bits_pool_t *p_ebp,unsigned int pos)
{
    if(pos >= p_ebp->bytes_len*8)
    {
        return -1;
    }
    int pos_0 = pos/8;
    int pos_1 = pos%8;
    return (p_ebp->da[pos_0]>>pos_1)&0x1;
}
static inline unsigned int ebp_count_bits(easy_bits_pool_t *p_ebp,int bit)
{
    unsigned int alen = p_ebp->bytes_len;
    unsigned int counts = 0;
    unsigned int ii = 0;
    unsigned int jj = 0;
    for(ii=0;ii<alen;ii++)
    {
        for(jj=0;jj<8;jj++)
        {
            if((p_ebp->da[ii]>>jj)&1 == bit)
            {
                counts++;
            }
        }
    }
    return counts;
}

#endif
