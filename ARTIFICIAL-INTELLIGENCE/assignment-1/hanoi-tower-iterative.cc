/** 
 * Vilnius University
 * Artificial Intelligence: Classical Principles
 * Assignment 1: Tower of Hanoi (Iterative)
 * @author Tayri Amador Guillén
 * @date 2026-09-22
 * @brief Iterative solution for the Tower of Hanoi problem using.
 */
#include <iostream>
#include <stack>
#include <vector>
#include <iomanip>

// Global variables
std::stack<int> A;
std::stack<int> B;
std::stack<int> C;

/**
 * @brief Retrieves a reference to the global stack corresponding to a tower.
 * 
 * @param name The character representing the tower's name ('A', 'B', or 'C').
 * @return std::stack<int>& Direct reference to the corresponding global stack.
 */
std::stack<int>& getTower(char name) {
  if (name == 'A') return A;
  if (name == 'B') return B;
  return C;
}

/**
 * @brief Prints the contents of a disk stack from the base to the top.
 * 
 * Uses a std::vector to extract and reverse the elements, ensuring that 
 * the disk at the base of the original stack is printed first.
 * 
 * @param name The character identifying the tower to be printed.
 */
void printTower(char name) {

  std::stack<int> temp = getTower(name); 
  std::vector<int> disks;
  
  while (!temp.empty()) {
    disks.push_back(temp.top());
    temp.pop();
  }
  
  std::cout << name << "=(";
  for (int i = (int)disks.size() - 1; i >= 0; i--) {
    std::cout << disks[i];
    if (i > 0) std::cout << ",";
  }
  std::cout << ")";
}

/**
 * @brief Displays the current state of all three towers on the console.
 */
void printState() {
  printTower('A');
  std::cout << ", ";
  printTower('B');
  std::cout << ", ";
  printTower('C');
}

/**
 * @brief Solves the Tower of Hanoi problem iteratively.
 * 
 * @param n The total number of disks to be moved.
 */
void ht(int n) {
    // Total moves required is 2^n - 1
  int totalMoves = (1 << n) - 1; 
  char pegs[3] = {'A', 'B', 'C'};
  
  // If n is even, move right (+1). If odd, move left (+2 is equivalent to -1 mod 3).
  int dir = (n % 2 == 0) ? 1 : 2; 
  if (n % 2 == 0) {
    std::cout << "Direction for disk: Clockwise" << std::endl;
  } else {
    std::cout << "Direction for disk: Counterclockwise" << std::endl;
  }
  int pos1 = 0; // Tracks the current index (0, 1, or 2) of disk 1

  for (int i = 1; i <= totalMoves; ++i) {
    if (i % 2 != 0) { 
      // ODD: Move only disk 1
      int nextPos = (pos1 + dir) % 3;
      
      getTower(pegs[pos1]).pop();
      getTower(pegs[nextPos]).push(1);
      
      std::cout << std::setw(4) << i << ". Move disk 1 from " << pegs[pos1] << " to " << pegs[nextPos] << ". ";
      
      pos1 = nextPos; // Update disk 1 position
    } 
    else { 
      // EVEN: Make the only legal move between the other two towers
      int p2 = (pos1 + 1) % 3;
      int p3 = (pos1 + 2) % 3;
      
      int from, to, disk;
      
      // 1. If peg p2 is empty, the move must be from p3 to p2
      if (getTower(pegs[p2]).empty()) {
        from = p3; 
        to = p2; 
        disk = getTower(pegs[p3]).top();
      } 
      // 2. If peg p3 is empty, the move must be from p2 to p3
      else if (getTower(pegs[p3]).empty()) {
        from = p2; 
        to = p3; 
        disk = getTower(pegs[p2]).top();
      } 
      // 3. If neither is empty, compare their top disks
      else {
        int top2 = getTower(pegs[p2]).top();
        int top3 = getTower(pegs[p3]).top();
          
          // The smaller disk is moved onto the larger one
        if (top2 < top3) {
          from = p2; to = p3; disk = top2;
        } else {
          from = p3; to = p2; disk = top3;
        }
      }
      getTower(pegs[from]).pop();
      getTower(pegs[to]).push(disk);
      
      std::cout << std::setw(4) << i << ". Move disk " << disk << " from " << pegs[from] << " to " << pegs[to] << ". ";
    }
    
    printState();
    std::cout << std::endl;
  }
  
  std::cout << "Total moves: " << totalMoves << std::endl;
}
/**
 * @brief Main entry point of the program.
 * 
 * @return int Returns 0 if execution was successful, or 1 on validation error.
 */
int main() {
  std::cout << "Enter the number of disks; n = ";
  int n = 0;
  std::cin >> n;
  
  std::cout << "n = " << n << std::endl;
  if (n < 1 || n > 10) {
    std::cout << "Error: n must be between 1 and 10." << std::endl;
    return 1;
  }
  
  // Initialize tower A with n disks
  for (int i = n; i >= 1; i--) {
    A.push(i);
  }
  
  std::cout << "Initial state ";
  printState();
  std::cout << std::endl;

  ht(n);
  
  return 0;
}