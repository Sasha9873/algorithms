#include <iostream>
#include <vector>
#include <algorithm>


enum states{
    used,
    unused
};

struct edge{
    edge(size_t to, size_t num): to(to), num(num) {}
   
    size_t to;
    size_t num;
};
 
 
std::vector<std::vector<edge>> read_graph(size_t k_vertex, size_t k_edge) 
{
    std::vector<std::vector<edge>> graph(k_vertex, std::vector<edge>());
    for(size_t i = 0; i < k_edge; ++i) 
    {
        size_t from, to;
        std::cin >> from >> to;

        graph[from - 1].emplace_back(to - 1, i + 1);
        graph[to - 1].emplace_back(from - 1, i + 1);
    }

    return graph;
}

void IS_BRIDGE(int v, int to) {
    std::cout << "Bridge: " << v << " " << to << '\n';
}

void dfs(int vertex, const std::vector<std::vector<edge>>& graph, std::vector<char>& used_v, std::vector<int>& fup, 
    std::vector<int>& time_in, std::vector<int>& num_bridges, int& timer, int& k_bridge, int parent) 
{
    used_v[vertex] = used;
    time_in[vertex] = fup[vertex] = timer++;

    for(auto cur_edge: graph[vertex]) 
    {
        if (cur_edge.to == parent)
            continue;

        if (used_v[cur_edge.to] == used) 
        {
            //printf("fup[%d] = %d tin[%ld] = %d\n", vertex, fup[vertex], cur_edge.to, time_in[cur_edge.to]);
            fup[vertex] = std::min(fup[vertex], time_in[cur_edge.to]);

        } 
        else 
        {
            //printf("to = %ld v = %d\n", cur_edge.to, vertex);
            dfs(cur_edge.to, graph, used_v, fup, time_in, num_bridges, timer, k_bridge, vertex);
            //printf("fup[%d]v = %d fup[%ld]to = %d tin[%d]v = %d\n", vertex, fup[vertex], cur_edge.to, fup[cur_edge.to], vertex, time_in[vertex]);
            fup[vertex] = std::min(fup[vertex], fup[cur_edge.to]);
            if (fup[cur_edge.to] > time_in[vertex]) 
            {
                ++k_bridge;
                num_bridges.emplace_back(cur_edge.num);
                //IS_BRIDGE(vertex, cur_edge.to);
            }
        }
    }
}

std::pair<std::vector<int>, int> find_bridges(const std::vector<std::vector<edge>>& graph) 
{
    int timer = 0, k_bridge = 0;
    std::vector<char> used_v(graph.size(), unused);
    std::vector<int> fup(graph.size(), 0);
    std::vector<int> time_in(graph.size(), 0);
    std::vector<int> num_bridges(graph.size(), 0);

    for (size_t i = 0; i < graph.size(); ++i)
    {
        if(used_v[i] == unused)
        {
            dfs(i, graph, used_v, fup, time_in, num_bridges, timer, k_bridge, -1);
        }
    } 

    return {num_bridges, k_bridge};    
}

int main() {
    size_t k_vertex = 0, k_edge = 0;
    std::cin >> k_vertex >> k_edge;

    auto graph = read_graph(k_vertex, k_edge);

    std::pair<std::vector<int>, int> bridges = find_bridges(graph);

    std::cout << bridges.second << "\n";

    std::sort(bridges.first.begin(), bridges.first.end());

    for(int br: bridges.first)
    {
        if(br > 0)
            std::cout << br << " ";
    }

    return 0;
}
