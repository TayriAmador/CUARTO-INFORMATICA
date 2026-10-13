/** 
 *  Vilnius University
 *  Artificial Intelligence: Classical Principles
 *  Assignment 2: Knights Tour Solver 
 *  @author Tayri Amador Guillén
 *  @date 2026-09-29
 *  @brief This program finds a Knight's Tour on an NxN chessboard using backtracking.
 */

#include <iostream>
#include <iomanip>
#include <fstream>
#include <string> 
#include <vector>

int N;

std::vector<std::vector<int>> board;
long long trace_count = 0;
bool found = false;

std::ofstream foutShort;
std::ofstream foutLong;

int CX[8] = {2, 1, -1, -2, -2, -1, 1, 2};
int CY[8] = {1, 2, 2, 1, -1, -2, -2, -1};

/**
 * @brief Prints a message to the standard output and saves it in both output files.
 * 
 * @param s Constant reference to the string containing the text to print.
 */
void printShort(const std::string &s) {
  std::cout << s << std::endl;
  foutShort << s << std::endl;
  foutLong << s << std::endl;
}

/**
 * @brief Attempts to find the Knight's Tour using a recursive backtracking algorithm.
 * 
 * Evaluates all 8 possible knight moves from the current position. If a move is valid 
 * it marks the board and recursively calls itself for the next move. If a dead-end is reached, 
 * it backtracks by unmarking the board.
 * 
 * @param L The current move number (starts at 2, up to N*N).
 * @param x The current X-coordinate of the knight.
 * @param y The current Y-coordinate of the knight.
 * @return true if a complete path is successfully found.
 * @return false if the current path ends without finding a solution.
 */

bool try_move(int L, int x, int y) {
  for (int k = 0; k < 8; k++) {
    int u = x + CX[k];
    int v = y + CY[k];

    trace_count++;

    std::string guiones(L - 2, '-');
    foutLong << "\n" << std::setw(8) << trace_count << ") " << guiones 
              << "R" << (k + 1) 
              << ". U=" << (u + 1) << ", V=" << (v + 1) 
              << ". L=" << L << ". ";
  
    // CASE A: Out of the board
    if (u < 0 || u >= N || v < 0 || v >= N) { 
      foutLong << "Out.";
      if (k == 7) foutLong << " Backtrack.";
      continue; 
    }
    
    // CASE B: It was already visited
    if (board[u][v] != 0) { 
      foutLong << "Thread.";
      if (k == 7) foutLong << " Backtrack.";
      continue; 
    }
    
    // CASE C: Valid movement
    foutLong << "Free. BOARD[" << (u + 1) << "," << (v + 1) << "]:=" << L << ".";
    board[u][v] = L;

    // Victory condition
    if (L == N * N) { 
      found = true;
      return true;
    }

    if (try_move(L + 1, u, v)) {
      return true;
    }
    
    board[u][v] = 0;
  }
  return false;
}


int main(int argc, char* argv[]) {
  if (argc < 3) {
    std::cerr << "Usage: " << argv[0] << " <out-short.txt> <out-long.txt>" << std::endl;
    return 1;
  }
  
  foutShort.open(argv[1]);
  foutLong.open(argv[2]);

  int x, y;
  std::cout << "Board size N: ";
  std::cin >> N;
  std::cout << "Start X: ";
  std::cin >> x;
  std::cout << "Start Y: ";
  std::cin >> y;

  board.assign(N, std::vector<int>(N, 0));
  board[x - 1][y - 1] = 1;

  // PART 1. Data

  printShort("PART 1. Data");
  printShort(" 1) Board: " + std::to_string(N) + "x" + std::to_string(N) + ".");
  printShort(" 2) Initial position: X=" + std::to_string(x) + ", Y=" + std::to_string(y) + ". L=1.\n");

  // PART 2. Trace 
  foutLong << "PART 2. Trace";
  
  try_move(2, x - 1, y - 1);

  // PART 3. Results

  printShort("\n\nPART 3. Results");
  
  if (found) {
    printShort("  1) Path is found. Trials=" + std::to_string(trace_count) + ".");
    printShort("  2) Path graphically:");

    // Calculamos el ancho dinámico basado en el número máximo (N*N)
    int width = std::to_string(N * N).length();

    printShort("  Y, V ^");

    // Imprimimos fila por fila directamente a los 3 canales usando setw
    for (int row = N; row >= 1; row--) {
      std::cout << std::setw(6) << row << " |";
      foutShort << std::setw(6) << row << " |";
      foutLong  << std::setw(6) << row << " |";
      
      for (int col = 1; col <= N; col++) {
        std::cout << std::setw(width + 1) << board[col - 1][row - 1];
        foutShort << std::setw(width + 1) << board[col - 1][row - 1];
        foutLong  << std::setw(width + 1) << board[col - 1][row - 1];
      }
      
      std::cout << std::endl;
      foutShort << std::endl;
      foutLong  << std::endl;
    }
    
    // Eje X adaptado al ancho dinámico
    std::string separador = "        " + std::string(N * (width + 1) + 1, '-') + "> X, U";
    printShort(separador);
    
    std::cout << "        ";
    foutShort << "        ";
    foutLong  << "        ";
    
    for (int col = 1; col <= N; col++) {
      std::cout << std::setw(width + 1) << col;
      foutShort << std::setw(width + 1) << col;
      foutLong  << std::setw(width + 1) << col;
    }
    
    std::cout << std::endl;
    foutShort << std::endl;
    foutLong  << std::endl;

  } else {
    printShort(" 1) No tour. Trials=" + std::to_string(trace_count) + ".");
  }

  foutShort.close();
  foutLong.close();
  
  return 0;
}