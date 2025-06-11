
#include <cstdlib>
#include <enet/enet.h>
#include <iostream>

int main() {
  if (enet_initialize() != 0) {
    std::cerr << "Failed to initialize ENet.\n";
    return EXIT_FAILURE;
  }
  atexit(enet_deinitialize);

  ENetAddress address;
  address.host = ENET_HOST_ANY; // Listen on all interfaces
  address.port = 1234;

  ENetHost *server = enet_host_create(&address, 32, 0, 0);
  if (server == nullptr) {
    std::cerr << "Failed to create server.\n";
    return EXIT_FAILURE;
  }
  std::cout << "Server started on port 1234.\n";

  ENetEvent event;
  while (true) {
    while (enet_host_service(server, &event, 1000) > 0) {
      switch (event.type) {
      case ENET_EVENT_TYPE_CONNECT:
        std::cout << "Client connected.\n";
        break;

      case ENET_EVENT_TYPE_RECEIVE:
        std::cout << "Message received: " << event.packet->data << "\n";
        enet_packet_destroy(event.packet);
        break;

      case ENET_EVENT_TYPE_DISCONNECT:
        std::cout << "Client disconnected.\n";
        break;

      default:
        break;
      }
    }
  }

  enet_host_destroy(server);
  return EXIT_SUCCESS;
}
