#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct list{
    int elem;
    struct list* next;
    struct list* prev;
}List;


struct list *list_new(int elem)
{
    struct list* lst = (struct list*)malloc(sizeof(struct list));
    if(lst == NULL)
        return NULL;

    lst->elem = elem;
    lst->next = NULL;
    lst->prev = NULL;

    return lst;
}

struct list *list_insert(struct list *head, int elem)
{
    if(head == NULL)
    {
        return list_new(elem);
    }

    List* cur = head;
    while(cur->next)
        cur = cur->next;

    cur->next = (List*)malloc(sizeof(List));
    if(cur->next == NULL)
        return NULL;

    cur->next->elem = elem;
    cur->next->prev = cur;
    cur->next->next = NULL;

    return head;
}


struct list *list_find(struct list *head, int elem)
{
    List* cur = head;

    while(cur)
    {
        if(cur->elem == elem)
            return cur;
        cur = cur->next;
    }
    return NULL;
}

struct list *list_erase(struct list *head, int elem)
{
    List* cur = head;

    while(cur)
    {
        if(cur->elem == elem)
        {
            if(cur == head)
            {
                if(cur->next)
                {
                    cur = cur->next;
                    free(cur->prev);
                    cur->prev = NULL;
                    //printf("cur->elem = %d\n", cur->elem);
                    return cur;
                }
                free(cur);
                cur = NULL;
                return NULL;
            }
            if(cur->next)
                cur->next->prev = cur->prev;
            if(cur->prev)
                cur->prev->next = cur->next;

            free(cur);
            cur = NULL;

            return head;
        }
        cur = cur->next;
    }

    return head;
}

struct list *list_insert_after(struct list *head, struct list *where, struct list *what)
{
    if(head == NULL)
    {
        return list_new(what->elem);
    }

    if(where == NULL || what == NULL)
        return head;

   /*List* what_next = what->next;
    List* what_prev = what->prev;*/

    what->next = where->next;
    what->prev = where;
    if(where->next)
        where->next->prev = what;
    where->next = what;

    /*what_prev = what_next;
    if(what_next)
        what_next->prev = what_prev;

    if(what == head)
        return what_next;*/
    return head;
}

void list_print(struct list const *head)
{
    if(head == NULL)
    {
        printf("[]\n");
        return;
    }


    printf("[");
    for(int i = 0; i < 1; ++i)
    {
        printf("%d", head->elem);
    }

    List* cur = head->next;

    while(cur)
    {
        printf(", %d", cur->elem);
        cur = cur->next;
    }
    printf("]\n");
}

struct list *list_insert_before(struct list *head, struct list *where, struct list *what)
{
    if(head == NULL)
    {
        return list_new(what->elem);
    }

    if(where == NULL || what == NULL)
        return head;

    
    what->next = where;
    what->prev = where->prev;
    if(where->prev)
        where->prev->next = what;
    where->prev = what;

    if(head == where)
        return what;

    return head;
}

struct list *list_next(struct list *curr)
{
    if(curr)
        return curr->next;
    return NULL;
}

struct list *list_delete(struct list *head)
{
    if(!head)
        return NULL;

    List* cur = head;

    while(cur)
    {
        if(cur->next == NULL)
        {
            free(cur);
            break;
        }
        cur = cur->next;
        free(cur->prev);
    }

    return NULL;
}

int main() {
    struct list *head = list_new(0);
    head = list_insert(head, 1);
    List* add = (List*)malloc(sizeof(List));
    add->elem = 6;
    head = list_insert_after(head, head, add);
    head = list_erase(head, 0);
    list_print(head);
    head = list_erase(head, 6);
    List* add2 = (List*)malloc(sizeof(List));
    add2->elem = 9;
    head = list_insert_after(head, head, add2);
    list_print(head);
    List* add3 = (List*)malloc(sizeof(List));
    add3->elem = 11;
    printf("%p %d %p\n", list_find(head, 9), list_find(head, 9)->elem, add3);
    head = list_insert_before(head, list_find(head, 9), add3);

    list_print(head);

    List* add4 = (List*)malloc(sizeof(List));
    add4->elem = 2;
    head = list_insert_after(head, list_find(head, 1), add4);

    /*for(int i = 2; i < 90; ++i){
        add->elem = i;
        head = list_insert_after(head, list_find(head, i - 1), add);
    }*/

    list_print(head);
    head = list_delete(head);
    list_print(head);


    /*free(add);
    free(add2);
    free(add3);
    free(add4);*/
}