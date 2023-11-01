#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct vector {
    size_t current_size;
    size_t capacity;
    size_t elem_size;
    void* data;
}Vector;

struct vector* vector_new(size_t elems, size_t elem_size)
{
    Vector* new_vector = (Vector*)malloc(sizeof(Vector));
    if(!new_vector)
        return NULL;
    new_vector->elem_size = elem_size;

    new_vector->data = malloc(elem_size * (elems << 1));
    if(!new_vector->data)
    {
        free(new_vector);
        return NULL;
    }
    new_vector->capacity = elems << 1;
    new_vector->current_size = elems;

    return new_vector;
}

int vector_push(struct vector *v, void const *elem)
{
    if(!v || !elem)
        return 1;

    if(!v->data)
        return 1;

    if(v->current_size == v->capacity)
    {
        if(v->capacity == 0)
            v->capacity = 10;
        
        void* new_data = realloc(v->data, v->elem_size * (v->capacity << 1));
        if(new_data == NULL)
            return 1;

        v->data = new_data;
        v->capacity <<= 1;
    }

    void* arr = ((char*)v->data) + v->elem_size * v->current_size; //arr = last empty elem in data;

    void* dest = memcpy(arr, elem, v->elem_size);
    if(arr != dest)
        return 1;

    ++v->current_size;

    return 0;
}

int vector_pop (struct vector *v, void *elem)
{
    if(v == NULL || elem == NULL)
        return 1;

    if(v->data == NULL)
        return 1;

    if(v->current_size == 0)
        return 1;

    void* arr = ((char*)v->data) + v->elem_size * (v->current_size - 1); //arr = last not empty elem in data;

    void* dest = memcpy(elem, arr, v->elem_size);
    if(dest != elem)
        return 1;

    --v->current_size;

    return 0;
}

int vector_empty(struct vector const *st)
{
    if(!st)
        return 1;
    if(st->current_size == 0)
        return 1;
    return 0;
}

struct vector *vector_delete(struct vector *v)
{
    if(!v)
        return NULL;
    if(v->data)
    {
        free(v->data);
        v->data = NULL;
        free(v);
        v = NULL;
        return NULL;
    }
    free(v);
    v = NULL;
    return NULL;
}

void vector_print(struct vector const *v, void (*pf)(void const *data))
{
    if(v == NULL || pf == NULL)
        return;

    if(v->current_size == 0)
    {
        printf("[]\n");
        return;
    }

    printf("[");
    
    void const* pr = (void const*)(v->data);
    pf(pr);
    
    for(int i = 1; i < v->current_size; ++i)
    {
        void const* pr = (void const*)(v->data + i * v->elem_size);
        printf(", ");
        pf(pr);
    }
    printf("]\n");
}

int vector_set(struct vector *v, size_t index, void const *elem)
{
    if(!v || !elem || index >= v->current_size)
        return 1;

    void* arr = (char*)v->data + index * v->elem_size;
    void* dest = memcpy(arr, elem, v->elem_size);

    if(dest != arr)
        return 1;

    return 0;
}

int vector_get(struct vector const *v, size_t index, void *elem)
{
    if(!v || !elem || index >= v->current_size)
        return 1;

    void* arr = (char*)v->data + index * v->elem_size;
    void* dest = memcpy(elem, arr, v->elem_size);

    if(dest != elem)
        return 1;

    return 0;
}

int vector_resize(struct vector *v, size_t new_size)
{
    if(!v)
        return 1;

    if (new_size > v->capacity)
    {
        while (new_size > v->capacity)
        {
            if (v->capacity == 0)
                v->capacity = 10;
            else 
                v->capacity <<= 1;
        }
        v->data = realloc(v->data, v->capacity * v->elem_size);
    }
 
    v->current_size = new_size;

    return 0;
}

size_t vector_size(struct vector const *v)
{
    if(!v)
        return 0;
    return v->current_size;
}

static void print_int(void const *data) {
   printf("%d", *(int *)data);
}

int main() {
    struct vector *v = vector_new(10, sizeof (int));
    for (int i = 0; i < 10; i++) {
        vector_set(v, i, &i);
    }
    int elem = 10;
    vector_push(v, &elem);
    vector_print(v, print_int);
    for (int i = 110000; i < 110200; i++) {
        int elem = i;
        vector_push(v, &elem);
        vector_pop(v, &elem);
        printf("%d\n", elem);
    }
    vector_print(v, print_int);
    
    v = vector_delete(v);
}