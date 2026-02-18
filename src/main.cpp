#include "board.h"
#include "terminal.h"

#include <cstdlib>
#include <iostream>
#include <sys/ioctl.h>
#include <termios.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
  struct winsize w;
  ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
  // std::cout << "lines\n" << std::setprecision(2) << w.ws_row << '\n';
  // std::cout << "columns \n" << w.ws_col << '\n';

  bool exit_loop = false;

  Board board(Difficulty::Expert);
  board.render();

  TerminalHandler term;

  while (!exit_loop) {
    char c;
    if (read(STDIN_FILENO, &c, 1) == 1) {
      if (c == 'w')
        board.moveCursor(0, -1);
      if (c == 's')
        board.moveCursor(0, 1);
      if (c == 'a')
        board.moveCursor(-1, 0);
      if (c == 'd')
        board.moveCursor(1, 0);

      board.render();
      std::cout << "\r[DEBUG] [" << c << " : " << (int)c << "] " << std::flush;
    };
  }
  return 0;
}
