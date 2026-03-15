#include "board.h"
#include <cstdlib>
#include <game.h>
#include <iostream>
#include <unistd.h>

void Game::run() {

  renderFrame();
  while (state != GameState::FINISHED) {
    processInput();
    renderFrame();
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
    if (checkWin()) {
      std::cout << "[DEBUG] Game won" << checkWin();
      state = GameState::FINISHED;
    };
    return;
    break;
  case RevealResult::Invalid:
    std::cerr << "[ERROR] Internal game reveal logic failure";
    return;
    break;
  case RevealResult::Mine:
    state = GameState::FINISHED;
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

bool Game::checkWin() {
  std::cout << "[DEBUG] Safe cells:" << board.getRevealedSafeCells() << '\n';
  std::cout << "[DEBUG] Total cells:" << board.getTotalSafeCells() << '\n';
  return board.getRevealedSafeCells() == board.getTotalSafeCells();
}
