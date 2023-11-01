#include <stdio.h>
#include <stdlib.h>

typedef struct
{
	int left;
	int right;
	long sum;
} node ;

typedef struct
{
	int n;
	node* data;
} tree ;

tree* create_seq_tree(int n)
{
	tree* new_tree = (tree*)calloc(1, sizeof(tree));
	if(!new_tree)
		return NULL;

	new_tree->data = (node*)calloc(4*n, sizeof(node));
	if(!new_tree->data)
	{
		free(new_tree);
		return NULL;
	}

	new_tree->n = n;

	return new_tree;
}

void build(int* arr, tree* new_tree, int left, int right, int pos)
{	
	new_tree->data[pos].left = left;
	new_tree->data[pos].right = right;

	if(left == right)
	{
		new_tree->data[pos].sum = arr[left - 1];
		printf("sum = %ld left = %d right = %d\n", new_tree->data[pos].sum, new_tree->data[pos].left, new_tree->data[pos].right);

		return;
	}

	int mid = (left + right) >> 1;

	build(arr, new_tree, left, mid, pos << 1);
	build(arr, new_tree, mid + 1, right, (pos << 1) + 1);

	new_tree->data[pos].sum = new_tree->data[pos << 1].sum + new_tree->data[(pos << 1) + 1].sum;
	
	printf("sum = %ld left = %d right = %d\n", new_tree->data[pos].sum, new_tree->data[pos].left, new_tree->data[pos].right);
}

void modify(node* data, int pos, int x, int need_pos)
{
	if(data[pos].left == data[pos].right)
	{
		data[pos].sum = x;
		return;
	}

	int mid = ((data[pos].left + data[pos].right) >> 1);

	if(need_pos <= mid)
		modify(data, pos << 1, x, need_pos);
	else
		modify(data, (pos << 1) + 1, x, need_pos);

	data[pos].sum = data[pos << 1].sum + data[(pos << 1) + 1].sum;
}

long find_sum(node* data, int pos, int q_left, int q_right)
{
	if(data[pos].right < q_left || data[pos].left > q_right)
		return 0;

	if(data[pos].left >= q_left && data[pos].right <= q_right)
	{
		printf("find sum %ld l = %d, r = %d\n", data[pos].sum, data[pos].left, data[pos].right);
		return data[pos].sum;
	}

	long sum_left = find_sum(data, pos << 1, q_left, q_right);
	long sum_right = find_sum(data, (pos << 1) + 1, q_left, q_right);

	return sum_right + sum_left;
}

int main()
{
	int n = 0, q = 0;
	scanf("%d %d", &n, &q);

	int* arr = (int*)calloc(n, sizeof(int));
	if(!arr)
		return 1;

	for(int i = 0; i < n; ++i)
		scanf("%d", &arr[i]);
	
	tree* new_tree = create_seq_tree(n);
	if(!new_tree)
		return 2;

	build(arr, new_tree, 1, n, 1);

	int num = 0, q_left = 0, q_right = 0, need_pos = 0, x = 0;

	for(int i = 0; i < q; ++i)
	{
		scanf("%d", &num);
		if(num == 1)
		{
			scanf("%d %d", &q_left, &q_right);
			printf("%ld\n", find_sum(new_tree->data, 1, q_left, q_right));
		}
		else if(num == 2)
		{
			scanf("%d %d", &need_pos, &x);
			modify(new_tree->data, 1, x, need_pos);
		}
	}

	free(arr);

	return 0;
}