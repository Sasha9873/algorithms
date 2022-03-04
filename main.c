#include "heap.h"
#include <stdio.h>

int main()
{
	struct heap* hp = make_heap(1000);
	struct TreeNode st;
	st.frequency = 56;

	insert(hp, &st);
	struct TreeNode result;
	get_min(hp, &result);
	printf("%ld\n", result.frequency);
	hp = remove_heap(hp);

	return 0;
}


void check_min()
{
	struct heap* hp = make_heap(1000);
	for(int i = 0; i < 100; ++i)
	{
		struct TreeNode st;
		st.frequency = 100 - i;
		insert(hp, st);
	}

	struct TreeNode result;
	extract_min(hp, &result);
	assert(result.frequency == 0);
}

void make_remove_heap_test()
{

}