#include "board.h"
#include <game.h>
#include <iostream>
#include <unistd.h>

void Game::run() {

  while (state != GameState::FINISHED) {
    renderFrame();
    processInput();
  }
}
void Game::processInput() {
  char c;
  if (read(STDIN_FILENO, &c, 1) == 1) {
    if (c == 'w')
      board.moveCursor(0, -1);
    if (c == 'a')
      board.moveCursor(-1, 0);
    if (c == 's')
      board.moveCursor(0, 1);
    if (c == 'd')
      board.moveCursor(1, 0);

    else if (c == ' ') {
      updateLogic();
    }

    else if (c == 'f') {
      board.toggleFlag();
    }

    else if (c == 'q') {
      state = GameState::FINISHED;
    }
    // std::cout << "\r[DEBUG] [" << c << " : " << (int)c << "] "
    //           << "BUFF SIZE: " << buf.view().size() << std::flush;
  };
}

void Game::updateLogic() {

  const Point &cursor = board.getCursor();
  const Cell &cell = board.getCell(cursor.x, cursor.y);

  if (state == GameState::BEGIN) {
    board.initialize();
    state = GameState::PLAYING;
  }

  if (cell.isFlagged)
    return;

  RevealResult outcome = board.reveal(cursor.x, cursor.y);
  switch (outcome) {
  case RevealResult::Safe:
    return;
    break;
  case RevealResult::Invalid:
    std::cerr << "[ERROR] Internal game reveal logic failure";
    return;
    break;
  case RevealResult::Mine:
    // handleLoss();
    return;
    break;
  }
};

void Game::renderFrame() {
  frameBuffer.str("");
  frameBuffer.clear();

  board.render(frameBuffer);
  std::cout << frameBuffer.view() << std::flush;
};
