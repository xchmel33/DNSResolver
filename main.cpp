#include "config.h"
#include "dns.h"

int main(int argc, char* argv[]) {
    // Parse arguments to get server IP, port, and other options
    Config config = parseArguments(argc, argv);

    // Create a UDP socket for DNS communication
    int sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0) {
        cerr << "Failed to create socket." << endl;
        return 1;
    }

    // Define the address of the DNS server for communication
    sockaddr_in serverAddr = {};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(config.port);  // Use port from arguments or default to 53
    serverAddr.sin_addr.s_addr = inet_addr(config.serverIP.c_str());  // Use server IP from arguments

    // Generate a DNS query based on the given domain and recursion option
    auto query = createQuery(config.address, config.recursion);  // Use address from arguments

    // Send the DNS query to the server using the socket
    sendto(sock, query.data(), query.size(), 0, reinterpret_cast<sockaddr*>(&serverAddr), sizeof(serverAddr));
    cout << "Sent DNS query " << query.data() << endl;

    // Prepare a buffer to receive the DNS response and retrieve it
    vector<uint8_t> response(512);
    recv(sock, response.data(), response.size(), 0);
    cout << "Received DNS response " << response.data() << endl;

    // Close the socket after communication
    close(sock);

    // Analyze the received DNS response and print relevant details
    analyzeResponse(response);

    return 0;
}

