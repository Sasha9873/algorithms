#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct list{
    void* elem;
    struct list* next;
    struct list* prev;
}List;

struct queue
{
    size_t elem_size;
    struct list* head;
    struct list* tail;
};

struct queue *queue_new(size_t elem_size)
{
    struct queue* que = (struct queue*)malloc(sizeof(struct queue));
    if(que == NULL)
        return NULL;

    que->tail = NULL;
    que->head = NULL;

    que->elem_size = elem_size;

    return que;
}

int queue_push(struct queue *q, const void *elem)
{
    if(q == NULL)
        return 1;
    if(elem == NULL)
        return 1;

    if(q->head == NULL)
    {
        q->head = (List*)malloc(sizeof(List));
        if(q->head == NULL)
            return 1;

        q->tail = q->head;
        q->head->next = NULL;
        q->head->prev = NULL;
    }
    else
    {
        List* new = (List*)malloc(sizeof(List));
        if(new == NULL)
            return 1;

        new->next = q->head;
        q->head->prev = new;

        q->head = new;
    }

    q->head->elem = malloc(q->elem_size);
    if(q->head->elem == NULL)
        return 1;

    char tmp;
    for(int i = 0; i < q->elem_size; ++i)
    {
        tmp = *((char*)elem + i);
       *((char*)(q->head->elem) + i) = tmp;
    }

    return 0;
}

int queue_pop (struct queue *q, void *elem)
{
    if(q == NULL)
        return 1;
    if(elem == NULL)
        return 1;
    if(q->tail == NULL)
        return 1;

    for(int i = 0; i < q->elem_size; ++i)
        *((char*)elem  + i)= *((char*)q->tail->elem + i);

    free(q->tail->elem);
    q->tail->elem = NULL;

    if(q->head == q->tail)   // if in queue was only one elem before pop
    {
        free(q->tail);
        q->head = NULL;
        q->tail = NULL;
        return 0;
    }   
    

    q->tail = q->tail->prev;

    free(q->tail->next);
    q->tail->next = NULL;
    
    return 0;
}

int queue_empty(struct queue const *q)
{
    if(!q)
        return 1;
    if(q->head == NULL)
        return 1;
    return 0;
}

struct queue *queue_delete(struct queue *q)
{
    if(!q)
        return NULL;

    List* cur = q->head;

    while(cur)
    {
        free(cur->elem);
        //cur->elem = NULL;
        if(cur->next == NULL)
        {
            free(cur);
            break;
        }
        cur = cur->next;
        free(cur->prev);
    }

    free(q);
    q = NULL;
    return NULL;
}

static void print_double(void const *q) {
   printf("%lf", *(double *)q);
}

void queue_print(struct queue const *q, void (*pf)(void const *data))
{
    if(q == NULL)
        return;

    if(pf == NULL)
        return;

    if(q->head == NULL)
    {
        printf("[]\n");
        return;
    }

    printf("[");
    for(int i = 0; i < 1; ++i)
    {
        void const* pr = (void const*)(q->head->elem);
        pf(pr);
    }
    List* cur = q->head;
    while(cur->next != NULL)
    {
        void const* pr = (void const*)(cur->next->elem);
        printf(", ");
        pf(pr);
        cur = cur->next;
    }
    printf("]\n");
}

int main() {
    struct queue *q = queue_new(sizeof (double));
    for (int i = 0; i < 10; i++) {
        double tmp = i * i;
        queue_push(q, &tmp);
    }
    printf("head = %lf tail = %lf\n", *(double*)q->head->elem, *(double*)q->tail->elem);
    queue_print(q, print_double);
    while (!queue_empty(q)) {
       double tmp;
        queue_pop(q, &tmp);
        printf("%lf\n", tmp);
    }    
    for (int i = 0; i < 10; i++) {
        double tmp = i;
        queue_push(q, &tmp);
    }
    printf("head = %lf tail = %lf\n", *(double*)q->head->elem, *(double*)q->tail->elem);
    queue_print(q, print_double);                                                                                                                                                                                    
 
    q = queue_delete(q);/* */
}