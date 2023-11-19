#include "config.h"


Config parseArguments(int argc, char* argv[], Config config, int startIdx) {
    int opt;

    // Start processing arguments from startIdx
    optind = startIdx;

    while ((opt = getopt(argc, argv, "rx6ts:p:")) != -1) {
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
            case 't':
                config.trace = true;
                break;
            default:
                cerr << "Usage: " << argv[0] << " [-r] [-x] [-6] [-t] -s server [-p port] address" << endl;
                exit(EXIT_FAILURE);
        }
    }
    if (startIdx != 1) {
        return config;
    }

    // Set the address after processing all options
    if (optind < argc) {
        config.address = argv[optind++];

        // Recursively call parseArguments if there are more arguments
        if (optind < argc) {
            config = parseArguments(argc, argv, config, optind);
        }
    } else {
        cerr << "Address is missing." << endl;
        exit(EXIT_FAILURE);
    }

    // Additional logic for PTR query type
    if (config.queryType == PTR) {
        config.address = reverseIP(config.address);
    }


    // Check for required server IP
    if (config.serverIP.empty()) {
        cerr << "DNS server IP is required." << endl;
        exit(EXIT_FAILURE);
    }

    return config;
}
