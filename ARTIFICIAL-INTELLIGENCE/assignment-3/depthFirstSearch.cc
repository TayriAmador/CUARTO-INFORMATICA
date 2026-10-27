/** 
 * Vilnius University
 * Artificial Intelligence: Classical Principles
 * Assignment 3: Depth first search 
 * 
 * @author Tayri Amador Guillén
 * @date 2026-09-20
 * @brief Solution of Depth First Search
 */
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <algorithm>
#include <iomanip>
#include <sstream>

/**
 * @brief Represents an individual cell in the maze using coordinates.
 */
struct Cell { 
    int x; 
    int y; 
};

/**
 * @brief Structure storing the maze information.
 */
struct Maze {
    int M{0}; // Maze width 
    int N{0}; // Maze height 
    std::vector<std::vector<int>> lab; 
};

/**
 * @brief Stores the final results after the algorithm's execution.
 */
struct RunResult {
    bool pathExists{false};         
    long long trials{0};            
    std::vector<Cell> pathNodes;    
    std::vector<int> ruleSeq;       
};

//Cardinal displacements.
static const int CX[5] = {0, -1, 0, 1, 0};
static const int CY[5] = {0, 0, -1, 0, 1};

/**
 * @brief State context passed throughout the DFS recursion.
 */
struct DFSContext {
    Maze *maze;                    
    int variant{1};                    
    int L{2};                          
    long long trials{0};              
    int stepNo{0};                 
    bool verbose{true};              
    bool yes{false};                
    int depth{0};                  
    std::vector<Cell> callStack;   
    std::vector<std::string> traceLines;
};

/**
 * @brief Checks if a coordinate is within the maze boundaries.
 * @param mz Constant reference to the maze.
 * @param x X-coordinate to evaluate.
 * @param y Y-coordinate to evaluate.
 * @return true If (x,y) is inside the map.
 * @return false If (x,y) is outside the map.
 */
static inline bool inBounds(const Maze &mz, int x, int y) {
    return x >= 1 && x <= mz.M && y >= 1 && y <= mz.N;
}

/**
 * @brief Logs a successful move to a free cell in the trace.
 * @param ctx Reference to the DFS context.
 * @param rule Applied rule.
 * @param u New X-coordinate.
 * @param v New Y-coordinate.
 * @param L Value assigned to the cell.
 * @param isTerminal True if this cell is on the map border (exit).
 */
static void printExplore(DFSContext &ctx, int rule, int u, int v, int L, bool isTerminal) {
    ctx.stepNo++;
    std::string s;
    s.append(ctx.depth, '-');
    {
        std::ostringstream oss;
        oss << "R" << rule << ". U=" << u << ", V=" << v
            << ". Free. L:=L+1=" << L << ". LAB[" << u << "," << v << "]:=" << L << ".";
        if (isTerminal) oss << " Terminal.";
        s += oss.str();
    }
    ctx.traceLines.push_back(s);
}

/**
 * @brief Logs a collision against a wall in the trace.
 * @param ctx Reference to the DFS context.
 * @param rule Applied rule (1-4).
 * @param u X-coordinate of the wall.
 * @param v Y-coordinate of the wall.
 */
static void printWall(DFSContext &ctx, int rule, int u, int v) {
    ctx.stepNo++;
    std::string s;
    s.append(ctx.depth, '-');
    std::ostringstream oss;
    oss << "R" << rule << ". U=" << u << ", V=" << v << ". Wall.";
    s += oss.str();
    ctx.traceLines.push_back(s);
}

/**
 * @brief Logs a collision against an already visited path in the trace.
 * @param ctx Reference to the DFS context.
 * @param rule Applied rule (1-4).
 * @param u X-coordinate of the visited cell.
 * @param v Y-coordinate of the visited cell.
 */
static void printThread(DFSContext &ctx, int rule, int u, int v) {
    ctx.stepNo++;
    std::string s;
    s.append(ctx.depth, '-');
    std::ostringstream oss;
    oss << "R" << rule << ". U=" << u << ", V=" << v << ". Thread.";
    s += oss.str();
    ctx.traceLines.push_back(s);
}

/**
 * @brief Logs a step back from a dead end in the trace.
 * @param ctx Reference to the DFS context.
 * @param x X-coordinate of the abandoned cell.
 * @param y Y-coordinate of the abandoned cell.
 * @param L Current value of the path counter before decrementing.
 * @param newVal New value assigned to the cell upon abandoning it (-1 or 0).
 */
static void printBacktrack(DFSContext &ctx, int x, int y, int L, int newVal) {
    std::string s = "[BACKTRACK]";
    s.append(ctx.depth, '-');
    std::ostringstream oss;
    oss << "Backtrack from X=" << x << ", Y=" << y
        << ", L=" << L << ". LAB[" << x << "," << y << "]:=" << newVal
        << ". L:=L-1=" << (L - 1) << ".";
    s += oss.str();
    ctx.traceLines.push_back(s);
}

/**
 * @brief Main recursive function of the Depth First Search algorithm.
 * @param ctx Reference to the global search context.
 * @param x Current X-coordinate.
 * @param y Current Y-coordinate.
 */
static void tryDfs(DFSContext &ctx, int x, int y) {
    //Base condition. If the exit was already found in another 
    if (ctx.yes) return;
    
    // Iterate through the 4 possible directions 
    for (int k = 1; k <= 4 && !ctx.yes; ++k) {
        int u = x + CX[k];
        int v = y + CY[k];
        
        // If the step goes out of bounds, we ignore it completely
        if (!inBounds(*ctx.maze, u, v)) continue;

        int &cell = ctx.maze->lab[v][u]; //Cell to move to
        
        if (cell == 0) { // If the cell is free
            ctx.trials++;
            ctx.L++;
            cell = ctx.L; // Leave our mark (number > 1)

            // If 'u' or 'v' are on the edges of the matrix, it means we have reached the outer boundary of the maze.
            bool isTerminal = (u == 1 || u == ctx.maze->M || v == 1 || v == ctx.maze->N);
            printExplore(ctx, k, u, v, ctx.L, isTerminal);

            if (isTerminal) {
                ctx.yes = true;
                return; // Start exiting the recursion.
            }

            //Recursive Call
            ctx.depth++;
            ctx.callStack.push_back({u, v});
            
            tryDfs(ctx, u, v); 
            
            ctx.callStack.pop_back();

            // if ctx.yes is still false this path was a dead end, we need to backtrack.
            if (!ctx.yes) {
                // Variant 1: Mark with -1 to never enter again. Variant 2: Leave as 0.
                int newVal = (ctx.variant == 1 ? -1 : 0);
                cell = newVal; 
                printBacktrack(ctx, u, v, ctx.L, newVal);
                ctx.L--; 
            }
            ctx.depth--;
            if (ctx.yes) return;
            
        } else if (cell == 1) { // Hit a wall
            printWall(ctx, k, u, v);
        } else { 
            printThread(ctx, k, u, v);
        }
    }
}

/**
 * @brief Prints the maze to the console with formatting and coordinate axes.
 * @param mz Maze to print.
 * @param title Title to print above the maze.
 */
static void printMaze(const Maze &mz, const std::string &title) {
  if (!title.empty()) std::cout << title << "\n";
  std::cout << "    Y, V\n    ^\n";

  for (int y = mz.N; y >= 1; --y) {
    std::cout << " " << std::setw(3) << y << " |";
    for (int x = 1; x <= mz.M; ++x) {
      std::cout << std::setw(5) << mz.lab[y][x];
    }
    std::cout << "\n";
  }

  std::cout << "     -";
  for (int x = 1; x <= mz.M; ++x) std::cout << "-----";
  std::cout << "--> X, U\n      ";
  for (int x = 1; x <= mz.M; ++x) std::cout << std::setw(5) << x;
  std::cout << "\n";
}

/**
 * @brief Reconstructs the path from the exit back to the start in reverse order.
 * @param mz Solved maze.
 * @param xStart Starting X-coordinate.
 * @param yStart Starting Y-coordinate.
 * @return std::vector<Cell> Sequential list of nodes from start to exit.
 */
static std::vector<Cell> reconstructPath(const Maze &mz, int xStart, int yStart) {
  int bestX = -1, bestY = -1, bestL = -1;
  
  // To start the reconstruction, we look for the largest L value.
  for (int x = 1; x <= mz.M; ++x) {
    for (int y : {1, mz.N}) {
      if (mz.lab[y][x] >= 2 && mz.lab[y][x] > bestL) {
        bestL = mz.lab[y][x]; bestX = x; bestY = y;
      }
    }
  }
  for (int y = 1; y <= mz.N; ++y) {
    for (int x : {1, mz.M}) {
      if (mz.lab[y][x] >= 2 && mz.lab[y][x] > bestL) {
        bestL = mz.lab[y][x]; bestX = x; bestY = y;
      }
    }
  }

  std::vector<Cell> path;
  if (bestL < 2) return path; // No valid exit found
  
  int curX = bestX, curY = bestY, curL = bestL;
  path.push_back({curX, curY});

  while (!(curX == xStart && curY == yStart)) {
    bool moved = false;
    for (int k = 1; k <= 4; ++k) {
      int nx = curX - CX[k], ny = curY - CY[k];
      if (!inBounds(mz, nx, ny)) continue;
      int val = mz.lab[ny][nx];
      if (val == curL - 1 && val >= 2) { // Found the previous step
        curX = nx; curY = ny; curL = val; 
        path.push_back({curX, curY});
        moved = true; 
        break;
      }
    }
    if (!moved) break;
  }
  
  // Since we built it from the end to the beginning, we reverse it
  std::reverse(path.begin(), path.end());
  return path;
}
 
/**
 * @brief Calculates which rules (directions) were used by looking at the difference between adjacent nodes.
 * @param nodes List of nodes in the path.
 * @return std::vector<int> List of rules (1 to 4).
 */
static std::vector<int> reconstructRules(const std::vector<Cell> &nodes) {
  std::vector<int> rules;
  for (size_t i = 1; i < nodes.size(); ++i) {
    int dx = nodes[i].x - nodes[i-1].x;
    int dy = nodes[i].y - nodes[i-1].y;
    for (int r = 1; r <= 4; ++r) {
      if (CX[r] == dx && CY[r] == dy) {
        rules.push_back(r);
        break;
      }
    }
  }
  return rules;
}

/**
 * @brief Prints the rules in a formatted way (e.g. R1, R4, R2).
 * @param rules Vector of rules to print.
 */
static void printRules(const std::vector<int> &rules) {
  std::cout << "   3.3. Rules: ";
  for (size_t i = 0; i < rules.size(); ++i) {
    std::cout << "R" << rules[i];
    if (i + 1 < rules.size()) std::cout << ", ";
    else std::cout << ".";
  }
  std::cout << "\n";
}

/**
 * @brief Prints the sequence of coordinates of the exit path.
 * @param nodes Vector of cells to print.
 */
static void printNodes(const std::vector<Cell> &nodes) {
    std::cout << "   3.4. Nodes: ";
    for (size_t i = 0; i < nodes.size(); ++i) {
        std::cout << "[X=" << nodes[i].x << ",Y=" << nodes[i].y << "]";
        if (i + 1 < nodes.size()) std::cout << ", ";
        else std::cout << ".";
    }
    std::cout << "\n";
}

/**
 * @brief Reads the text file and builds the maze matrix.
 * @param path Path of the file to read.
 * @param mz Reference to the maze (output).
 * @param x0 Starting X-coordinate (output).
 * @param y0 Starting Y-coordinate (output).
 * @return true If loaded successfully.
 * @return false If there was an error opening or parsing.
 */
static bool loadMazeFromFile(const std::string &path, Maze &mz, int &x0, int &y0) {
  std::ifstream in(path);
  if (!in) return false;

  std::vector<std::vector<int>> lines;
  std::string lineStr;
  
  while (std::getline(in, lineStr)) {
    lineStr.erase(std::remove(lineStr.begin(), lineStr.end(), '\r'), lineStr.end());
    if (lineStr.empty()) continue;
    std::istringstream iss(lineStr);
    std::vector<int> row;
    int val;
    while (iss >> val) row.push_back(val);
    if (!row.empty()) lines.push_back(row);
  }

  if (lines.empty()) return false;

  bool hasExplicitDimensions = (lines.size() >= 2 && lines[0].size() == 2 && lines[1].size() > 2);
  int gridStartIdx = hasExplicitDimensions ? 1 : 0;

  int gridEndIdx = static_cast<int>(lines.size()) - 1;
  if (gridEndIdx > gridStartIdx && lines.back().size() == 2) {
    x0 = lines.back()[0];
    y0 = lines.back()[1];
    gridEndIdx--;
  } else {
    x0 = 5; 
    y0 = 4;
  }

  int numGridRows = gridEndIdx - gridStartIdx + 1;
  if (numGridRows <= 0) return false;

  mz.N = numGridRows;
  mz.M = static_cast<int>(lines[gridStartIdx].size());
  mz.lab.assign(mz.N + 1, std::vector<int>(mz.M + 1, 1)); 

  for (int i = 0; i < mz.N; ++i) {
    int lineIdx = gridStartIdx + i;
    int y = i + 1;
    for (int x = 1; x <= mz.M; ++x) {
      if (x - 1 < static_cast<int>(lines[lineIdx].size())) {
        mz.lab[y][x] = lines[lineIdx][x - 1];
      }
    }
  }
  return true;
}

/**
 * @brief Orchestrates the execution of a single attempt to solve the maze from start to finish.
 * @param mz Copy of the maze to solve.
 * @param x0 Starting X-coordinate.
 * @param y0 Starting Y-coordinate.
 * @param variant Variant chosen by the user (1 or 2).
 * @param verbose Defines if many details are printed to the console (Flag).
 * @return RunResult Structure with the obtained results.
 */
static RunResult runOnce(Maze mz, int x0, int y0, int variant, bool verbose) {
  RunResult rr;
  if (!inBounds(mz, x0, y0) || mz.lab[y0][x0] != 0) {
    std::cout << "Error: invalid start position.\n";
    return rr;
  }
  
  // Context initialization
  DFSContext ctx; 
  ctx.maze = &mz; 
  ctx.variant = variant; 
  ctx.verbose = verbose;
  ctx.L = 2; 
  ctx.trials = 0; 
  ctx.stepNo = 0; 
  ctx.depth = 0; 
  ctx.yes = false;

  std::cout << "PART 1. Data\n  1.1. Labyrinth\n\n";
  printMaze(mz, "");
  
  int &startCell = mz.lab[y0][x0]; 
  startCell = ctx.L;
  std::cout << "\n   1.2. Initial position X=" << x0 << ", Y=" << y0 << ". L=" << ctx.L << ".\n\n";

  std::cout << "PART 2. Trace\n";
  ctx.callStack.push_back({x0, y0});
  
  //INITIAL CALL TO THE ALGORITHM
  tryDfs(ctx, x0, y0); 

  // Print the accumulated traces
  int printStep = 1;
  for (size_t i = 0; i < ctx.traceLines.size(); ++i) {
    if (ctx.traceLines[i].rfind("[BACKTRACK]", 0) == 0) {
      std::string line = ctx.traceLines[i].substr(11);
      std::cout << "       " << line << "\n";
    } else {
      std::cout << std::setw(3) << printStep << ") " << ctx.traceLines[i] << "\n";
      printStep++;
    }
  }
  ctx.callStack.pop_back();

  rr.pathExists = ctx.yes;
  rr.trials = ctx.trials;

  std::cout << "\nPART 3. Results\n\n";
  if (rr.pathExists) std::cout << "   3.1. Path is found.\n\n";
  else std::cout << "   3.1. Path is NOT found.\n\n";

  std::cout << "   3.2. Path graphically:\n\n";
  printMaze(mz, "");
  std::cout << "\n";

  // Final reconstruction if we found a path
  if (rr.pathExists) {
    rr.pathNodes = reconstructPath(mz, x0, y0);
    rr.ruleSeq = reconstructRules(rr.pathNodes);
    
    printRules(rr.ruleSeq);
    std::cout << "\n";
    printNodes(rr.pathNodes);
  }
  return rr;
}


int main(int argc, char* argv[]) {
  std::ios::sync_with_stdio(false);
  std::cin.tie(nullptr);

  if (argc < 2) {
    std::cerr << "Usage: " << argv[0] << " <labyrinth_file>\n";
    std::cerr << "Example: " << argv[0] << " input1.txt\n";
    return 1;
  }

  std::string path = argv[1];

  std::cout << "Select variant (V1 or V2): " << std::flush;
  std::string variantStr;
  std::cin >> variantStr;

  int variant = 1;
  std::string variantSuffix = "v1";
  if (variantStr == "V2" || variantStr == "v2" || variantStr == "2") {
    variant = 2;
    variantSuffix = "v2";
  }

  Maze mz; 
  int x0 = 1, y0 = 1;

  if (!loadMazeFromFile(path, mz, x0, y0)) {
    std::cerr << "Failed to read file: " << path << "\n";
    return 1;
  }

  std::string baseName = path;
  std::size_t slashPos = baseName.find_last_of("/\\");
  if (slashPos != std::string::npos) baseName = baseName.substr(slashPos + 1);
  std::size_t dotPos = baseName.find_last_of(".");
  if (dotPos != std::string::npos) baseName = baseName.substr(0, dotPos);

  std::string outputFile = "output_" + baseName + "_" + variantSuffix + ".txt";
  std::ofstream out(outputFile);

  if (!out) {
    std::cerr << "Cannot open " << outputFile << " for writing.\n";
    return 1;
  }

  std::stringstream buffer;
  std::streambuf* oldCout = std::cout.rdbuf(buffer.rdbuf());

  runOnce(mz, x0, y0, variant, true);

  std::cout.rdbuf(oldCout);

  std::string capturedOutput = buffer.str();
  std::cout << "\n" << capturedOutput;
  out << capturedOutput;
  out.close();

  std::cout << "\nResults written successfully to " << outputFile << "\n";
  return 0;
}