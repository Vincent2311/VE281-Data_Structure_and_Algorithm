#include <climits>
#include <iostream>
#include <list>
#include <vector>
// You are not allowed to include additional libraries, either in shortestP2P.hpp or shortestP2P.cc

#define INF INT_MAX

using namespace std;

class ShortestP2P {
 public:
  ShortestP2P() {}

  ~ShortestP2P() { delete[] dist; }

  /* Read the graph from stdin
   * The input has the following format:
   *
   * Firstline: 1 unsigned int, the total number of verticies, X.
   * Secondline: 1 unsigned int, the total number of edges between vertices, Y.
   * A set of Y lines, each line contains tuple of 2 unsigned int and 1 int (not unsigned int!), in the format of:
   * A(point 1, unsigned) B(point 2, unsigned) dist(distance, int)
   *
   * Example:
   * 4
   * 4
   * 0 1 5
   * 1 2 -1
   * 2 3 4
   * 3 1 6
   *
   *
   *
   *
   * Vertices that are not connected have a infinitly large distance. You may use INF (previously defined at the top of
   * this cope snippet) for infinitly large distance.
   *
   * Special: when the graph is not proper, where there exist any pair of vertices whose minimum distance does not
   * exist, terminate immediately by printing: cout << "Invalid graph. Exiting." << endl;
   *
   * Note: vertex pairs that are not connected, which have infinitely large distances are not considered cases where
   * "minimum distances do not exist".
   */
  void readGraph() {
    std::cin >> vertices;
    dist = new int[vertices * vertices];
    std::fill_n(dist, vertices * vertices, INF);
    for (unsigned int i = 0; i < vertices; i++) {
      dist[i * vertices + i] = 0;
    }

    std::cin >> edges;

    unsigned int startNode;
    unsigned int endNode;
    int weight;

    for (unsigned int i = 0; i < edges; i++) {
      std::cin >> startNode >> endNode >> weight;
      dist[startNode * vertices + endNode] = weight;
    }

    for (unsigned int k = 0; k < vertices; k++) {
      for (unsigned int i = 0; i < vertices; i++) {
        for (unsigned int j = 0; j < vertices; j++) {
          if ((dist[k * vertices + j] != INF && dist[i * vertices + k] != INF) &&
              dist[i * vertices + j] > (dist[i * vertices + k] + dist[k * vertices + j]))
            dist[i * vertices + j] = dist[i * vertices + k] + dist[k * vertices + j];
        }
        if (dist[i * vertices + i] < 0) {
          cout << "Invalid graph. Exiting." << endl;
          std::exit(0);
        }
      }
    }
  }

  /* Input: 2 vertices A and B
   * Output: distance between them.
   * cout << dist << endl;
   *
   * When the A and B are not connected print INF:
   * cout << "INF" << endl;
   */
  void distance(unsigned int A, unsigned int B) {
    if (A == INF) {
      return;
    }
    if (dist[A * vertices + B] == INF) {
      cout << "INF" << endl;
    } else {
      cout << dist[A * vertices + B] << endl;
    }
  };

 private:
  // internal data and functions.
  unsigned int vertices;
  unsigned int edges;
  int *dist;
};
