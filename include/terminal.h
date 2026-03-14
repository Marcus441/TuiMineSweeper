#pragma once

#include <sys/ioctl.h>
#include <termios.h>

class TerminalHandler {
public:
  TerminalHandler();
  ~TerminalHandler();

  // Disable copying
  TerminalHandler(const TerminalHandler &) = delete;
  TerminalHandler &operator=(const TerminalHandler &) = delete;

private:
  struct termios old_termios, new_termios;
  void enableRawMode();
  void resetTerminal();
};
