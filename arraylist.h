#ifndef __ARRAY_LIST_H__
#define __ARRAY_LIST_H__
#define MAX_ID_NUM   32
typedef struct _array_list_node_
{
    int id;
    int n_index;
}array_list_node_t;

typedef struct _array_list_
{
    int empty_lh;
    int used_lh;
    array_list_node_t iddata[MAX_ID_NUM];
}array_list_t;

static inline int get_pri(int id)
{
    return id;
}
static inline int init_array_list(array_list_t *head)
{
    head->used_lh = -1;
    head->empty_lh = 0;
    int ii = 0;
    for(ii=0;ii<MAX_ID_NUM;ii++)
    {
        head->iddata[ii].id = 0;
        head->iddata[ii].n_index = ii+1;
    }
    head->iddata[MAX_ID_NUM-1].n_index = -1;
    return 0;
}
static inline int array_list_addid(array_list_t *head,int id,int pri)
{
    int ii = -1;
    int ii_pre = -1;
    int index = head->empty_lh;
    if((index < 0)||(index >= MAX_ID_NUM))
    {
        return -1;
    }
    head->empty_lh = head->iddata[index].n_index;/*get this node from empty_lh*/
    
    head->iddata[index].id = id;
    head->iddata[index].n_index = -1;/*store id in this node*/

    ii = head->used_lh;
    if((ii>=0)&&(ii<MAX_ID_NUM))
    {
        if(pri < get_pri(head->iddata[ii].id))
        {
            /*insert head*/
            head->iddata[index].n_index = head->used_lh;
            head->used_lh = index;
            return 0;
        }
    }
    ii_pre = ii;/*the first one we have compared*/
    ii = head->iddata[ii_pre].n_index;
    while((ii>=0)&&(ii<MAX_ID_NUM))
    {
        if(pri < get_pri(head->iddata[ii].id))
        {
            /*we should insert after ii_pre*/
            break;
        }
        ii_pre = ii;
        ii = head->iddata[ii].n_index;
    }
    /*insert after ii_pre*/
    head->iddata[index].n_index = head->iddata[ii_pre].n_index;
    head->iddata[ii_pre].n_index = index;
    return 0;
}
static inline int array_list_delid(array_list_t *head,int id)
{
    int ii = -1;
    int ii_pre = -1;

    ii = head->used_lh;
    if((ii>=0)&&(ii<MAX_ID_NUM))
    {
        if(id == head->iddata[ii].id)
        {
            /*delete head*/
            head->used_lh = head->iddata[ii].n_index;
            /*insert to empty list head*/
            head->iddata[ii].n_index = head->empty_lh;
            head->empty_lh = ii;
            return 0;
        }
    }
    ii_pre = ii;/*the first one we have compared*/
    ii = head->iddata[ii_pre].n_index;
    while((ii>=0)&&(ii<MAX_ID_NUM))
    {
        if(id == get_pri(head->iddata[ii].id))
        {
            /*we should del this one*/
            head->iddata[ii_pre].n_index = head->iddata[ii].n_index;
            /*insert to empty list head*/
            head->iddata[ii].n_index = head->empty_lh;
            head->empty_lh = ii;
            return 0;
        }
        ii_pre = ii;
        ii = head->iddata[ii].n_index;
    }
    return -1;/*no this node*/
}

static inline int array_list_findid(array_list_t *head,int id)
{
    int ii = -1;
    int ii_pre = -1;

    ii = head->used_lh;
    if((ii>=0)&&(ii<MAX_ID_NUM))
    {
        if(id == head->iddata[ii].id)
        {
            return ii;
        }
    }
    ii_pre = ii;/*the first one we have compared*/
    ii = head->iddata[ii_pre].n_index;
    while((ii>=0)&&(ii<MAX_ID_NUM))
    {
        if(id == get_pri(head->iddata[ii].id))
        {
            return ii;
        }
        ii_pre = ii;
        ii = head->iddata[ii].n_index;
    }
    return -1;/*no this node*/
}
static inline int show_array_list(array_list_t *head)
{
    printf("\n************************\n");
    printf("empty_list:\n");
    int ii = 0;
    for(ii=head->empty_lh;ii>=0;ii=head->iddata[ii].n_index)
    {
        printf("index:%d ",ii);
    }
    printf("\n");
    printf("used_list:\n");
    for(ii=head->used_lh;ii>=0;ii=head->iddata[ii].n_index)
    {
        printf("index:%d id:%d ",ii,head->iddata[ii].id);
    }
    printf("\n");
    printf("\n#########################\n");
    return 0;
}
#endif