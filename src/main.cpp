#include "board.h"
#include <cstdlib>
#include <iostream>
#include <ostream>
#include <sys/ioctl.h>
#include <termios.h>

#include <unistd.h>

static int exit_loop = 1;

static struct termios old_termios, new_termios;

void enableRawMode() {
  tcgetattr(STDIN_FILENO, &old_termios);
  new_termios = old_termios;               // copy the termios state
  new_termios.c_lflag &= ~(ICANON | ECHO); // put terminal into raw mode by
                                           // disabling canonical input and echo
  tcsetattr(STDIN_FILENO, TCSAFLUSH, &new_termios);
}

void resetTerminal() {
  std::cout << "\e[m" << "\e[?25h" << std::endl;
  tcsetattr(STDIN_FILENO, TCSAFLUSH, &old_termios);
}

void configureTerminal() {
  enableRawMode();
  std::cout << "\e[?25l"; // hide cursor
  atexit(resetTerminal);
}
int main(int argc, char *argv[]) {
  struct winsize w;
  ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
  // std::cout << "lines\n" << std::setprecision(2) << w.ws_row << '\n';
  // std::cout << "columns \n" << w.ws_col << '\n';

  Board board(Difficulty::Expert);
  board.render();
  while (exit_loop) {
  }
  return 0;
}
