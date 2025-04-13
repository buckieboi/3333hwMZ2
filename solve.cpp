


#include <string>
#include <unordered_set>
#include "minpriorityqueue.h" // Includes <vector>, <unordered_map>, <utility>
#include "solve.h"
#include "vertex.h"

using namespace std;

// For the mandatory running time, assume that the time for
// operations of queue, unordered_set, and map are O(1). 
// (They are for average-case, but not worst-case).
//
// For the mandatory running time below, s is the length of 
// the input string representing the maze.
// 
// For a complete description of the maze string 
// and maze solution formats, see the assignment pdf.
//
//
// Returns a string representing a shortest solution to the maze.
// Undefined behavior if the maze is not valid or has no solution.
//
// Must run in O(s*log(s)) time.





//confused what to do with this as its in the solve.h file.
// oh does this sent the string over? then we can parse it.
string solve(string maze);




// Pseudocode:
// Convert the maze string into a list of row strings.
// This function splits the maze input (by newline) and returns a vector where each element is a row.
static vector<string> parseMaze(const string& maze) {
    vector<string> grid;
    size_t pos = 0;
    while (pos < maze.size()) {
        size_t newline = maze.find('\n', pos);
        if (newline == string::npos)
            break;
        grid.emplace_back(maze.substr(pos, newline - pos));
        pos = newline + 1;
    }
    return grid;
}



// 4. Build the graph by linking each Vertex to its valid (non-wall) adjacent neighbors
for (int r = 0; r < rowCount; r++) {
    for (int c = 0; c < colCount; c++) {
        if (vertices[r][c] != nullptr) {
            // Up
            if (r > 0 && vertices[r - 1][c] != nullptr)
                vertices[r][c]->neighs.push_back(vertices[r - 1][c]);
            // Down
            if (r < rowCount - 1 && vertices[r + 1][c] != nullptr)
                vertices[r][c]->neighs.push_back(vertices[r + 1][c]);
            // Left
            if (c > 0 && vertices[r][c - 1] != nullptr)
                vertices[r][c]->neighs.push_back(vertices[r][c - 1]);
            // Right
            if (c < colCount - 1 && vertices[r][c + 1] != nullptr)
                vertices[r][c]->neighs.push_back(vertices[r][c + 1]);
        }
    }
}


// If no path is found, cleanup and return the original maze
if (!found) {
    for (auto &row : vertices)
        for (auto &v : row)
            delete v;
    return maze;
}


    // 6. Backtrack from the exit to the entrance, marking the path with 'o'
    for (Vertex* cur = goalVertex; ; cur = parent[cur]) {
        grid[cur->row][cur->col] = 'o';
        if (cur == startVertex)
            break;
    }

    // 7. Cleanup dynamically allocated Vertex objects
    for (auto &row : vertices) {
        for (auto &v : row) {
            delete v;
        }
    }
    
    // 8. Rebuild the solution string from the modified grid
    string solution;
    for (const auto& rowStr : grid) {
        solution += rowStr + "\n";
    }
    return solution;
}

// find the smallest number, entrance of the portal on the grid.
// should we just can left to right top to bottom for it?

// 1. Parse the maze and find the two exit positions and all portal pairs.
void findSmallestportal(vertex[r][c]){
    //starts at 1st maze cell the looks throughout maze for the portals.
    entrancePortal = 9
    for(int i; i< grid.size(); i++){ //rows 
        for(int i; i< grid.size(); i++){ //columns
            //lets use ascii values to tell if its 0-9
            // if its '#' or ' ' skip for now.
            if( vertex[r][c] == '0-9'){
                if( vertex[r][c] <= entrancePortal){
                    entrancePortal = vertex[r][c]
                }
            }
        }
        // i need 2 forloops to search the rows and coloumns right?
    }
    
    // i need 2 forloops to search the rows and coloumns right?
}

//do i need to find the exait after finding the entrance? find the mastching number 
// duplicate in the graph?