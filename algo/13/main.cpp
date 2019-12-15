#include <iostream>
#include <queue>


struct Edge {
    int v1;
    int v2;
    int weight;

    bool operator<(const Edge& other) const {
        return weight > other.weight;
    }
};


int main() {
    int n, m;
    std::cin >> n >> m;

    std::vector<std::vector<Edge>> vertices(n);
    std::vector<bool> check(n);

    for (int i = 0; i < m; i++) {
        int v1, v2, weight;
        std::cin >> v1 >> v2 >> weight;
        vertices[v1 - 1].push_back(Edge{v1 - 1, v2 - 1, weight});
        vertices[v2 - 1].push_back(Edge{v1 - 1, v2 - 1, weight});
    }

    int result = 0;
    std::priority_queue<Edge> queue;
    check[0] = true;
    for (Edge e : vertices[0]) {
        queue.push(e);
    }
    for (int i = 0; i < n - 1; i++) {
        Edge minEdge = queue.top();
        while (check[minEdge.v1] && check[minEdge.v2]) {
            queue.pop();
            minEdge = queue.top();
        }
        result += minEdge.weight;
        int v = check[minEdge.v1] ? minEdge.v2 : minEdge.v1;
        check[v] = true;
        for (Edge e : vertices[v]) {
            queue.push(e);
        }
    }
    std::cout << result;
    return 0;
}
