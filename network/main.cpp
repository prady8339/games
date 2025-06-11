#include <cstdlib>
#include <cstring>
#include <enet/enet.h>
#include <iostream>

void run_server() {
  ENetAddress address;
  address.host = ENET_HOST_ANY;
  address.port = 1234;

  ENetHost *server = enet_host_create(&address, 32, 0, 0);
  if (!server) {
    std::cerr << "Failed to create server.\n";
    exit(EXIT_FAILURE);
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
}

void run_client() {
  ENetHost *client = enet_host_create(nullptr, 1, 0, 0);
  if (!client) {
    std::cerr << "Failed to create client.\n";
    exit(EXIT_FAILURE);
  }

  ENetAddress address;
  enet_address_set_host(&address, "127.0.0.1");
  address.port = 1234;

  ENetPeer *peer = enet_host_connect(client, &address, 0);
  if (!peer) {
    std::cerr << "No available peers for connection.\n";
    enet_host_destroy(client);
    exit(EXIT_FAILURE);
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
    while (true) {
      std::string s;
      std::getline(std::cin, s);
      ENetPacket *packet = enet_packet_create(s.c_str(), s.size() + 1,
                                              ENET_PACKET_FLAG_RELIABLE);
      enet_peer_send(peer, 0, packet);
      enet_host_flush(client);
    }
  } else {
    std::cerr << "Failed to connect to server.\n";
    enet_peer_reset(peer);
  }

  enet_host_destroy(client);
}

int main(int argc, char **argv) {
  if (enet_initialize() != 0) {
    std::cerr << "Failed to initialize ENet.\n";
    return EXIT_FAILURE;
  }
  atexit(enet_deinitialize);

  if (argc < 2) {
    std::cerr << "Usage: ./app [server|client]\n";
    return EXIT_FAILURE;
  }

  if (strcmp(argv[1], "server") == 0) {
    run_server();
  } else if (strcmp(argv[1], "client") == 0) {
    run_client();
  } else {
    std::cerr << "Unknown mode: " << argv[1] << "\n";
    std::cerr << "Usage: ./app [server|client]\n";
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
