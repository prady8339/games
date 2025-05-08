#include <enet/enet.h>
#include <iostream>
#include <stdio.h>

int main(int argc, char **argv) {
  if (enet_initialize() != 0) {
    fprintf(stderr, "An error occurred while initializing ENet.\n");
    return EXIT_FAILURE;
  }
  std::cout << "test test and rest";
  atexit(enet_deinitialize);
}
