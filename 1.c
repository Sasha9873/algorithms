#include <stdio.h>
#include <stdint.h> // гарантия размера
#include <stdlib.h>

typedef unsigned int T;

#define K 2

int radix_sort(T* a, T n)
{
	T* b = (T*)malloc(n * sizeof(T));
	if(b == NULL)
		return 1;

	uint64_t cnt[K];


	uint64_t length = sizeof(T) * 8;
	uint64_t mask = 1;

	for(uint64_t phase = 0; phase < length; ++phase) //побитово сортируем (mask) справа налево
	{
		for(uint64_t i = 0; i < K; ++i)  // = {0}; обнуляем массив с суммами, которые отвечают за то, сколько чисел было до этого(в отсортированных по левой цифре и т.д)
		{
			cnt[i] = 0;
		}

		for(uint64_t i = 0; i < n; ++i)  //кол-во цифр
		{
			++cnt[(a[i] & mask) ==  mask];
		}

		uint64_t count = 0;
		for(uint64_t i = 0; i < K; ++i) //префиксная сумма в cnt
		{
			uint64_t tmp = cnt[i];
			cnt[i] = count;
			count += tmp;
		}

		for(uint64_t i = 0; i < n; ++i)  //записываем в b[i] отсортированный массив
		{
			b[cnt[(a[i] & mask) ==  mask]++] = a[i];
		}

		for(uint64_t i = 0; i < n; ++i)
			a[i] = b[i];

		mask <<= 1;
	}
	
	return 0;
}

int radix_sort(T* a, T n)
{
	T* b = (T*)malloc(n * sizeof(T));
	if(b == NULL)
		return 1;

	uint64_t cnt[K];


	uint64_t length = sizeof(T) * 8;
	uint64_t mask = 1;

	for(uint64_t phase = 0; phase < length; ++phase) //побитово сортируем (mask) справа налево
	{
		for(uint64_t i = 0; i < K; ++i)  // = {0}; обнуляем массив с суммами, которые отвечают за то, сколько чисел было до этого(в отсортированных по левой цифре и т.д)
		{
			cnt[i] = 0;
		}

		for(uint64_t i = 0; i < n; ++i)  //кол-во цифр
		{
			++cnt[(a[i] & mask) ==  mask];
		}

		uint64_t count = 0;
		for(uint64_t i = 0; i < K; ++i) //префиксная сумма в cnt
		{
			uint64_t tmp = cnt[i];
			cnt[i] = count;
			count += tmp;
		}

		for(uint64_t i = 0; i < n; ++i)  //записываем в b[i] отсортированный массив
		{
			b[cnt[(a[i] & mask) ==  mask]++] = a[i];
		}

		for(uint64_t i = 0; i < n; ++i)
			a[i] = b[i];

		mask <<= 1;
	}
	
	return 0;
}


int main()
{
	unsigned int a[1000];
	unsigned int n;

	scanf("%d", &n);
	for(uint64_t i = 0; i < n; ++i)
		scanf("%d", a + i);

	radix_sort(a, n);

	for(uint64_t i = 0; i < n; ++i)
		printf("%d ", *(a + i));

	return 0;
}