#pragma once
#include <board.h>
#include <sstream>

enum class GameState { BEGIN, PLAYING, PAUSED, LOST, FINISHED };

class Game {
public:
  Game(Difficulty d) {
    frameBuffer.str("");
    board = Board(d);
  };
  Game(Game &&) = default;
  Game(const Game &) = delete;
  Game &operator=(Game &&) = default;
  Game &operator=(const Game &) = delete;

  void run();

private:
  std::stringstream frameBuffer;
  Board board;
  GameState state = GameState::BEGIN;

  void processInput();
  void updateLogic();
  void renderFrame();

  void handleWin();
  void handleLoss();
};
