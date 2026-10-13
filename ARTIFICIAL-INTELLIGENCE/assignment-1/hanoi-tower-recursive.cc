/** Vilnius University
  * Artificial Intelligence: Classical Principles
  * Assignment 1: Tower of Hanoi (Recursive)
  * @author Tayri Amador Guillén
  * @date 2026-09-15
  * @brief Recursive solution for the Tower of Hanoi problem. 
*/
#include <iostream>
#include <stack>
#include <iomanip>

//Towers
std::stack<int> A;
std::stack<int> B;
std::stack<int> C;

int moveCounter= 0;

/**
 * @brief Prints the contents of a disk stack from the base to the top.
 * Uses a temporary stack to reverse the order of the elements.
 * 
 * @param stackCopy Copy of the stack to be printed. 
 */

void printStack(std::stack<int> stackCopy) {
  std::stack<int> temp;

  while(!stackCopy.empty()) {
    temp.push(stackCopy.top());
    stackCopy.pop();
  }
  std::cout << "(";
  bool primero = true;
  while (!temp.empty()) {
    if (!primero) std::cout << ",";
    std::cout << temp.top();
    temp.pop();
    primero = false;
  }
  std::cout << ")";
}

/**
 * @brief Displays the current state of the three towers.
 */

void printState() {
  std::cout << "A="; 
  printStack(A);
  std::cout << ", B="; 
  printStack(B);
  std::cout << ", C="; 
  printStack(C);
}

/**
 * @brief Identifies the tower
 * @param towerName Represents the tower's name.
 * @return std::stack<int>& Reference to the corresponding global stack.
 */
std::stack<int>& getTower(char towerName) {
  if (towerName == 'A') return A;
  if (towerName == 'B') return B;
  return C;
}

/**
 * @brief Solves the Tower of Hanoi problem recursively.
 * 
 * Moves n disks from the source tower to the destination tower, 
 * using an intermediary tower.
 * 
 * @param x Identifies the source tower.
 * @param y Identifies the intermediary tower.
 * @param z Identifies the destination tower.
 * @param n The number of disks to be moved.
 */
void ht(char x, char y, char z, int n) {
  
  if (n > 0) {
    ht(x, z, y, n-1); 

    moveCounter++;
    int disk = getTower(x).top();
    getTower(x).pop();
    getTower(z).push(disk);
    
    std::cout << std::setw(4) << moveCounter << ". Move disk " << disk << " from " << x << " to " << z << ". ";
    printState();
    std::cout << std::endl;
    
    ht(y, x, z, n-1); 
  }
}

int main() {
  std::cout << "Enter the number of disks; n = ";
  int n = 0;
  std::cin >> n;
  
  std::cout << "n = " << n << std::endl;
  if (n < 1 || n > 10) {
    std::cout << "Error: n must be between 1 and 10." << std::endl;
    return 1;
  }
  
  for (int i = n; i >= 1; i--) {
    A.push(i); 
  }
  
  std::cout << "Initial state A=";
  printStack(A);
  std::cout << ", B=(), C=()" << std::endl;
  
  ht('A', 'B', 'C', n);
  
  std::cout << "Total moves: " << moveCounter << std::endl;
  return 0;
}