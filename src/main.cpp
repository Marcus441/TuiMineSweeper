#include "board.h"
#include "game.h"
#include "terminal.h"

#include <sys/ioctl.h>
#include <termios.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
  struct winsize w;
  ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);

  TerminalHandler term;
  Game game = Game();
  game.run();

  return 0;
}
