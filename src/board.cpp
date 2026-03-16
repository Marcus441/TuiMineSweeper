#include "board.h"
#include <cstddef>
#include <iostream>
#include <ostream>
#include <random>

void Board::render(std::ostream &buf) {
  // Move terminal cursor to Home (0,0)
  buf << "\033[H";

  renderTopBorder(buf);

  for (size_t r = 0; r < rows; ++r) {
    renderCellRow(buf, r);

    if (r < rows - 1) {
      renderRowDivider(buf, r);
    }
  }

  renderBottomBorder(buf);

  // Final flush to ensure the frame is sent to the terminal
  buf << std::flush;
}

void Board::moveCursor(int dx, int dy) {
  cursor.x = std::clamp(cursor.x + dx, 0, (int)cols - 1);
  cursor.y = std::clamp(cursor.y + dy, 0, (int)rows - 1);
}

void Board::renderTopBorder(std::ostream &buf) {
  bool rowActive = (cursor.y == 0);
  std::string color = "\033[1;32m";
  std::string reset = "\033[0m";

  if (rowActive && cursor.x == 0)
    buf << color;
  buf << "┌";
  if (rowActive && cursor.x == 0)
    buf << reset;

  for (size_t c = 0; c < cols; ++c) {
    bool isCurrentCell = (rowActive && (int)c == cursor.x);

    if (isCurrentCell)
      buf << color;
    buf << "───";
    if (isCurrentCell)
      buf << reset;

    bool highlightJoint =
        (rowActive && ((int)c == cursor.x || (int)c == cursor.x - 1));
    if (highlightJoint)
      buf << color;
    buf << (c == cols - 1 ? "┐" : "┬");
    buf << reset;
  }
  buf << "\n";
}

void Board::renderCellRow(std::ostream &buf, size_t rowIdx) {
  bool rowActive = (cursor.y == (int)rowIdx);
  std::string color = "\033[1;32m";
  std::string red = "\033[31m";
  std::string errorRed = "\x1B[1;31m";
  std::string reset = "\033[0m";

  if (rowActive && cursor.x == 0)
    buf << color;
  buf << "│";
  if (rowActive && cursor.x == 0)
    buf << reset;

  for (size_t c = 0; c < cols; ++c) {

    const Cell &cell = internalGetCell(c, rowIdx);
    // bool isCursor = (rowActive && (int)c == cursor.x);

    // if (isCursor) {
    //   buf << color << " * " << reset;
    // } else
    if (!cell.isRevealed) {
      if (cell.isFlagged)
        buf << red << " F " << reset;
      else
        buf << " • ";
    } else if (cell.isMine) {
      buf << errorRed << " X " << reset;
    } else if (cell.neighborMines == 0) {
      buf << "   ";
    } else {
      buf << " " << cell.neighborMines << " ";
    }

    bool highlighted =
        (rowActive && ((int)c == cursor.x || (int)c == cursor.x - 1));

    if (highlighted)
      buf << color;
    buf << "│";
    buf << reset;
  }
  buf << "\n";
}

void Board::renderRowDivider(std::ostream &buf, size_t rowIdx) {
  bool isCursorRowLine =
      (cursor.y == (int)rowIdx || cursor.y == (int)rowIdx + 1);
  std::string color = "\033[1;32m";
  std::string reset = "\033[0m";

  if (isCursorRowLine && cursor.x == 0)
    buf << color;
  buf << "├";
  if (isCursorRowLine && cursor.x == 0)
    buf << reset;

  for (size_t c = 0; c < cols; ++c) {
    bool isCursorCol = ((int)c == cursor.x);

    if (isCursorRowLine && isCursorCol)
      buf << color;
    buf << "───";
    buf << reset;

    bool highlightJoint =
        (isCursorRowLine && (isCursorCol || (int)c == cursor.x - 1));

    if (highlightJoint)
      buf << color;
    buf << (c == cols - 1 ? "┤" : "┼");
    buf << reset;
  }
  buf << "\n";
}

void Board::renderBottomBorder(std::ostream &buf) {
  bool rowActive = (cursor.y == (int)rows - 1);
  std::string color = "\033[1;32m";
  std::string reset = "\033[0m";

  if (rowActive && cursor.x == 0)
    buf << color;
  buf << "└";
  if (rowActive && cursor.x == 0)
    buf << reset;

  for (size_t c = 0; c < cols; ++c) {
    bool isCurrentCell = (rowActive && (int)c == cursor.x);

    if (isCurrentCell)
      buf << color;
    buf << "───";
    buf << reset;

    bool highlightJoint =
        (rowActive && ((int)c == cursor.x || (int)c == cursor.x - 1));

    if (highlightJoint)
      buf << color;
    buf << (c == cols - 1 ? "┘" : "┴");
    buf << reset;
  }
  buf << "\n";
  buf << "\n";
}

void Board::generateMines(int startX, int startY) {
  std::vector<int> pool;
  for (size_t i = 0; i < rows * cols; ++i) {
    int x = i % cols;
    int y = i / cols;
    // Skip the 3x3 area around the first click
    if (std::abs(x - startX) <= 1 && std::abs(y - startY) <= 1)
      continue;
    pool.push_back(i);
  }
  std::shuffle(pool.begin(), pool.end(), std::mt19937{std::random_device{}()});

  for (size_t i = 0; i < mines && i < pool.size(); ++i) {
    grid[pool[i]].isMine = true;
  }
  total_safe_cells = (rows * cols) - mines;
  calculateNumbers();
}

void Board::calculateNumbers() {
  for (size_t y = 0; y < rows; ++y) {
    for (size_t x = 0; x < cols; ++x) {
      if (internalGetCell(x, y).isMine)
        continue;

      int count = 0;
      // The 8-neighbor loop
      for (int dy = -1; dy <= 1; ++dy) {
        for (int dx = -1; dx <= 1; ++dx) {
          int nx = x + dx;
          int ny = y + dy;
          if (isValid(nx, ny) && internalGetCell(nx, ny).isMine) {
            count++;
          }
        }
      }
      internalGetCell(x, y).neighborMines = count;
    }
  }
}

RevealResult Board::reveal(int x, int y) {
  // Safety checks: Is it on the board? Is it already revealed or flagged?
  if (!isValid(x, y))
    return RevealResult::Invalid;
  Cell &cell = internalGetCell(x, y);
  if (cell.isRevealed || cell.isFlagged)
    return RevealResult::Invalid;

  cell.isRevealed = true;

  if (cell.isMine)
    return RevealResult::Mine;

  // recursively reveal all 8 neighbors
  if (cell.neighborMines == 0) {
    for (int dy = -1; dy <= 1; ++dy) {
      for (int dx = -1; dx <= 1; ++dx) {
        reveal(x + dx, y + dy);
      }
    }
  }
  ++revealed_safe_cells;
  return RevealResult::Safe;
}

void Board::revealAllMines() {
  for (size_t i = 0; i < rows * cols; ++i) {
    Cell &cell = grid[i];

    if (cell.isMine && !cell.isFlagged) {
      cell.isRevealed = true;
    } else if (cell.isFlagged) {
      cell.isRevealed = true;
    }
  }
}

void Board::initialize() { generateMines(cursor.x, cursor.y); }

void Board::toggleFlag() {
  Cell &c = internalGetCell(cursor.x, cursor.y);
  if (!c.isRevealed) {
    c.isFlagged = !c.isFlagged;
  }
}
