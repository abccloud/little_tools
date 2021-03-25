
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>
#include <assert.h>
#include "easy_bits_ops.h"
#include "array_queue.h"
#include "arraylist.h"


int test_array_list(void)
{
    int rt = 0;
    int ii = 0;
    array_list_t al;
    srand(time(NULL));
    rt = init_array_list(&al);
    assert(0==rt);
    show_array_list(&al);
    for(ii=10;ii<10+MAX_ID_NUM;ii++)
    {
        rt = array_list_addid(&al,ii,get_pri(ii));
        assert(0==rt);
    }
    show_array_list(&al);
    for(ii=10;ii<10+MAX_ID_NUM;ii++)
    {
        rt = array_list_delid(&al,ii);
        assert(0==rt);
    }
    show_array_list(&al);
    for(ii=10;ii<10+MAX_ID_NUM;ii++)
    {
        rt = array_list_delid(&al,ii);
        assert(-1==rt);
    }
    show_array_list(&al);
    for(ii=0;ii<MAX_ID_NUM;ii++)
    {
        int xx = rand()%100;
        if(array_list_findid(&al,xx)==-1)
        {
            rt = array_list_addid(&al,xx,get_pri(xx));
            assert(0==rt);
        }
        else
        {
            rt = array_list_delid(&al,xx);
            assert(0==rt);
        }
    }
    show_array_list(&al);
    return 0;
    
}
int test_easy_bits(viod)
{
    /*get a ebp, you can operation bit 0-99 don't touch more than 99*/
    easy_bits_pool_t *p_ebp = get_a_init_ebp(100);
    ebp_set_bit(p_ebp,0);
    ebp_set_bit(p_ebp,23);
    ebp_set_bit(p_ebp,35);
    ebp_set_bit(p_ebp,66);
    ebp_set_bit(p_ebp,99);/*max bit*/
    ebp_clear_bit(p_ebp,35);
    printf("%d\n",ebp_count_bits(p_ebp,1));
    printf("%d\n",ebp_get_bit(p_ebp,0));
    printf("%d\n",ebp_get_bit(p_ebp,23));
    printf("%d\n",ebp_get_bit(p_ebp,35));
    printf("%d\n",ebp_get_bit(p_ebp,66));
    printf("%d\n",ebp_get_bit(p_ebp,99));
    ebp_free(p_ebp);

    int base[64];
    /*init ebp from the base mem, you can operation bit 0-bits_len,don't touch more than bits_len*/
    unsigned int bits_len = init_ebp(base,sizeof(base));
    p_ebp = (easy_bits_pool_t *)base;
    ebp_set_bit(p_ebp,0);
    ebp_set_bit(p_ebp,bits_len/2);
    ebp_set_bit(p_ebp,bits_len-1);/*max bit*/
    ebp_clear_bit(p_ebp,bits_len/2);
    printf("+%d\n",ebp_count_bits(p_ebp,1));
    printf("+%d\n",ebp_get_bit(p_ebp,0));
    printf("+%d\n",ebp_get_bit(p_ebp,bits_len/2));
    printf("+%d\n",ebp_get_bit(p_ebp,bits_len-1));

    return 0;
}
int test_array_queue(void)
{
    /*get a array queue, you max store 100 number*/
    array_queue_t *aq = get_a_init_array_queue(100);
    aq_put(aq,10);
    aq_put(aq,200);
    unsigned int out;
    int ret = aq_get(aq,&out);
    printf("array_queue:out(%d) ret(%d)\n",out,ret);
    aq_get(aq,&out);
    printf("array_queue:out(%d) ret(%d)\n",out,ret);
    aq_free(aq);
    
    int base[64];
     /*init a array queue, you max store maxq numbers*/
    int maxq = init_array_queue(base,sizeof(base));
    printf("array_queue:max(%d)\n",maxq);
    aq = (array_queue_t *)base;
    aq_put(aq,100);
    aq_put(aq,3000);
    ret = aq_get(aq,&out);
    printf("array_queue:out(%d) ret(%d)\n",out,ret);
    aq_get(aq,&out);
    printf("array_queue:out(%d) ret(%d)\n",out,ret);
    int ii = 0;
    for(ii=0;ii<maxq+1;ii++)
    {
        ret = aq_put(aq,ii);
        if(ret<0)
        {
            printf("ii(%d) put full\n",ii);
        }
    }
    ii = 0;
    while(aq_get(aq,&out)==0)
    {
        ii++;
    }
    printf("aq empty now circle (%d)\n",ii);
    return 0;
}
int main(int argc,char *argv[])
{
    test_easy_bits();
    test_array_queue();
    test_array_list();
    return 0;
}
