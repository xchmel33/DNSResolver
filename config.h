//
// Created by lukXmelo on 23/09/2023.
//

#ifndef DNSRESOLVER_CONFIG_H
#define DNSRESOLVER_CONFIG_H

#include <iostream>
#include <cstring>
#include <vector>
#include <unistd.h>
using namespace std;

/**
 * \struct Config
 * \brief Structure to hold configuration parameters.
 */
struct Config {
    bool recursion = false;
    bool reverseQuery = false;
    bool typeAAAA = false;
    string serverIP;
    uint16_t port = 53;
    string address;
};

/**
 * \brief Parse program arguments and return configuration.
 * \param argc Argument count.
 * \param argv Array of arguments.
 * \return Configuration parsed from the arguments.
 */
Config parseArguments(int argc, char* argv[]);

#endif //DNSRESOLVER_CONFIG_H
