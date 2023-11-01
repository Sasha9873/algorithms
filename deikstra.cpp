#include <iostream>
#include <queue>
#include <limits>
 
enum States {
    used,
    unused
};
 
static constexpr size_t kInf = std::numeric_limits<size_t>::max();
 
struct Edge {
    Edge(size_t weight, size_t to): weight(weight), to(to) {}
   
    size_t weight;
    size_t to;
};
 
std::vector<std::vector<Edge> >
ReadGraph(size_t n, size_t m) {
    std::vector<std::vector<Edge>> graph(n, std::vector<Edge>());
    for (size_t i = 0; i < m; ++i) {
        size_t from, to, weight;
        std::cin >> from >> to >> weight;
        graph[from].emplace_back(weight, to);
        graph[to].emplace_back(weight, from);
    }
    return graph;
}
 
std::vector<size_t> Dijkstra(const std::vector<std::vector<Edge>>& graph, size_t from) 
{
    std::vector<unsigned char> used_v(graph.size(), unused);
    std::vector<size_t> dist(graph.size(), kInf);
    dist[from] = 0;
    std::priority_queue<std::pair<size_t, size_t>, std::vector<std::pair<size_t, size_t>>,
    std::greater<std::pair<size_t, size_t>>> queue;
    queue.emplace(dist[from], from);
    for (size_t i = 0; i < graph.size(); ++i) {
        if (queue.empty()) {
            return dist;
        }
        size_t v = queue.top().second;
        while (!queue.empty() && used_v[v] == used) {
            queue.pop();
            v = queue.top().second;
        }
        queue.pop();
        if (dist[v] == kInf) {
            return dist;
        }
        used_v[v] = used;
        for (auto&& edge : graph[v]) {
            if(dist[edge.to] > dist[v] + edge.weight)
                dist[edge.to] = std::min(dist[edge.to], dist[v] + edge.weight);
            queue.emplace(dist[edge.to], edge.to);
        }
    }
    return dist;
}
 
int main() {
    size_t n, m;
    std::cin >> n >> m;
    auto graph = ReadGraph(n, m);
    auto dist = Dijkstra(graph, 0);
    for (auto&& dst : dist) {
        std::cout << dst << " ";
    }
    return 0;
}