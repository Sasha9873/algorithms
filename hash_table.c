#include <stdio.h>
#include <stdlib.h>
#include <string.h>
 
#define str_len 21
#define hash_size 100000
 
typedef struct node
{
	char key[str_len];
	char val[str_len];
} Node;

typedef struct list
{
	Node elem;
	struct list* next;
}List; 

typedef struct
{
	List** arr;
	size_t size;
} hash_table;
 

hash_table* create_hash_table(size_t size)
{
	hash_table* new_ht = calloc(1, sizeof(hash_table));
	if(!new_ht)
		return NULL;

	new_ht->arr = calloc(size, sizeof(struct list*));
	if(!new_ht->arr)
	{
		free(new_ht);
		return NULL;
	}
	
	new_ht->size = size;

	return new_ht;
}

List* list_find_before(List* head, char* key)
{
	if (!head || !key || strcmp(head->elem.key, key) == 0)
		return NULL;
	else
	{
		List* current = head;

		while(current->next && strcmp(current->next->elem.key, key))
			current = current->next;

		if(!current->next)
			return NULL;
		else
			return current;
	}
}

List* list_find(struct list* head, char* key)
{
	if(!key)
		return NULL;

	if (!head || strcmp(head->elem.key, key) == 0)
		return head;

	else
	{
		List* current = head->next;

		while(current && current->elem.key && strcmp(current->elem.key, key) != 0)
		{
			current = current->next;
		}

		if(!current)
			return NULL;
		else
			return current;
	}
}
 
int hash_func(char* key)
{
	if(!key)
		return 0;

	unsigned long long hash = 0;
	unsigned long long p = 31, p_pow = 1;

	int len = strlen(key);
	
	for(int i = 0; i < len; ++i)
	{
		hash += (key[i] - 'a' + 1) * p_pow;
		p_pow *= p;
	}

	return hash % hash_size;
}

List* create_list(char* key,char* value)
{
	if(!key || !value)
		return NULL;

	List* head = (List*)calloc(1, sizeof(List));
	if(!head)
		return NULL;

	head->next = NULL;

	strcpy(head->elem.key, key);
	strcpy(head->elem.val, value);

	return head;
}

List* list_insert(List* head, char* key, char* value)
{
	if(!key || !value)
		return NULL;

	if(!head)
	{
		head = create_list(key, value);

		return head;
	}
	else
	{
		List* cur = head;
		while(cur->next)
			cur = cur->next;

		cur->next = create_list(key, value);

		return head;
	}
}

List* list_erase(List* head, char* key)
{
	if(!key || !head)
		return NULL;

	if(!strcmp(head->elem.key, key))
	{
		List* delete = head;
		head = head->next;

		free(delete);
		delete = NULL;

		return head;
	}

	List* prev = list_find_before(head, key);

	if(!prev)
		return head;
	else
	{
		List* delete = prev->next;
		prev->next = delete->next;

		free(delete);
		delete = NULL;

		return head;
	}
}

List* list_delete(List* head)
{
	if(!head)
		return NULL;

	List* cur = head;

	while(cur)
	{
		List* delete = cur;
		cur = cur->next;
		free(delete);
		delete = NULL;
	}

	return NULL;
}

hash_table* hash_table_delete(hash_table* delete)
{
	if (!delete)
		return NULL;

	for (int i = 0; i < (int)delete->size; ++i)
		delete->arr[i] = list_delete(delete->arr[i]);

	free(delete->arr);
	delete->arr = NULL;

	free(delete);
	delete = NULL;

	return NULL;
}


int main()
{
	hash_table** table_arr = calloc(10, sizeof(hash_table*));
	if(!table_arr)
		return 1;

	for (int i = 0; i < 10; ++i)
		table_arr[i] = NULL;

	char op[20] = {0};

	while (scanf("%s", op) != EOF)
	{
		//printf("op = %s\n", op);
		if (strcmp(op, "create") == 0)
		{
			size_t num = 0;
			scanf("%zu", &num);

			table_arr[num] = create_hash_table(hash_size);
		}

		else if (strcmp(op, "insert") == 0)
		{
			size_t num = 0;
			char key[str_len] = {0}, value[str_len] = {0};

			scanf("%zu %21s %21s", &num, key, value);
			
			if (table_arr[num] != NULL)
			{
				int hash = hash_func(key);

				List* find = list_find(table_arr[num]->arr[hash], key);

				if (find == NULL)
					table_arr[num]->arr[hash] = list_insert(table_arr[num]->arr[hash], key, value);
				else
					strcpy(find->elem.val, value);
			}
		}

		else if (strcmp(op, "erase") == 0)
		{
			size_t num = 0;
			char key[str_len] = {0};
			scanf("%zu %21s", &num, key);

			if (table_arr[num] != NULL)
			{
				int hash = hash_func(key);

				table_arr[num]->arr[hash] = list_erase(table_arr[num]->arr[hash], key);
			}
		}

		else if (strcmp(op, "find") == 0)
		{
			size_t num = 0;
			char key[str_len] = {0};
			scanf("%zu %21s", &num, key);

			if (table_arr[num] == NULL)
				printf("MISSING\n");
			else
			{
				int hash = hash_func(key);
				List* find = list_find(table_arr[num]->arr[hash], key);

				if (find == NULL)
					printf("MISSING\n");
				else
					printf("%s\n", find->elem.val);
			}
		}

		else if (strcmp(op, "destroy") == 0)
		{
			size_t num = 0;
			scanf("%zu", &num);

			table_arr[num] = hash_table_delete(table_arr[num]);
		}
	}

	for (int i = 0; i < 10; i++)
		table_arr[i] = hash_table_delete(table_arr[i]);

	free(table_arr);

	return 0;
}