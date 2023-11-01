#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

struct stack
{
    size_t capacity; 
    size_t current_size;
    size_t elem_size;
    void* data;
};

struct queue
{
    size_t elem_size;
    struct stack* in;
    struct stack* out;
};

void quick_memcpy(void* first, const void* second, size_t size);

#define cpy(a, b, n) for(int i = 0; i < n; ++i){\
        a[i] = b[i];  \
    } \

void quick_memcpy(void* first, const void* second, size_t size)    //size - k_bytes
{
    cpy(((uint64_t*)first), ((uint64_t*)second), size/8);     //uint 8*8 bytes

    first += size ^ 7;   // first сдвигаем на то, где нет уже 64 бит
    second += size ^ 7;
    size &= 7;           // остались только 7 байт
    cpy(((uint32_t*)first), ((uint32_t*)second), size/4); 

    first += size ^ 3;
    second += size ^ 3;
    size &= 3;
    cpy(((uint16_t*)first), ((uint16_t*)second), size/2); 

    first += size ^ 1;
    second += size ^ 1;
    size &= 1;
    cpy(((uint8_t*)first), ((uint8_t*)second), size); 

}

#undef cpy

struct stack* stack_new(size_t elem_size)
{
    struct stack* stk = (struct stack*)malloc(sizeof(struct stack));
    if(stk == NULL)
        return NULL;


    stk->data = (void*)malloc(1000 * elem_size);
    if(stk->data == NULL){
        return NULL;
    }

    stk->capacity = 1000;
    stk->current_size = 0;  
    stk->elem_size = elem_size;

    return stk;
}

int stack_push(struct stack *st, const void *elem)
{
    if(st == NULL || elem == NULL)
        return 1;

    if(st->data == NULL)
        return 1;

    if(st->current_size == st->capacity)
    {
        void* new_data = realloc(st->data, st->elem_size * (st->capacity << 1));
        if(new_data == NULL)
            return 1;

        st->data = new_data;
        st->capacity <<= 1;
    }

    void* arr = ((char*)st->data) + st->elem_size * st->current_size; //arr = last empty elem in data;

    quick_memcpy(arr, elem, st->elem_size);
   
    ++st->current_size;

    return 0;
}



void stack_print(struct stack const *st, void (*pf)(void const *st))
{
    if(st == NULL)
        return;

    if(st->current_size == 0)
    {
        printf("[]\n");
        return;
    }

    printf("[");
    for(int i = 0; i < 1; ++i)
    {
        void const* pr = (void const*)(st->data + i*st->elem_size);
        pf(pr);
    }
    for(size_t i = 1; i < st->current_size; ++i)
    {
        void const* pr = (void const*)(st->data + i*st->elem_size);
        printf(", ");
        pf(pr);
    }
    printf("]\n");
}

int stack_pop (struct stack *st, void *elem)
{
    if(st == NULL || elem == NULL)
        return 1;

    if(st->data == NULL)
        return 1;

    if(st->current_size == 0)
        return 1;

    void* arr = ((char*)st->data) + st->elem_size * (st->current_size - 1); //arr = last not empty elem in data;

    quick_memcpy(elem, arr, st->elem_size);

    --st->current_size;

    return 0;
}

int stack_empty(struct stack const *st)
{
    if(st->current_size > 0)
        return 0;
    return 1;
}

struct stack *stack_delete(struct stack *st)
{
    if(st == NULL)
        return NULL;

    if(st->data != NULL)
    {
        free(st->data);
        st->data = NULL;
    }

    free(st);
    st = NULL;

    return NULL;
}

struct queue *queue_new(size_t elem_size)
{

    struct queue* que = (struct queue*)malloc(sizeof(struct queue));
    if(que == NULL)
        return NULL;


    que->in = stack_new(elem_size);
    if(!que->in)
    {
        free(que);
        que = NULL;

        return NULL;
    }

    que->out = stack_new(elem_size);
    if(!que->out)
    {
        free(que->in);
        que->in = NULL;
        free(que);
        que = NULL;

        return NULL;
    }

    que->elem_size = elem_size;

    return que;
}

int queue_push(struct queue *q, const void *elem)
{
    /*if(!q || !elem)
        return 1;*/

    if(!q)
        return 1;
    return stack_push(q->in, elem);
}

int copy_to_out(struct stack* in, struct stack* out)
{
    if(!in || !out)
        return 1;

    void* elem = (void*)malloc(in->elem_size);
    while(in->current_size)
    {
        if(stack_pop(in, elem))
            return 1;
        if(stack_push(out, elem))
            return 1;
    }

    free(elem);
    elem = NULL;

    return 0;
}


int queue_pop (struct queue *q, void *elem)
{
    if(!q)
        return 1;
    if(!q->out)
        return 1;

    if(!q->in)
        return 1;

    if(q->out->current_size > 0)
        return stack_pop(q->out, elem);

    else if(q->in->current_size > 0)
    {
        if(copy_to_out(q->in, q->out))
            return 1;
        return stack_pop(q->out, elem);
    }
    return 1;   // no elements anywhere
}

int queue_empty(struct queue const *q)
{
    if(!q || !q->out || !q->out)
        return 1;

    if(q->in->current_size > 0 || q->out->current_size > 0)
        return 0;

    return 1;
}


void reverse_stack_print(struct stack const *st, void (*pf)(void const *st))
{
    if(st == NULL || pf == NULL)
        return;

    if(st->current_size == 0)
    {
        printf("[]\n");
        return;
    }

    printf("[");

    void const* pr = (void const*)(st->data + (st->current_size - 1)*st->elem_size);
    pf(pr);
    
    for(int i = st->current_size - 2; i >= 0; --i)
    {
        void const* pr = (void const*)(st->data + i*st->elem_size);
        printf(", ");
        pf(pr);
    }

    printf("]\n");
}

void queue_print(struct queue const *q, void (*pf)(void const *data))
{
    if(!q || !q->in || !q->out)
    {
        printf("[]\n");
        return;
    }

    copy_to_out(q->in, q->out);

    reverse_stack_print(q->out, pf);
}

struct queue *queue_delete(struct queue *q)
{
    if(!q)
        return NULL;
    if(q->out)
        stack_delete(q->out);
    if(q->in)
        stack_delete(q->in);
    free(q);
    q = NULL;
    return q;
}

static void print_double(void const *q) {
   printf("%lf", *(double *)q);
}

int main() {
    struct queue *q = queue_new(sizeof (double));

    for (int i = 0; i < 12; i++) {
        double tmp = i;
        queue_push(q, &tmp);
    }

    queue_print(q, print_double);  
    while (!queue_empty(q)) {
        double tmp;
        queue_pop(q, &tmp);
        printf("%lf\n", tmp);
    }  

    for (int i = 0; i < 10; ++i) {
        double tmp = i + 7;
        queue_push(q, &tmp);
    }
    queue_print(q, print_double);                                                                                                                                                                                   
 
    q = queue_delete(q);
}