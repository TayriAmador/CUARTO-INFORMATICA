/**
 * Vilnius University
 * Artificial Intelligence: Classical Principles
 * Assignment 4: Breadth first search 
 * @author Tayri Amador Guillén
 * @date 2026-10-27
 * @brief Solution of Breadth First Search 
 */

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <iomanip>
#include <sstream>

/**
 * @brief Utility class to duplicate output stream to both console and a file.
 */
class TeeStream {
public:
  TeeStream(std::ostream& console_out, std::ostream& file_out) 
    : out1(console_out), out2(file_out) {}

  template<typename T>
  TeeStream& operator<<(const T& val) {
    out1 << val;
    out2 << val;
    return *this;
  }

  TeeStream& operator<<(std::ostream& (*manip)(std::ostream&)) {
    out1 << manip;
    out2 << manip;
    return *this;
  }

private:
  std::ostream& out1;
  std::ostream& out2;
};

/**
 * @brief Prints the graphical representation of the labyrinth (LABCOPY).
 * 
 * @param out Output stream wrapper (TeeStream).
 * @param lab Constant 2D vector of integers representing the current state.
 * @param M   Total width of the labyrinth (number of columns).
 * @param N   Total height of the labyrinth (number of rows).
 */
void print_labyrinth(TeeStream& out, const std::vector<std::vector<int>>& lab, int M, int N) {
  out << " Y, V\n";
  for (int j = N - 1; j >= 0; --j) {
    out << std::setw(3) << j + 1 << " |";
    for (int i = 0; i < M; ++i) {
      out << std::setw(4) << lab[j][i];
    }
    out << "\n";
  }
  out << "    ";
  for (int i = 0; i < M * 4 + 3; ++i) out << "-";
  out << "> X, U\n    ";
  out << " ";
  for (int i = 1; i <= M; ++i) {
    out << std::setw(4) << i;
  }
  out << "\n" << std::endl;
}

/**
 * @brief Loads the labyrinth data and starting position robustly from a text file.
 * 
 * @param filename Name or path of the text file.
 * @param M        Reference to store the computed labyrinth width.
 * @param N        Reference to store the computed labyrinth height.
 * @param LABCOPY  Reference to the 2D vector where the labyrinth grid will be loaded.
 * @param X        Reference to store the initial X coordinate.
 * @param Y        Reference to store the initial Y coordinate.
 * @return true if the file is successfully read, false otherwise.
 */
bool load_labyrinth_from_file(const std::string& filename, int& M, int& N, 
                              std::vector<std::vector<int>>& LABCOPY, int& X, int& Y) {
  std::ifstream file(filename);
  if (!file.is_open()) {
    std::cerr << "Error: File can't be opened '" << filename << "'.\n";
    return false;
  }

  std::vector<std::vector<int>> temp_grid;
  std::string line;

  while (std::getline(file, line)) {
    // Ignore completely empty lines or lines with just whitespace/carriage returns
    if (line.find_first_not_of(" \t\r\n\v") == std::string::npos) continue;
    
    std::stringstream ss(line);
    std::vector<int> row;
    int val;
    while (ss >> val) {
      row.push_back(val);
    }
    
    if (!row.empty()) {
      temp_grid.push_back(row);
    }
  }
  file.close();

  if (temp_grid.size() < 2) {
    std::cerr << "Error: File doesn't have enough content.\n";
    return false;
  }

  // The last valid line contains X and Y
  std::vector<int> last_row = temp_grid.back();
  temp_grid.pop_back();

  if (last_row.size() != 2) {
    std::cerr << "Error: Last line has to have X e Y.\n";
    return false;
  }

  X = last_row[0];
  Y = last_row[1];

  N = temp_grid.size();
  M = temp_grid[0].size();

  LABCOPY.assign(N, std::vector<int>(M, 0));
  for (int k = 0; k < N; ++k) {
    int j = N - 1 - k;
    for (int i = 0; i < M; ++i) {
      LABCOPY[j][i] = temp_grid[k][i];
    }
  }

  return true;
}

/**
 * @brief Executes the Breadth-First Search (BFS) wave algorithm on the labyrinth.
 * 
 * @param out     Output stream wrapper (TeeStream).
 * @param M       Width of the labyrinth.
 * @param N       Height of the labyrinth.
 * @param LABCOPY Reference to the labyrinth state matrix (updated with wave labels).
 * @param X       Initial X coordinate.
 * @param Y       Initial Y coordinate.
 * @param U       Reference to store the exit X coordinate if found.
 * @param V       Reference to store the exit Y coordinate if found.
 * @return true if a path to the border is found, false otherwise.
 */
bool run_bfs(TeeStream& out, int M, int N, std::vector<std::vector<int>>& LABCOPY, int X, int Y, int& U, int& V) {
  int MN = M * N;
  std::vector<int> FX(MN + 1, 0);
  std::vector<int> FY(MN + 1, 0);
  
  FX[1] = X;
  FY[1] = Y;
  int CLOSE = 1;
  int NEWN = 1;
  bool YES = false;
  U = X; 
  V = Y;

  int CX[4] = {-1, 0, 1, 0};
  int CY[4] = {0, -1, 0, 1};

  out << "PART 2. Trace\n" << std::endl;
  out << "WAVE 0, label L=\"2\". Initial position X=" << X << ", Y=" << Y << ", NEWN=" << NEWN << "\n";

  if (X == 1 || X == M || Y == 1 || Y == N) {
    return true;
  }

  int last_printed_wave = -1;

  while (CLOSE <= NEWN && !YES) {
    int curr_x = FX[CLOSE];
    int curr_y = FY[CLOSE];
    
    int parent_val = LABCOPY[curr_y-1][curr_x-1];
    int next_val = parent_val + 1;
    int wave_index = next_val - 2;

    if (wave_index != last_printed_wave) {
      last_printed_wave = wave_index;
      out << "WAVE " << wave_index << ", label L=\"" << next_val << "\"\n";
    }
        
    out << "    Close CLOSE=" << CLOSE << ", X=" << curr_x << ", Y=" << curr_y << ".\n";
    
    for (int k = 0; k < 4; ++k) {
      int u = curr_x + CX[k];
      int v = curr_y + CY[k];
      
      out << "        R" << k+1 << ". X=" << u << ", Y=" << v << ". ";
      
      if (u < 1 || u > M || v < 1 || v > N) {
        out << "Wall.\n";
        continue;
      }
          
      int cell_val = LABCOPY[v-1][u-1];
      
      if (cell_val == 1) {
        out << "Wall.\n";
      } else if (cell_val >= 2) {
        out << "CLOSED or OPEN.\n";
      } else if (cell_val == 0) {
        LABCOPY[v-1][u-1] = next_val;
        
        if (u == 1 || u == M || v == 1 || v == N) {
          NEWN++;
          FX[NEWN] = u;
          FY[NEWN] = v;
          out << "Free. NEWN=" << NEWN << ". Terminal.\n";
          YES = true;
          U = u;
          V = v;
          break;
        } else {
          NEWN++;
          FX[NEWN] = u;
          FY[NEWN] = v;
          out << "Free. NEWN=" << NEWN << ".\n";
        }
      }
    }
    out << std::endl;
    CLOSE++;
  }
  return YES;
}

/**
 * @brief Reconstructs and prints the final path, rules, and graphical result.
 * 
 * @param out     Output stream wrapper (TeeStream).
 * @param M       Width of the labyrinth.
 * @param N       Height of the labyrinth.
 * @param LABCOPY Constant reference to the solved labyrinth matrix.
 * @param U       Exit X coordinate.
 * @param V       Exit Y coordinate.
 */
void print_results(TeeStream& out, int M, int N, const std::vector<std::vector<int>>& LABCOPY, int U, int V) {
  int CX[4] = {-1, 0, 1, 0};
  int CY[4] = {0, -1, 0, 1};

  out << "\nPART 3. Results\n" << std::endl;
  out << " 3.1. Path is found.\n" << std::endl;
  out << " 3.2. Path graphically:\n" << std::endl;
  out << "LABCOPY\n";
  print_labyrinth(out, LABCOPY, M, N);
  
  std::vector<std::pair<int, int>> path_nodes;
  std::vector<std::string> path_rules;
  path_nodes.push_back({U, V});
  
  int curr_u = U, curr_v = V;
  
  while (LABCOPY[curr_v-1][curr_u-1] > 2) {
    int target_val = LABCOPY[curr_v-1][curr_u-1] - 1;
    for (int k = 0; k < 4; ++k) {
      int next_u = curr_u - CX[k];
      int next_v = curr_v - CY[k];
      
      if (next_u >= 1 && next_u <= M && next_v >= 1 && next_v <= N) {
        if (LABCOPY[next_v-1][next_u-1] == target_val) {
          path_nodes.push_back({next_u, next_v});
          path_rules.push_back("R" + std::to_string(k+1));
          curr_u = next_u;
          curr_v = next_v;
          break;
        }
      }
    }
  }
  
  std::reverse(path_nodes.begin(), path_nodes.end());
  std::reverse(path_rules.begin(), path_rules.end());
  
  out << " 3.3. Rules: ";
  for (size_t i = 0; i < path_rules.size(); ++i) {
    out << path_rules[i] << (i == path_rules.size() - 1 ? "." : ", ");
  }
  out << std::endl;
  out << "\n 3.4. Nodes: ";
  for (size_t i = 0; i < path_nodes.size(); ++i) {
    out << "[X=" << path_nodes[i].first << ",Y=" << path_nodes[i].second << "]";
    out << (i == path_nodes.size() - 1 ? "\n" : ", ");
  }
  out << std::endl;
}

/**
 * @brief Coordinates the overall execution flow of the labyrinth solver.
 * 
 * @param filename Name or path of the input text file.
 */
void solve_labyrinth_from_file(const std::string& filename) {
  int M, N, X, Y;
  std::vector<std::vector<int>> LABCOPY;

  if (!load_labyrinth_from_file(filename, M, N, LABCOPY, X, Y)) {
    return;
  }

  LABCOPY[Y-1][X-1] = 2;

  // Set up the output file
  std::string out_filename = "output_" + filename;
  std::ofstream outfile(out_filename);
  if (!outfile.is_open()) {
    std::cerr << "Error: Output file was not created\n";
    return;
  }

  // Initialize the TeeStream to write to both std::cout and outfile
  TeeStream out(std::cout, outfile);

  out << "\nPART 1. Data\n";
  out << " 1.1. Labyrinth\n" << std::endl;
  print_labyrinth(out, LABCOPY, M, N);
  out << " 1.2. Initial position X=" << X << ", Y=" << Y << ". L=2.\n\n";

  int U = X, V = Y;
  bool success = run_bfs(out, M, N, LABCOPY, X, Y, U, V);

  if (success) {
    print_results(out, M, N, LABCOPY, U, V);
  } else {
    out << " No path found.\n";
  }
  
  outfile.close();
  std::cout << "\n[!] Results in '" << out_filename << "'.\n";
}

/**
 * @brief Main entry point of the program. Accepts input filename via command-line arguments.
 * 
 * @param argc Number of command-line arguments.
 * @param argv Array of command-line arguments.
 * @return int Returns 0 upon successful execution, 1 on error.
 */
int main(int argc, char* argv[]) {
  if (argc < 2) {
    std::cerr << "Uso: " << argv[0] << " <testX.txt>\n";
    return 1;
  }
  
  solve_labyrinth_from_file(argv[1]);

  return 0;
}