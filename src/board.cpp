#include "board.h"
#include <cstddef>
#include <iostream>
#include <ostream>

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
  std::string color = "\033[1;33m";
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
  std::string color = "\033[1;33m";
  std::string reset = "\033[0m";

  if (rowActive && cursor.x == 0)
    buf << color;
  buf << "│";
  if (rowActive && cursor.x == 0)
    buf << reset;

  for (size_t c = 0; c < cols; ++c) {

    buf << "   ";

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
  std::string color = "\033[1;33m";
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
  std::string color = "\033[1;33m";
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
