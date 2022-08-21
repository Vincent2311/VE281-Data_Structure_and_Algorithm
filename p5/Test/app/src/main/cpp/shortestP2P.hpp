#include <climits>
#include <iostream>
#include <list>
#include <vector>
#include <algorithm>


using namespace std;

class ShortestP2P {
public:
    ShortestP2P() {}

    ~ShortestP2P() {
        delete[] graph;
    }

    /**
     * read the graph from a vector, the graph is connected and you don't need to consider negative edges.
     *
     * @param vertex_number: the number of vertices in the graph
     * @param edges: a vector representing the edges in the undirected graph,
     *               each element of it is a vector containing three integers,
     *               the first two integers are the two vertices of an edge,
     *               the third integer is the length of the edge
     */

    void setGraph(int vertex_number, vector<vector<int> *> *edges) {
        V = vertex_number;
        graph = new int[V * V];
        fill_n(graph, V*V, 0);
        for (auto i: *edges) {
            graph[(*i)[0] * V + (*i)[1]] = (*i)[2];
            graph[(*i)[1] * V + (*i)[0]] = (*i)[2];
        }
    }

    /**
     * input: two vertices A and B
     * @param A
     * @param B
     * @return a vector, which contains all the vertices in the shortest path (including A and B)
     */
    vector<int> *distance(unsigned int A, unsigned int B) {
        auto *order = new vector<int>;

        dist = new int[V];
        predecessor = new int[V];
        sptSet = new bool[V];
        for (int i = 0; i < V; i++) {
            dist[i] = INT_MAX;
            sptSet[i] = false;
            predecessor[i] = -1;
        }


        dist[A] = 0;
        for (int count = 0; count < V - 1; count++) {
            int u = minDistance(dist, sptSet);

            sptSet[u] = true;

            for (int v = 0; v < V; v++) {
                if (!sptSet[v] && graph[u * V + v] && dist[u] + graph[u * V + v] < dist[v]) {
                    dist[v] = dist[u] + graph[u * V + v];
                    predecessor[v] = u;
                }
            }
        }

        printPath(predecessor,B,order);


        delete[] sptSet;
        delete[] predecessor;
        delete[] dist;

        return order;
    };

private:
    // internal data and functions.
    int V;
    int *dist;
    bool *sptSet;
    int *graph;
    int *predecessor;

    int minDistance(int dist[], bool sptSet[]) {
        int min = INT_MAX, min_index;

        for (int v = 0; v < V; v++)
            if (!sptSet[v] && dist[v] <= min) min = dist[v], min_index = v;

        return min_index;
    }

    void printPath(int parent[], int j, vector<int>* vec) {
        if (parent[j] == -1) {
            vec->push_back(j);
            return;
        }
        printPath(parent, parent[j],vec);
        vec->push_back(j);
    }
};
