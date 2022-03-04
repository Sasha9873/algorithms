#include "heap.h"

struct heap* make_heap(size_t capacity)
{
	if(capacity == 0)
		return NULL;

	struct heap* heap_ = (struct heap*)malloc(sizeof(struct heap));
	if(heap_ == NULL)
		return NULL;

	struct TreeNode* data = (struct TreeNode*)malloc(sizeof(struct TreeNode) * capacity);
	if(data == NULL){
		free(heap_);
		return NULL;
	}

	heap_->size = 0;
	heap_->capacity = capacity;
	heap_->data = data;

	return heap_;
}

bool get_min(struct heap* hp, struct TreeNode* min)
{
	if(hp->size == 0)
		return false;

	memcpy(min, &hp->data[0], sizeof(struct TreeNode));
	return true;
}

void swap(void* first, void* second, size_t size)
{
	uint8_t* f = (uint8_t*)first;
	uint8_t* s = (uint8_t*)second;

	for(size_t i = 0; i < size; ++i)
	{
		f[i] ^= s[i];
		s[i] ^= f[i];
		f[i] ^= s[i];
	}
}

void shift_up(struct heap* hp, size_t i)
{
	if(i == 0)
		return;

	size_t parent = (i - 1) >> 1;
 
	if(hp->data[parent].frequency > hp->data[i].frequency)  //min наверху
	{
		swap(&hp->data[parent], &hp->data[i], sizeof(struct TreeNode));
		shift_up(hp, parent);
	}
}

bool insert(struct heap* hp, struct TreeNode* node)
{
	if(hp->size == hp->capacity && !resize(hp, 2 * hp->capacity))
		return false;

	memcpy(&hp->data[hp->size++], node, sizeof(struct TreeNode));
	shift_up(hp, hp->size - 1);

	return true;
}

void shift_down(struct heap* hp, size_t i)
{
	size_t left_i = 2 * i + 1;
	size_t right_i = 2 * i + 2;

	size_t min_i = i;

	if(right_i < hp->size && hp->data[left_i].frequency < hp->data[min_i].frequency)
		min_i = left_i;

	if(left_i < hp->size && hp->data[right_i].frequency < hp->data[min_i].frequency)
		min_i = right_i;

	if(min_i != i)
	{
		swap(&hp->data[min_i], &hp->data[i], sizeof(struct TreeNode));
		shift_down(hp, min_i);
	}

}

bool extract_min(struct heap* hp, struct TreeNode* node)
{
	if(get_min(hp, node))
	{
		memcpy(&hp->data[0], &hp->data[hp->size - 1], sizeof(struct TreeNode));
		--hp->size;
		shift_down(hp, 0);
		return true;
	}

	return false;
}

bool resize(struct heap* hp, size_t capacity)
{
	struct TreeNode* new_data = realloc(hp->data, capacity * sizeof(struct TreeNode));

	if(new_data == NULL)
		return false;

	hp->data = new_data;
	hp->capacity = capacity;

	return true;
}

struct heap* remove_heap(struct heap* hp)
{
	free(hp->data);
	hp->data = NULL;
	free(hp);
	hp = NULL;

	return NULL;
}