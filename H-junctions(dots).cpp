#include <iostream>
#include <vector>
#include <algorithm>
#include <set>

enum states{
    used,
    unused
};
 
static int NO_PARENT = -1;
 
std::vector<std::vector<int>> read_graph(size_t k_vertex, size_t k_edge) 
{
    std::vector<std::vector<int>> graph(k_vertex, std::vector<int>());
    for(size_t i = 0; i < k_edge; ++i) 
    {
        size_t from, to;
        std::cin >> from >> to;

        graph[from - 1].emplace_back(to - 1);
        graph[to - 1].emplace_back(from - 1);
    }

    return graph;
}

void dfs(int vertex, const std::vector<std::vector<int>>& graph, std::vector<char>& used_v, std::vector<int>& fup, 
    std::vector<int>& time_in, std::set<int>& num_dots, int& timer, int parent) 
{
    used_v[vertex] = used;
    time_in[vertex] = fup[vertex] = timer++;
    int children = 0;

    for(auto to: graph[vertex]) 
    {
        if(to == parent)
            continue;

        if(used_v[to] == used) 
        {
            //printf("fup[%d] = %d tin[%ld] = %d\n", vertex, fup[vertex], cur_edge.to, time_in[cur_edge.to]);
            fup[vertex] = std::min(fup[vertex], time_in[to]);

        } 
        else 
        {
            //printf("to = %ld v = %d\n", cur_edge.to, vertex);
            dfs(to, graph, used_v, fup, time_in, num_dots, timer, vertex);
            //printf("fup[%d]v = %d fup[%ld]to = %d tin[%d]v = %d\n", vertex, fup[vertex], cur_edge.to, fup[cur_edge.to], vertex, time_in[vertex]);
            fup[vertex] = std::min(fup[vertex], fup[to]);
            if (fup[to] >= time_in[vertex] && parent != NO_PARENT)
                num_dots.insert(vertex + 1);

            ++children;
        }
    }

    if(parent == -1 && children > 1)
        num_dots.insert(vertex + 1);
}

std::set<int> find_dots(const std::vector<std::vector<int>>& graph) 
{
    int timer = 0;
    std::vector<char> used_v(graph.size(), unused);
    std::vector<int> fup(graph.size(), 0);
    std::vector<int> time_in(graph.size(), 0);
    std::set<int> num_dots;

    for(size_t i = 0; i < graph.size(); ++i)
    {
        if(used_v[i] == unused)
        {
            dfs(i, graph, used_v, fup, time_in, num_dots, timer, NO_PARENT);
        }
    } 

    return num_dots;    
}

int main() {
    size_t k_vertex = 0, k_edge = 0;
    std::cin >> k_vertex >> k_edge;

    auto graph = read_graph(k_vertex, k_edge);

    std::set<int>dots = find_dots(graph);

    std::cout << dots.size() << "\n";

    for(int dot: dots)
        std::cout << dot << " ";

    return 0;
}
