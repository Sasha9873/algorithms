#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const int UP_CAP_COEFF = 0.8;
const int HASH1_COEFF = 2;
const int RESIZE_COEFF = 2;

typedef struct
{
	char* key;  //ключ: Ваня
	int val; // значение: номер телефона
 	bool is_del = 0;

}backet;

typedef struct
{
	backet* data;
	size_t size;
	size_t capacity;  //size of table
	size_t size_del = 0;

}hash_table;

size_t hash1(char* key, size_t capacity)
{
	size_t len = strlen(key);
	for(int i = 0; i < len; ++i)
	{
		res = ((res + key[i]) * HASH1_COEFF) % capacity;
	}
	return res;
}
size_t hash2(char* key)
{
	return strlen(key) | 1;  //odd value
}


hash_table* ht_create(size_t capacity)
{
	hash_table* ht = (hash_table*)malloc(sizeof(hash_table));
	//check
	backet* data = (backet*)calloc(capacity, sizeof(backet));
	//check
	ht->data = data;
	ht->capacity = capacity;
	ht->size = 0;

	return ht;
}

int ht_insert(hash_table* ht, char* key, int val)
{
	if(!ht || !val)
		return 1;

	if(ht->size >= ht->capacity * UP_CAP_COEFF)
		rehash(ht);

	size_t h1 = hash1(key, ht->capacity);
	if(!ht->data[h1])
	{
		size_t h2 = hash2(key);
		size_t i = 0;
		//backet* data = ht->data[(h1 + i*h2) % ht->capacity];
		while(!ht->data[(h1 + i*h2) % ht->capacity]->key && !(strcmp(key, ht->data[(h1 + i*h2) % ht->capacity]->key)))  //если несколько одинаковых ключей значение будет равно последнему
		{
			if(!ht->data[(h1 + i*h2) % ht->capacity])
			{
				ht->data[(h1 + i*h2) % ht->capacity]->val = val;
				ht->data[(h1 + i*h2) % ht->capacity]->key = (char*)malloc(sizeof(char)*(strlen(key) + 1));
				//check
				//strcpy(, key);

			}
			else
			{
				ht->data[(h1 + i*h2) % ht->capacity]->val = val;
			}
			++i;
			ht->data[(h1 + i*h2) % ht->capacity]->is_del = 0;
			//data = ht->data[(h1 + i*h2) % ht->capacity];
		}
	}
	else
	{
		ht->data[(h1 + i*h2) % ht->capacity]->val = val;
		ht->data[(h1 + i*h2) % ht->capacity]->key = (char*)malloc(sizeof(char)*(strlen(key) + 1));
		//check
		//strcpy(, key);
	}
	++ht->size;

	return 0;
}

int rehash(hash_table* ht)
{
	size_t new_capasity = ht->capacity * RESIZE_COEFF;
	backet* new_data = (backet*)calloc(sizeof(backet), new_capasity);
	if(!new_data)
		return 1;

	for(size_t i = 0; i < ht->capacity; ++i)
	{
		if(!ht->data[i]->key)
			ht_super_insert(new_data, new_capasity, ht->data[i]->key, ht->data[i]->val);
	}
	free(ht->data);

	ht->data = new_data;
	ht->capacity = new_capasity;
	ht->size_del = 0;

	return 0;
}

int erase(hash_table* ht, char* key)
{
	if(!ht)
		return 1;
	size_t  index = ht_find_ind(ht, key);  //place of key or place we can push it
	if(!ht->data[index]->key)
		return 1;
	ht->data[index]->is_del = 1;
	//--ht->size;   не делать если не считаем, сколько удалено элементов(не удаляем их нормально)
	--ht->size;
	++ht->size_del;
	if(ht->size + ht->size_del <= ht->capacity*)
}

int* ht_get(hash_table* ht, char* key)
{
	if(!ht)
		return NULL;
	size_t index = ht_find_ind(ht, key);
	if(!ht->data[index]->key || ht->data[index]->is_del)
		return NULL;
	return &(ht->data[index]->val);
}

