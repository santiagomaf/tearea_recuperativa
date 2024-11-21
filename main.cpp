#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <queue>
#include <unordered_map>
#include <limits>
#include <string>

using namespace std;

struct Edge {
    int dest;
    int weight;
};

class Graph {
private:
    unordered_map<int, vector<Edge>> adjacencyList;

public:
    void loadGraph(const string& filename) {
        ifstream file(filename);
        if (!file.is_open()) {
            cerr << "Error: No se pudo abrir el archivo " << filename << endl;
            return;
        }

        adjacencyList.clear();

        string line;
        int numVertices;
        file >> line >> numVertices; // Leer "n_vertex: <V>"
        while (getline(file, line)) {
            if (line.empty()) continue;
            istringstream iss(line);
            string edgeLabel;
            int src, dest, weight;
            iss >> edgeLabel >> src >> dest >> weight;
            adjacencyList[src].push_back({dest, weight});
            adjacencyList[dest].push_back({src, weight}); // Para grafos no dirigidos
        }
        file.close();
        cout << "Grafo cargado exitosamente desde " << filename << endl;
    }

    pair<vector<int>, int> dijkstra(int start, int end) {
        unordered_map<int, int> distances;
        unordered_map<int, int> previous;
        for (const auto& [vertex, _] : adjacencyList) {
            distances[vertex] = numeric_limits<int>::max();
            previous[vertex] = -1;
        }

        distances[start] = 0;
        priority_queue<pair<int, int>, vector<pair<int, int>>, greater<>> pq;
        pq.push({0, start});

        while (!pq.empty()) {
            int currentDistance = pq.top().first;
            int currentVertex = pq.top().second;
            pq.pop();

            if (currentVertex == end) break;

            if (currentDistance > distances[currentVertex]) continue;

            for (const auto& edge : adjacencyList[currentVertex]) {
                int neighbor = edge.dest;
                int newDist = currentDistance + edge.weight;

                if (newDist < distances[neighbor]) {
                    distances[neighbor] = newDist;
                    previous[neighbor] = currentVertex;
                    pq.push({newDist, neighbor});
                }
            }
        }

        vector<int> path;
        int totalDistance = distances[end];
        if (totalDistance == numeric_limits<int>::max()) {
            cout << "No existe un camino entre " << start << " y " << end << endl;
            return {{}, -1};
        }

        for (int at = end; at != -1; at = previous[at]) {
            path.insert(path.begin(), at);
        }

        return {path, totalDistance};
    }
};

void interactiveMode() {
    Graph graph;
    string command;

    cout << "--- Bienvenido al programa Dijkstra ---" << endl;

    while (true) {
        cout << "> ";
        getline(cin, command);
        istringstream iss(command);
        string cmd;
        iss >> cmd;

        if (cmd == "loadGraph") {
            string filename;
            iss >> filename;
            graph.loadGraph(filename);
        } else if (cmd == "dijkstra") {
            int start, end;
            iss >> start >> end;
            auto [path, distance] = graph.dijkstra(start, end);
            if (distance != -1) {
                cout << "Camino más corto: ";
                for (size_t i = 0; i < path.size(); ++i) {
                    if (i > 0) cout << " -> ";
                    cout << path[i];
                }
                cout << "\nDistancia total: " << distance << endl;
            }
        } else if (cmd == "exit") {
            break;
        } else {
            cout << "Comando no reconocido. Intenta de nuevo." << endl;
        }
    }
}

int main() {
    interactiveMode();
    return 0;
}
