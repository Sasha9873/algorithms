#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define max_len 21

typedef struct node
{
	char key[max_len];
	char value[max_len];
	unsigned char height;
	struct node* left;
	struct node* right;
}tree;

size_t max(size_t a, size_t b)
{
    if (a > b)
        return a;
    return b;
}

int cmp(char* s1, char* s2)
{
    if (strlen(s1) != strlen(s2))
        return max(strlen(s1), strlen(s2));

    int str_cmp = strcmp(s1, s2);

    return str_cmp;
}

unsigned char height(tree* p)
{
	return p ? p->height : 0;
}

int bfactor(tree* p)
{
	return height(p->right) - height(p->left);
}

void fix_height(tree* p)
{
	if(!p)
		return;

	unsigned char hl = height(p->left);
	unsigned char hr = height(p->right);
	p->height = (hl>hr ? hl : hr) + 1;
}

tree* find(tree* p, char* key)
{
	if(!p)
		return NULL;
	if(strcmp(key, p->key) == 0)
		return p;
	else if(strcmp(key, p->key) < 0)
		return find(p->left, key);
	else
		return find(p->right, key);
}

tree* rotate_right(tree* p)
{
	tree* q = p->left;
	p->left = q->right;
	q->right = p;

	fix_height(p);
	fix_height(q);

	return q;
}

tree* rotate_left(tree* q)
{
	tree* p = q->right;
	q->right = p->left;
	p->left = q;

	fix_height(q);
	fix_height(p);

	return p;
}

tree* balance(tree* p)
{
	fix_height(p);

	if(bfactor(p) == 2)
	{
		if(bfactor(p->right) < 0)
			p->right = rotate_right(p->right);
		return rotate_left(p);
	}
	if(bfactor(p) == -2)
	{
		if(bfactor(p->left) > 0)
			p->left = rotate_left(p->left);
		return rotate_right(p);
	}
	return p;
}

tree* insert(tree* p, char* key, char* value)
{
	if(!key || !value)
		return p;

	if (!p)
	{
		p = calloc(1, sizeof(tree));
		if(!p)
			return NULL;

		strcpy(p->key, key);
		strcpy(p->value, value);
		p->height = 1;
		
		return p;
	}

	if (strcmp(key, p->key) < 0)
		p->left = insert(p->left, key, value);
	else if (strcmp(key, p->key) > 0)
		p->right = insert(p->right, key, value);
	else
		strcpy(p->value, value);

	return balance(p);
}

tree* find_min(tree* p)
{
	return p->left ? find_min(p->left) : p;
}

tree* remove_min(tree* p)
{
	if(!p->left)
		return p->right;

	p->left = remove_min(p->left);
	
	return balance(p);
}

tree* erase(tree* p, char* key)
{
	if (!p)
		return NULL;

	if (strcmp(key, p->key) < 0)
		p->left = erase(p->left, key);
	else if (strcmp(key, p->key) > 0)
		p->right = erase(p->right, key);
	else
	{
		if (!p->right)
		{
			tree* temp = p->left;
			free(p);

			return temp;
		}

		if (!p->left)
		{
			tree* temp = p->right;
			free(p);

			return temp;
		}

		tree* left = p->left;
		tree* right = p->right;
		free(p);

		tree* min = find_min(right);
		min->right = remove_min(right);
		min->left = left;
		
		return balance(min);
	}

	return balance(p);
}


void delete_tree(tree* p)
{
    if (!p)
        return;

    delete_tree(p->left);
    delete_tree(p->right);

    free(p);
}

int main()
{
	char* type = malloc(10 * sizeof(char));
	if(!type)
		return 1;

    char key[max_len] = {0};
    char value[max_len] = {0};

	tree* tree_new = NULL;

    while(scanf("%10s", type) > 0)
    {
        if(strcmp(type, "insert") == 0)
        {
            scanf("%20s %20s", key, value);
            tree_new = insert(tree_new, key, value);
        }
        else if(strcmp(type, "erase") == 0)
        {
            scanf("%20s", key);
            tree_new = erase(tree_new, key);
        }
        else if(strcmp(type, "find") == 0)
        {
            scanf("%20s", key);
            tree* found = find(tree_new, key);

            if (found == NULL)
            	printf("MISSING\n");
            else
                printf("%s\n", found->value);
        }
    }

    free(type);
    delete_tree(tree_new);

	return 0;
}