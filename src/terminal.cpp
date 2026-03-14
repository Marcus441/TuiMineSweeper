#include "terminal.h"

#include <iostream>
#include <ostream>
#include <sys/ioctl.h>
#include <unistd.h>

void TerminalHandler::enableRawMode() {
  tcgetattr(STDIN_FILENO, &old_termios);
  new_termios = old_termios;               // copy the termios state
  new_termios.c_lflag &= ~(ICANON | ECHO); // put terminal into raw mode by
                                           // disabling canonical input and echo
  tcsetattr(STDIN_FILENO, TCSAFLUSH, &new_termios);
}

void TerminalHandler::resetTerminal() {
  std::cout << "\e[m" << "\e[?25h" << std::endl;
  tcsetattr(STDIN_FILENO, TCSAFLUSH, &old_termios);
}

TerminalHandler::~TerminalHandler() { this->resetTerminal(); }

TerminalHandler::TerminalHandler() {
  enableRawMode();
  // move cursor to home and clear term
  std::cout << "\033[2J\033[H"
            << "\e[?25l" << std::flush; // hide cursor
}
