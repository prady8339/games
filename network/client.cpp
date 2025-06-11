
#include <cstdlib>
#include <enet/enet.h>
#include <iostream>

int main() {
  if (enet_initialize() != 0) {
    std::cerr << "Failed to initialize ENet.\n";
    return EXIT_FAILURE;
  }
  atexit(enet_deinitialize);

  ENetHost *client = enet_host_create(nullptr, 1, 0, 0);
  if (client == nullptr) {
    std::cerr << "Failed to create client.\n";
    return EXIT_FAILURE;
  }

  ENetAddress address;
  enet_address_set_host(&address, "127.0.0.1");
  address.port = 1234;

  ENetPeer *peer = enet_host_connect(client, &address, 0);
  if (peer == nullptr) {
    std::cerr << "No available peers for connection.\n";
    enet_host_destroy(client);
    return EXIT_FAILURE;
  }

  ENetEvent event;
  if (enet_host_service(client, &event, 5000) > 0 &&
      event.type == ENET_EVENT_TYPE_CONNECT) {
    std::cout << "Connected to server.\n";

    const char *msg = "Hello from client!";
    ENetPacket *packet =
        enet_packet_create(msg, strlen(msg) + 1, ENET_PACKET_FLAG_RELIABLE);
    enet_peer_send(peer, 0, packet);
    enet_host_flush(client);
  } else {
    std::cerr << "Failed to connect to server.\n";
    enet_peer_reset(peer);
  }

  enet_host_destroy(client);
  return EXIT_SUCCESS;
}
