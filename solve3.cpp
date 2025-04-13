#include <string>
#include "minpriorityqueue.h" // Already includes <vector>, <unordered_map>, <utility>
#include "solve.h"
#include "vertex.h"

using namespace std;

// Helper function: Splits the maze string (by newline) into a vector of row strings.
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

// Helper function for portal cost: returns the digit value found at the vertex's position.
static int getPortalCost(Vertex *v, const vector<string> &grid) {
    return grid[v->row][v->col] - '0';
}

string solve(string maze) {
    // 1. Parse the maze into a grid.
    vector<string> grid = parseMaze(maze);
    int rowCount = grid.size();
    if (rowCount == 0)
        return maze;
    int colCount = grid[0].size();

    // 2. Allocate a 2D vector of Vertex pointers for all non-wall cells.
    vector<vector<Vertex*>> vertices(rowCount, vector<Vertex*>(colCount, 0));
    // Portal map: maps digit (portal) to a vector of corresponding Vertex pointers.
    unordered_map<char, vector<Vertex*>> portalMap;

    // Identify the two exits on the boundary and allocate vertices.
    // (Assuming exactly two exits exist.)
    Vertex *startVertex = 0, *goalVertex = 0;
    for (int r = 0; r < rowCount; r++) {
        for (int c = 0; c < colCount; c++) {
            char ch = grid[r][c];
            if (ch != '#') {
                // Create a Vertex for the cell.
                vertices[r][c] = new Vertex(r, c);
                vertices[r][c]->row = r;
                vertices[r][c]->col = c;
                // When the cell is on the boundary and is not a wall, treat it as an exit.
                if ((r == 0 || r == rowCount - 1 || c == 0 || c == colCount - 1) && ch != '#') {
                    if (startVertex == 0)
                        startVertex = vertices[r][c];
                    else if (goalVertex == 0)
                        goalVertex = vertices[r][c];
                }
                // If the cell is a digit (portal), record it.
                if (ch >= '0' && ch <= '9')
                    portalMap[ch].push_back(vertices[r][c]);
            }
        }
    }

    if (!startVertex || !goalVertex) {
        // Cleanup and return original maze if no exits found.
        for (int r = 0; r < rowCount; r++)
            for (int c = 0; c < colCount; c++)
                delete vertices[r][c];
        return maze;
    }

    // 3. Build adjacent edges for up/down/left/right moves.
    // For adjacent moves, we use a cost of 1.
    for (int r = 0; r < rowCount; r++) {
        for (int c = 0; c < colCount; c++) {
            if (vertices[r][c] != 0) {
                if (r > 0 && vertices[r-1][c] != 0)
                    vertices[r][c]->neighs.push_back(make_pair(vertices[r-1][c], 1));
                if (r < rowCount - 1 && vertices[r+1][c] != 0)
                    vertices[r][c]->neighs.push_back(make_pair(vertices[r+1][c], 1));
                if (c > 0 && vertices[r][c-1] != 0)
                    vertices[r][c]->neighs.push_back(make_pair(vertices[r][c-1], 1));
                if (c < colCount - 1 && vertices[r][c+1] != 0)
                    vertices[r][c]->neighs.push_back(make_pair(vertices[r][c+1], 1));
            }
        }
    }

    // 4. Add portal edges.
    for (auto &entry : portalMap) {
        if (entry.second.size() == 2) {
            Vertex *v1 = entry.second[0];
            Vertex *v2 = entry.second[1];
            int portalCost = getPortalCost(v1, grid); // Same cost for both endpoints.
            v1->neighs.push_back(make_pair(v2, portalCost));
            v2->neighs.push_back(make_pair(v1, portalCost));
        }
    }

    // 5. Run Dijkstra's algorithm using your MinPriorityQueue.
    // Use unordered_maps to track the best cost and parent for each vertex.
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

        // Explore each neighbor.
        for (int i = 0; i < current->neighs.size(); i++) {
            Vertex *next = current->neighs[i].first;
            int edgeCost = current->neighs[i].second;
            int newCost = currentCost + edgeCost;
            if (costSoFar.find(next) == costSoFar.end()) {
                costSoFar[next] = newCost;
                parent[next] = current;
                frontier.push(next, newCost);
            } else if (newCost < costSoFar[next]) {
                costSoFar[next] = newCost;
                parent[next] = current;
                frontier.decrease_key(next, newCost);
            }
        }
    }

    if (!found) {
        // No solution found; clean up and return the original maze.
        for (int r = 0; r < rowCount; r++)
            for (int c = 0; c < colCount; c++)
                delete vertices[r][c];
        return maze;
    }

    // 6. Backtrack from goal to start and mark the path with 'o'.
    // We'll copy grid to solutionGrid for marking.
    vector<string> solutionGrid = grid;
    for (Vertex *cur = goalVertex; cur != 0; cur = parent[cur]) {
        solutionGrid[cur->row][cur->col] = 'o';
        if (cur == startVertex)
            break;
    }

    // 7. Clean up: delete all allocated vertices.
    for (int r = 0; r < rowCount; r++) {
        for (int c = 0; c < colCount; c++) {
            delete vertices[r][c];
        }
    }

    // 8. Reconstruct the solution string.
    string solution = "";
    for (int r = 0; r < rowCount; r++) {
        solution += solutionGrid[r] + "\n";
    }
    return solution;
}
