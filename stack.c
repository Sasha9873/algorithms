#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

struct stack
{
    size_t capacity; 
    size_t current_size;
    size_t elem_size;
    void* data;
};

struct stack* stack_new(size_t elem_size)
{ 
    struct stack* stk = (struct stack*)calloc(1, sizeof(struct stack));
    if(stk == NULL)
        return NULL;


    stk->data = (void*)calloc(100, elem_size);
    if(stk->data == NULL){
        return NULL;
    }

    stk->capacity = 100;
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
        st->data = realloc(st->data, st->elem_size * (st->capacity * 2));
        if(st->data == NULL)
            return 1;
        st->capacity *= 2;
    }

    char* arr = (char*)st->data;
    arr += st->elem_size * st->current_size; //arr = first empty elem in data

    char tmp;
    for(int i = 0; i < st->elem_size; ++i)
    {
        tmp = *((char*)elem + i);
       *((char*)(arr) + i) = tmp;
    }
    st->current_size++;

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
    for(int i = 1; i < st->current_size; ++i)
    {
        void const* pr = (void const*)(st->data + i*st->elem_size);
        printf(", ");
        pf(pr);
    }/**/
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

    if(3 * st->current_size <= st->capacity)
    {
        st->data = realloc(st->data, st->elem_size * (st->capacity / 2));
        if(st->data == NULL)
            return 1;
        st->capacity /= 2;
    }

    char* arr = (char*)st->data;
    arr += st->elem_size * (st->current_size - 1); //arr = last not empty elem in data

    char tmp;
    for(int i = 0; i < st->elem_size; ++i)
    {
        tmp = *((char*)(arr) + i);
        *((char*)elem + i) = tmp;
    }

    st->current_size--;

    return 0;
}

int stack_top (struct stack *st, void *elem)
{
    if(st == NULL || elem == NULL)
        return 1;

    if(st->data == NULL)
        return 1;

    if(st->current_size == 0)
        return 1;

    if(3 * st->current_size <= st->capacity)
    {
        st->data = realloc(st->data, st->elem_size * (st->capacity / 2));
        if(st->data == NULL)
            return 1;
        st->capacity /= 2;
    }

    char* arr = (char*)st->data;
    arr += st->elem_size * (st->current_size - 1); //arr = last not empty elem in data

    char tmp;
    for(int i = 0; i < st->elem_size; ++i)
    {
        tmp = *((char*)(arr) + i);
        *((char*)elem + i) = tmp;
    }

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

static void print_int(void const *st) {
   printf("%d", *(int *)st);
}

int main() {
    struct stack *st = stack_new(sizeof (int));
    for (int i = 0; i < 10; i++) {
        int tmp = i * i;
        stack_push(st, &tmp);
    }
    void* elem;
    stack_print(st, print_int);
    stack_pop(st, elem);
    stack_print(st, print_int);
    st = stack_delete(st);
    stack_print(st, print_int);

}