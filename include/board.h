#pragma once

#include <cstddef>
#include <ostream>
#include <vector>

enum class RevealResult { Invalid, Safe, Mine };

enum class Difficulty { Beginner, Intermediate, Expert, Custom };

struct Point {
  int x, y;
};

struct Cell {
  bool isMine = false;
  bool isRevealed = false;
  bool isFlagged = false;
  int neighborMines = 0;
};

class Board {
public:
  Board() : Board(Difficulty::Beginner) {}

  Board(Difficulty d) {
    switch (d) {
    case Difficulty::Beginner:
      setup(9, 9, 10);
      break;
    case Difficulty::Intermediate:
      setup(16, 16, 40);
      break;
    case Difficulty::Expert:
      setup(16, 30, 99);
      break;
    default:
      setup(9, 9, 10);
      break;
    }
  }

  void render(std::ostream &buf);
  void moveCursor(int dx, int dy);
  void initialize();
  void toggleFlag();
  RevealResult reveal(int x, int y);

  const Point &getCursor() const { return cursor; }
  const Cell &getCell(int x, int y) const { return grid.at(y * cols + x); }

private:
  size_t rows;
  size_t cols;
  size_t mines;
  Point cursor = {0, 0};
  std::vector<Cell> grid;

  void setup(size_t r, size_t c, size_t m) {
    rows = r;
    cols = c;
    mines = m;
    grid.assign(rows * cols, Cell{});
  }

  // Utility
  bool isValid(int x, int y) const {
    return x >= 0 && x < (int)cols && y >= 0 && y < (int)rows;
  }

  // Renderer
  void renderTopBorder(std::ostream &buf);
  void renderRowDivider(std::ostream &buf, size_t rowIdx);
  void renderCellRow(std::ostream &buf, size_t rowIdx);
  void renderBottomBorder(std::ostream &buf);

  // Board fill
  void generateMines(int startX, int startY);
  void calculateNumbers();
  Cell &internalGetCell(int x, int y) { return grid.at(y * cols + x); }
};
