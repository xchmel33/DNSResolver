//
// Created by lukXmelo on 23/09/2023.
//

#ifndef DNSRESOLVER_CONFIG_H
#define DNSRESOLVER_CONFIG_H

#include "helpers.h"

enum QueryType{
    A = 1,
    NS = 2,
    CNAME = 5,
    SOA = 6,
    PTR = 12,
    AAAA = 28
};

/**
 * \struct Config
 * \brief Structure to hold configuration parameters.
 */
struct Config {
    bool recursion = false;
    bool trace = false;
    QueryType queryType = A;
    string serverIP;
    uint16_t port = 53;
    string address;
};

/**w
 * \brief Parse program arguments and return configuration.
 * \param argc Argument count.
 * \param argv Array of arguments.
 * \return Configuration parsed from the arguments.
 */
Config parseArguments(int argc, char* argv[], Config config = Config(), int startIdx = 1);

#endif //DNSRESOLVER_CONFIG_H
