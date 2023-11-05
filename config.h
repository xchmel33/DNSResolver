//
// Created by lukXmelo on 23/09/2023.
//

#ifndef DNSRESOLVER_CONFIG_H
#define DNSRESOLVER_CONFIG_H

#include "helpers.h"

enum QueryType{
    A = 1,
    PTR = 12,
    AAAA = 28
};

/**
 * \struct Config
 * \brief Structure to hold configuration parameters.
 */
struct Config {
    bool recursion = false;
    QueryType queryType = A;
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
