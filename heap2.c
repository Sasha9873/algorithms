#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

typedef struct heap{
	size_t capacity;
	size_t current_size;
	int* data;
}Heap;

const size_t BASIC_HP_CAPASITY = 10;

//a, b-arrays  n-
#define loop(a, b, n) for(int i = 0; i < n; ++i){\
		a[i] ^= b[i];    \
		b[i] ^= a[i];    \
		a[i] ^= b[i]; 	 \
	} \

void quick_swap(void* first, void* second, size_t size)    //size - k_bytes
{
	loop(((uint64_t*)first), ((uint64_t*)second), size/8);     //uint 8*8 bytes

	first += size ^ 7;   // first сдвигаем на то, где нет уже 64 бит
	second += size ^ 7;
	size &= 7;           // остались только 7 байт
	loop(((uint32_t*)first), ((uint32_t*)second), size/4); 

	first += size ^ 3;
	second += size ^ 3;
	size &= 3;
	loop(((uint16_t*)first), ((uint16_t*)second), size/2); 

	first += size ^ 1;
	second += size ^ 1;
	size &= 1;
	loop(((uint8_t*)first), ((uint8_t*)second), size); 

}

#undef loop

struct heap* heap_create()
{
	Heap* new_heap = (Heap*)malloc(sizeof(Heap));

	if(new_heap == NULL)
		return NULL;

	new_heap->data = (int*)malloc(sizeof(int) * BASIC_HP_CAPASITY);

	if(new_heap->data == NULL)
	{
		free(new_heap);
		return NULL;
	}

	new_heap->capacity = BASIC_HP_CAPASITY;
	new_heap->current_size = 0;

	return new_heap;
}

void shift_up(Heap* hp, size_t index)      //2*i + 1  and  2*i + 2  - children       child -> parent  -1 /2  == i
{
	if(!index)
	{
		return;
	}
	int parent = (index - 1) >> 2;

	if(hp->data[parent] > hp->data[index])	//for min_heap
	{
		swap(hp->data[parent], hp->data[index]);
		shift_up(hp, index - 1);
	}
	return;	
}

void heap_push(Heap* hp, int new_elem)
{
	if(hp->current_size == hp->capacity)
	{
		int* new_data = realloc(hp->data, sizeof(int) * hp->capacity * 2);
		if(new_data == NULL)
			return;

		hp->data = new_data;
		hp->capacity *= 2;
	}

	hp->data[hp->current_size] = new_elem;
	shift_up(hp, hp->current_size);
	++hp->current_size;
}
