#include "board.h"
#include <iostream>

Board::Board(Difficulty d) {
  switch (d) {
  case Difficulty::Beginner:
    rows = 9;
    cols = 9;
    mines = 10;
    break;
  case Difficulty::Intermediate:
    rows = 16;
    cols = 16;
    mines = 40;
    break;
  case Difficulty::Expert:
    rows = 16;
    cols = 30;
    mines = 99;
    break;
  default:
    rows = 9;
    cols = 9;
    mines = 10;
  }
};

void Board::render() {
  for (size_t r = 0; r < rows; ++r) {
    for (size_t c = 0; c < cols; ++c) {
      if ((r == 0 || r == rows - 1) && (c == 0 || c == cols - 1))
        std::cout << '+'; // Corners
      else if (r == 0 || r == rows - 1)
        std::cout << '-'; // Top/Bottom walls
      else if (c == 0 || c == cols - 1)
        std::cout << '|'; // Side walls
      else {
        std::cout << 'o';
      }
    }
    std::cout << '\n';
  }
}
