//
// Created by lukXmelo on 05/11/2023.
//

#ifndef DNSRESOLVER_HELPERS_H
#define DNSRESOLVER_HELPERS_H

#include <iostream>
#include <fstream>
#include <cstring>
#include <vector>
#include <unistd.h>
#include <string>
#include <regex>
#include <sstream>
#include <algorithm>
#include <map>
#include <optional>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netdb.h>

using namespace std;


/**
 * \brief
 */
bool isValidIPv4(const string &ip);

/**
 * \brief
 */
bool isValidIPv6(const string &ip);

/**
 * \brief Parse program arguments and return configuration.
 * \param argc Argument count.
 * \param argv Array of arguments.
 * \return Configuration parsed from the arguments.
 */
string typeToString(uint16_t type);

/**
 * @brief Converts a qname to a string.
 * @param qname The qname to be converted.
 * @return string The converted qname.
 */
string qnameToString(const vector<uint8_t>& qname);

/**
 * @brief Converts a qtype to a string.
 * @param qclass The qtype to be converted.
 * @return string The converted qtype.
 */
string classToString(uint16_t qclass);

/**
 * @brief Converts a qtype to a string.
 * @param qtype The qtype to be converted.
 * @return string The converted qtype.
 */
string reverseIPv4(const string& ip);

/**
 * @brief Converts a qtype to a string.
 * @param qtype The qtype to be converted.
 * @return string The converted qtype.
 */
string reverseIPv6(const string& ip);

/**
 * @brief Converts a qname to a string.
 * @param qname The qname to be converted.
 * @return string The converted qname.
 */
string reverseIP(const string& ip);

/**
 * @brief Converts a qtype to a string.
 * @param qtype The qtype to be converted.
 * @return string The converted qtype.
 */
string formatIPv6(const vector<uint8_t>& response, int offset);

/**
 * @brief Converts a qtype to a string.
 * @param qtype The qtype to be converted.
 * @return string The converted qtype.
 */
string join(const vector<string>& vec, char delim);

#endif //DNSRESOLVER_HELPERS_H
