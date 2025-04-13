#include <string>
#include "minpriorityqueue.h" // Already includes <vector>, <unordered_map>, <utility>
#include "solve.h"
#include "vertex.h"

using namespace std;

// Helper function: Splits the input maze string at newlines.
static vector<string> parseMaze(const string &maze) {
    vector<string> grid;
    size_t pos = 0;
    while (pos < maze.size()) {
        size_t newline = maze.find('\n', pos);
        if (newline == string::npos)
            break;
        grid.push_back(maze.substr(pos, newline - pos));
        pos = newline + 1;
    }
    return grid;
}

// Helper function: Returns the cost to step onto the cell at the vertex's location
// (looked up in the grid). If the cell is a digit ('0'-'9'), cost equals its numeric value;
// for a space, cost is 1.
static int getCost(Vertex *v, const vector<string> &grid) {
    char ch = grid[v->row][v->col];
    if (ch >= '0' && ch <= '9')
        return ch - '0';
    return 1;
}

string solve(string maze) {
    // 1. Parse the maze string into a vector of row strings.
    vector<string> grid = parseMaze(maze);
    int rowCount = grid.size();
    if (rowCount == 0)
        return maze;
    int colCount = grid[0].size();

    // 2. Create a 2D vector of Vertex pointers.
    // For any cell that is not a wall ('#'), allocate a Vertex.
    vector<vector<Vertex*>> vertices(rowCount, vector<Vertex*>(colCount, 0));

    // Map for portal vertices: digit character -> vector of Vertex pointers.
    unordered_map<char, vector<Vertex*>> portalMap;

    // Identify the two exits (boundary spaces) and allocate vertices.
    // (Assume exactly two exits on the boundary.)
    Vertex *startVertex = 0, *goalVertex = 0;
    for (int r = 0; r < rowCount; r++) {
        for (int c = 0; c < colCount; c++) {
            char ch = grid[r][c];
            if (ch != '#') {
                // Create a vertex using the provided constructor (r, c).
                vertices[r][c] = new Vertex(r, c);
                // Record row and col (if not set inside the constructor).
                vertices[r][c]->row = r;
                vertices[r][c]->col = c;
                // If on the boundary and the cell is a space, consider it an exit.
                if ((r == 0 || r == rowCount - 1 || c == 0 || c == colCount - 1) && ch == ' ') {
                    if (startVertex == 0)
                        startVertex = vertices[r][c];
                    else
                        goalVertex = vertices[r][c];
                }
                // If the cell is a digit (portal), add it to the portal map.
                if (ch >= '0' && ch <= '9')
                    portalMap[ch].push_back(vertices[r][c]);
            }
        }
    }
    if (!startVertex || !goalVertex) {
        // Clean up allocated vertices.
        for (int r = 0; r < rowCount; r++) {
            for (int c = 0; c < colCount; c++)
                delete vertices[r][c];
        }
        return maze;
    }

    // 3. Build graph edges for adjacent moves (up, down, left, right).
    // The neighbor list (neighs) is a vector of pairs: (neighbor vertex, cost).
    for (int r = 0; r < rowCount; r++) {
        for (int c = 0; c < colCount; c++) {
            if (vertices[r][c] != 0) {
                // Up
                if (r > 0 && vertices[r - 1][c] != 0)
                    vertices[r][c]->neighs.push_back(make_pair(vertices[r - 1][c], getCost(vertices[r - 1][c], grid)));
                // Down
                if (r < rowCount - 1 && vertices[r + 1][c] != 0)
                    vertices[r][c]->neighs.push_back(make_pair(vertices[r + 1][c], getCost(vertices[r + 1][c], grid)));
                // Left
                if (c > 0 && vertices[r][c - 1] != 0)
                    vertices[r][c]->neighs.push_back(make_pair(vertices[r][c - 1], getCost(vertices[r][c - 1], grid)));
                // Right
                if (c < colCount - 1 && vertices[r][c + 1] != 0)
                    vertices[r][c]->neighs.push_back(make_pair(vertices[r][c + 1], getCost(vertices[r][c + 1], grid)));
            }
        }
    }

    // 4. For each portal digit that appears exactly twice, link the two vertices via a portal edge.
    for (auto &entry : portalMap) {
        if (entry.second.size() == 2) {
            Vertex *v1 = entry.second[0];
            Vertex *v2 = entry.second[1];
            int portalCost = getCost(v1, grid); // Same cost for both.
            v1->neighs.push_back(make_pair(v2, portalCost));
            v2->neighs.push_back(make_pair(v1, portalCost));
        }
    }

    // 5. Dijkstra's algorithm:
    // Use MinPriorityQueue<Vertex*> (which provides push(T, int), front(), pop(), and size()).
    unordered_map<Vertex*, int> costSoFar;
    unordered_map<Vertex*, Vertex*> parent;
    MinPriorityQueue<Vertex*> frontier;
    frontier.push(startVertex, 0);
    costSoFar[startVertex] = 0;
    parent[startVertex] = 0;

    bool found = false;
    while (frontier.size() > 0) {
        Vertex *current = frontier.front();
        frontier.pop();
        int currentCost = costSoFar[current];

        if (current == goalVertex) {
            found = true;
            break;
        }

        // Explore each neighbor (which is a pair: (neighbor vertex, edge weight)).
        for (int i = 0; i < current->neighs.size(); i++) {
            Vertex *next = current->neighs[i].first;
            int edgeCost = current->neighs[i].second;
            int newCost = currentCost + edgeCost;
            if (costSoFar.find(next) == costSoFar.end() || newCost < costSoFar[next]) {
                costSoFar[next] = newCost;
                parent[next] = current;
                frontier.push(next, newCost);
            }
        }
    }

    if (!found) {
        // No solution found; cleanup and return original maze.
        for (int r = 0; r < rowCount; r++)
            for (int c = 0; c < colCount; c++)
                delete vertices[r][c];
        return maze;
    }

    // 6. Backtrack from goal to start, marking the path with 'o' in the grid.
    for (Vertex *cur = goalVertex; cur != 0; cur = parent[cur]) {
        grid[cur->row][cur->col] = 'o';
        if (cur == startVertex)
            break;
    }

    // 7. Clean up: Delete all dynamically allocated Vertex objects.
    for (int r = 0; r < rowCount; r++) {
        for (int c = 0; c < colCount; c++) {
            delete vertices[r][c];
        }
    }

    // 8. Reconstruct the solution string from the grid.
    string solution = "";
    for (int r = 0; r < rowCount; r++) {
        solution += grid[r] + "\n";
    }
    return solution;
}
