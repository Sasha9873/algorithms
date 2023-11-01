#include <stdio.h>
#include <stdint.h> // гарантия размера
#include <stdlib.h>

typedef unsigned int T;

#define K 2

void fast_sort(unsigned *begin, unsigned *end)
{
	T n = (end - begin);
	//printf("%u\n", n);

	T* b = (T*)malloc(n * sizeof(T));
	if(b == NULL)
		return;

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
			++cnt[(begin[i] & mask) ==  mask];
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
			b[cnt[(begin[i] & mask) ==  mask]++] = begin[i];
		}

		for(uint64_t i = 0; i < n; ++i)
			begin[i] = b[i];

		mask <<= 1;
	}
	free(b);
}

int main() {
    unsigned array[8] = {3,1,4,1,5,9,2,6};
    fast_sort(array, array+8);
    for(int i = 0; i < 8; ++i)
    	printf("%d ", array[i]);
    // Now array = {1,1,2,3,4,5,6,9}
}