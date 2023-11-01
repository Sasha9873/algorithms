#include <iostream>
#include <vector>
#include <algorithm>

struct edge {
    edge(int to, int c, int f, int rev_f): to(to), c(c), f(f), rev_f(rev_f) {}
   
    int to;
    int c;
    int f = 0;
    int rev_f = 0;
};
 
std::vector<std::vector<edge>> read_graph(size_t k_vertex, size_t k_edge, int sourse, int stock) 
{
    std::vector<std::vector<edge>> graph(k_vertex + 2, std::vector<edge>());

    int k_left;
    std::cin >> k_left;

    for(size_t i = 0; i < k_left; ++i)
        graph[sourse].emplace_back(i, 1, 0, 0);
    for(size_t i = k_left; i < k_vertex; ++i)
        graph[i].emplace_back(stock, 1, 0, 0);



    for (size_t i = 0; i < k_edge; ++i) 
    {
        size_t from, to, c;
        std::cin >> from >> to;

        graph[from].emplace_back(to, 1, 0, 0);
    }

    return graph;
}

int dfs(int vertex, int c, std::vector<int>& used, int stock, std::vector<std::vector<edge>>& graph, int k_vertex) //c - пропускная способность
{
	if(vertex == stock)
		return c;

	used[vertex] = 1;

	std::cout << "c = " << c << "\n";
std::cout << "vertex: " << vertex << "\n";

	for(auto& cur_edge : graph[vertex])
	{
		printf("to = %d used = %d\n", cur_edge.to, used[cur_edge.to]);
		if(!used[cur_edge.to] && cur_edge.f < cur_edge.c)
		{

			int delta = dfs(cur_edge.to, std::min(c, cur_edge.c - cur_edge.f), used, stock, graph, k_vertex);
			std::cout << "delta = " << delta << "\n";

			if(delta > 0)
			{
				cur_edge.f += delta;
				std::cout << cur_edge.f << "\n";
				cur_edge.rev_f -= delta;

				for(size_t i = 0; i < k_vertex + 2; ++i)
					for(auto edge : graph[i])
						printf("from = %d to = %d f = %d\n", i, edge.to, edge.f);

				return delta;
			}
		}
	}

	return 0;
}


int main()
{
	size_t k_vertex = 0, k_edge = 0;
	std::cin >> k_vertex >> k_edge;

	int stock, sourse;
    std::cin >> sourse >> stock;

	std::vector<std::vector<edge>> graph = read_graph(k_vertex, k_edge, sourse, stock);

	for(size_t i = 0; i < k_vertex + 2; ++i)
		for(auto to : graph[i])
			printf("from = %d to %d\n", i, to);


	std::vector<int> used(k_vertex + 2, 0);
	while(dfs(sourse, 1, used, stock, graph, k_vertex) > 0) {
		used.assign(k_vertex + 2, 0);
	}
	
	//dfs(sourse, 1, used, stock, graph, k_vertex);

	int stock_float = 0;

	for(size_t i = 0; i < k_vertex + 2; ++i)
		for(auto edge : graph[i])
			printf("from = %d to = %d f = %d\n", i, edge.to, edge.f);

	for(size_t i = 0; i < k_vertex; ++i)
		for(auto edge : graph[i])
			if(edge.to == stock)
			{	
				printf("from = %d  f = %d\n", i, edge.f);
				stock_float += edge.f;
			}

	std::cout << stock_float << "\n";

	return 0;
}