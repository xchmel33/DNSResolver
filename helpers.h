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
 * \brief checks if a string is a valid IPv4 address.
 * \param ip The string to be checked.
 */
bool isValidIPv4(const string &ip);

/**
 * \brief checks if a string is a valid IPv6 address.
 * \param ip The string to be checked.
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
 * @brief Converts a qname from Question section to a string.
 * @param qname The qname to be converted.
 * @return string The converted qname.
 */
string qnameToString(const vector<uint8_t>& qname);

/**
 * @brief Converts a qclass from Question section to a string.
 * @param qclass The qclass to be converted.
 * @return string The converted qclass.
 */
string classToString(uint16_t qclass);

/**
 *  @breif reverses ipv4 using in-addr.arpa
 * @param ip string to be reversed
 * @return reveresed ipv4 string
 */
string reverseIPv4(const string& ip);

/**
 *  @breif reverses ipv6 using ip6.arpa
 * @param ip string to be reversed
 * @return reveresed ipv6 string
 */
string reverseIPv6(const string& ip);

/**
 * @breif reverses any ipv4 or ipv6 address
 * @param ip string to be reversed
 * @return reversed ip string
 */
string reverseIP(const string& ip);

/**
 * @breif formats IPv6 to shortened address format
 * @param response vector of bytes containing the response
 * @param offset offset of the response
 * @return formatted IPv6 string
 */
string formatIPv6(const vector<uint8_t>& response, int offset);

/**
 * @brief Joins a vector of strings with a delimiter.
 * @param vec The vector of strings to be joined.
 * @param delim The delimiter to be used.
 * @return string The joined string.
 */
string join(const vector<string>& vec, char delim);

#endif //DNSRESOLVER_HELPERS_H
