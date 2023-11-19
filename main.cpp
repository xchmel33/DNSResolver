#include "dns.h"

int main(int argc, char* argv[]) {
    // Parse arguments to get server IP, port, and other options
    Config config = parseArguments(argc, argv);

    if (config.address.empty()){
        cerr << "Address is missing or invalid." << endl;
        return -1;
    }

    // Send the DNS query
    int sock = createSocket(false);
    ssize_t sentBytes = sendDNSQuery(sock, config);
    if (sentBytes == -1) {
        cerr << "Failed to send DNS query." << endl;
        close(sock);
        return -2;
    }

    // Receive DNS response
    vector<uint8_t> response = receiveDNSResponse(sock, 5);
    if (response.empty()) {
        cerr << "Failed to receive a valid DNS response during the timeout period." << endl;
        return -3;
    }

    // backup server ip
    string dns_server = config.serverIP;

    // Analyze the response and handle recursion if necessary
    DNSPacket* dnsResponse = handleRecursion(response, config, sock);
    if (dnsResponse == nullptr) {
        return -4;
    }

    // Close the socket after communication
    close(sock);

    // Analyze the received DNS response and print relevant details
    if (config.trace) {
        printTrace(*dnsResponse, dns_server, true);
    } else {
        printResponse(*dnsResponse);
    }

    delete dnsResponse;

    return 0;
}

