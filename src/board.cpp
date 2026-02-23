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
  buf << "\033[H";
  // 1. TOP BORDER
  buf << "┌";
  for (size_t c = 0; c < cols; ++c) {
    buf << "───" << (c == cols - 1 ? "┐" : "┬");
  }
  buf << "\n";

  for (size_t r = 0; r < rows; ++r) {
    // 2. CELL CONTENT ROW
    buf << "│";
    for (size_t c = 0; c < cols; ++c) {
      bool isCursor = (r == cursor.y && c == cursor.x);

      if (isCursor)
        buf << " * ";
      else
        buf << " • ";

      buf << "│";
    }
    buf << "\n";

    // 3. ROW DIVIDER (Don't draw after the last row)
    if (r < rows - 1) {
      buf << "├";
      for (size_t c = 0; c < cols; ++c) {
        buf << "───" << (c == cols - 1 ? "┤" : "┼");
      }
      buf << "\n";
    }
  }

  // 4. BOTTOM BORDER
  buf << "└";
  for (size_t c = 0; c < cols; ++c) {
    buf << "───" << (c == cols - 1 ? "┘" : "┴");
  }
  buf << "\n";
}

void Board::moveCursor(int dx, int dy) {
  cursor.x = std::clamp(cursor.x + dx, 0, (int)cols - 1);
  cursor.y = std::clamp(cursor.y + dy, 0, (int)rows - 1);
}
