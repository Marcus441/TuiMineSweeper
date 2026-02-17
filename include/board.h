#include <cstddef>
enum class Difficulty { Beginner, Intermediate, Expert, Custom };

struct Board {
  size_t rows;
  size_t cols;
  size_t mines;
  void render();
  Board(Difficulty d);
};
