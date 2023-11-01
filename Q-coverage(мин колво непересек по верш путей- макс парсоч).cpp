#include <iostream>
#include <vector>
#include <cstring>

using std::vector;


vector<vector<int>> make_bipartite_graph(int k_vertex, int k_edge)
{
    vector<vector<int>> graph(k_vertex << 1, vector<int>());
    int to, from;
    for(size_t i = 0; i < k_edge; ++i)
    {
        std::cin >> from >> to;

        graph[from - 1].push_back(to + k_vertex - 1);
    }
    			
    return graph;
}

int kuhn(int vertex, vector<int>& matched, const vector<vector<int>>& graph, vector<char>& used)
{
    if(used[vertex])
        return 0;
    used[vertex] = 1;

    for(int to : graph[vertex])
    {
        if(matched[to] == -1 || kuhn(matched[to], matched, graph, used))
        {
            matched[to] = vertex;
            return 1;
        }
    }

    return 0;
}

int make_max_matching(int k_vertex, int k_edge, const vector<vector<int>>& graph)
{
	vector<int> matched(k_vertex << 1, -1);
    vector<char> used(k_vertex << 1, 0);
	int ans = 0;

	for(size_t i = 0; i < k_vertex; ++i)
    {
        used.assign(k_vertex << 1, 0);
        ans += kuhn(i, matched, graph, used);      
    }

    return ans;
}



int main()
{
	size_t k_vertex = 0, k_edge = 0;
	std::cin >> k_vertex >> k_edge;

	vector<vector<int>> graph = make_bipartite_graph(k_vertex, k_edge);

    int ans = make_max_matching(k_vertex, k_edge, graph);

	std::cout << k_vertex - ans;
/**/
	return 0;
}