#include <cstddef>
enum class Difficulty { Beginner, Intermediate, Expert, Custom };

struct Point {
  int x, y;
};

struct Board {
  size_t rows;
  size_t cols;
  size_t mines;

  Board(Difficulty d);
  Point cursor = {0, 0};

  void render();
  void moveCursor(int dx, int dy);
};
