#pragma once
#include "heap.h"

void calculate_frequencies(uint8_t* bytes, size_t size, size_t* frequencies)
{
	for(size_t i = 0; i < size; ++i)
	{
		frequencies[bytes[i]]++;
	}
}

struct TreeNode* build_tree(size_t* frequencies, size_t f_size)
{
	struct heap* hp = make_heap(256);
	for(size_t i = 0; i < f_size; ++i)
	{
		if(frequencies[i] != 0)
		{
			struct TreeNode st;
			st.frequency = frequencies[i];
			st.byte = i;
			st.left = NULL;
			st.right = NULL;
			insert(hp, &st);
		}
	}

	while(get_size(hp) != 1)
	{
		struct TreeNode* node1 = (struct TreeNode*)malloc(sizeof(struct TreeNode));
		struct TreeNode* node2 = (struct TreeNode*)malloc(sizeof(struct TreeNode));
		extract_min(hp, node1);
		extract_min(hp, node2);
		struct TreeNode st;
		st.frequency = node1->frequency + node2->frequency;
		st.left = node1;
		st.right = node2;
		insert(hp, &st);
	}
	struct TreeNode* root = (struct TreeNode*)malloc(sizeof(struct TreeNode));
	extract_min(hp, root); //root - haffman tree
	hp = remove_heap(hp);

	return root;
}

int8_t read_bytes(FILE* f, size_t* size_f)
{
	int ch = 0;
	size_t capacity = 64;
	size_t size = 0;
	int8_t* data = (int8_t*)malloc(sizeof(int8_t) * capacity);

	while((ch = getc(f)) != EOF)
	{
		char c = ch;
		void* t = (void*)&c;
		int8_t* norm = (int8_t*)t;

		if(size == capacity)
		{
			data = realloc(data, capacity << 1);
			capacity <<= 1;
		}

		data[size++] = *norm;
	}
	
	*size_f = size;

	return data;
}
