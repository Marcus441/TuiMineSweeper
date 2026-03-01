#include "board.h"
#include "terminal.h"

#include <cstdlib>
#include <iostream>
#include <sstream>
#include <sys/ioctl.h>
#include <termios.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
  struct winsize w;
  ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
  TerminalHandler term;
  // std::cout << "lines\n" << std::setprecision(2) << w.ws_row << '\n';
  // std::cout << "columns \n" << w.ws_col << '\n';

  bool exit_loop = false;

  Board board(Difficulty::Expert);
  std::stringstream buf;
  board.render(buf);

  while (!exit_loop) {
    buf.str("");
    buf.clear();

    board.render(buf);
    std::cout << buf.view() << std::flush;
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
        board.handleAction();
      }

      else if (c == 'f') {
        board.toggleFlag();
      }

      else if (c == 'q') {
        exit_loop = true;
      }
      // std::cout << "\r[DEBUG] [" << c << " : " << (int)c << "] "
      //           << "BUFF SIZE: " << buf.view().size() << std::flush;
    };
  }
  return 0;
}
