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

void Board::render(std::ostream &buf) {
  buf << "\033[H"; // Home
  for (size_t r = 0; r < rows; ++r) {
    for (size_t c = 0; c < cols; ++c) {
      if ((int)r == cursor.y && (int)c == cursor.x)
        buf << "*";
      else
        buf << "O";
    }
    buf << "\n";
  }
}

void Board::moveCursor(int dx, int dy) {
  cursor.x = std::clamp(cursor.x + dx, 1, (int)cols - 1);
  cursor.y = std::clamp(cursor.y + dy, 1, (int)rows - 1);
}
