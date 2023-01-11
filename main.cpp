#include "CLI.h"
#include "defaultIO.h"

int main() {
    DefaultIO* io = new StandardIO();
    CLI cli(io);
    cli.start();
    return 0;
}