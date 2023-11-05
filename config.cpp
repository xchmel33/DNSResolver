#include "config.h"

Config parseArguments(int argc, char* argv[]) {
    Config config;
    int opt;

    while ((opt = getopt(argc, argv, "rx6s:p:")) != -1) {
        switch (opt) {
            case 'r':
                config.recursion = true;
                break;
            case 'x':
                config.queryType = PTR;
                break;
            case '6':
                config.queryType = AAAA;
                break;
            case 's':
                config.serverIP = optarg;
                break;
            case 'p':
                config.port = static_cast<uint16_t>(stoi(optarg));
                break;
            default:
                cerr << "Usage: " << argv[0] << " [-r] [-x] [-6] -s server [-p port] address" << endl;
                exit(EXIT_FAILURE);
        }
    }

    if (optind < argc) {
        config.address = argv[optind];
    } else {
        cerr << "Address is missing." << endl;
        exit(EXIT_FAILURE);
    }

    if (config.serverIP.empty()) {
        cerr << "DNS server IP is required." << endl;
        exit(EXIT_FAILURE);
    }

    if (config.queryType == PTR) {
        config.address = reverseIP(config.address);
    }

    return config;
}