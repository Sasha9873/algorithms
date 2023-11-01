#include <iostream>
#include <vector>
 
enum states{
    used,
    unused
};
 
 
std::vector<std::vector<int>> read_graph(size_t k_vertex, size_t k_edge) 
{
    std::vector<std::vector<int>> graph(k_vertex, std::vector<int>());
    for(size_t i = 0; i < k_edge; ++i) 
    {
        size_t from, to;
        std::cin >> from >> to;

        graph[from].emplace_back(to);
        graph[to].emplace_back(from);
    }

    return graph;
}

bool paint_vertexes(const std::vector<std::vector<int>>& graph, std::vector<char>& used_v, std::vector<char>& color, int vertex)
{
    for(int to: graph[vertex])
    {
        if(used_v[to] == unused)
        {
            used_v[to] = used;
            color[to] = !color[vertex];

            if(!paint_vertexes(graph, used_v, color, to))
                return false;
        }
        else if(color[vertex] == color[to])
        {
            return false;
        }
    }

    return true;
}
 
bool is_bipartite(const std::vector<std::vector<int>>& graph) 
{
    std::vector<char> used_v(graph.size(), unused);
    std::vector<char> color(graph.size(), 0);

    for(size_t i = 0; i < graph.size(); ++i) 
    {
        if(used_v[i] == unused)
        {
            used_v[i] = used;
            if(!paint_vertexes(graph, used_v, color, i))
                return false;
        }
    }

    return true;
}
 
int main() {
    size_t k_vertex = 0, k_edge = 0;
    std::cin >> k_vertex >> k_edge;

    auto graph = read_graph(k_vertex, k_edge);

    bool is = is_bipartite(graph);

    if(is)
        std::cout << "YES\n";
    else
        std::cout << "NO\n";
    
    return 0;
}