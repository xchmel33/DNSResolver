/**
 * \file dns.h
 * \brief DNS utility definitions
 * \author Lukas Chmelo
 */

#ifndef DNSRESOLVER_DNS_H
#define DNSRESOLVER_DNS_H

#include <iostream>
#include <cstring>
#include <vector>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netdb.h>

/**
 * \struct DNSHeader
 * \brief Structure to represent the DNS header
 */
struct DNSHeader {
    uint16_t id;
    uint16_t flags;
    uint16_t qd_count;
    uint16_t an_count;
    uint16_t ns_count;
    uint16_t ar_count;
};

/**
 * \struct Question
 * \brief Structure to represent the DNS query question section.
 */
struct Question {
    vector<uint8_t> qname;
    uint16_t qtype;
    uint16_t qclass;
};

/**
 * \struct DNSAnswer
 * \brief Structure to represent the DNS answer
 */
struct DNSAnswer {
    uint16_t name;
    uint16_t type;
    uint16_t aclass;
    uint32_t ttl;
    uint16_t rdlength;
    in_addr ipAddress; // or uint32_t ip for simpler cases
};

/**
 * \brief Create a DNS query based on the given domain and recursion option.
 * \param domain Domain name for the query.
 * \param recursive Whether recursion is desired in the query.
 * \return Vector containing the bytes of the DNS query.
 */
vector<uint8_t> createQuery(const string& domain, bool recursive);


/**
 * @brief Extracts the DNSHeader from the provided DNS response.
 *
 * This function assumes that the DNSHeader is located at the beginning of the response.
 * If the response is too short to contain a valid DNS header, the function prints an
 * error message and terminates the program.
 *
 * @param response The full DNS response as a byte vector.
 * @return DNSHeader The extracted DNSHeader structure from the response.
 */
DNSHeader getDNSHeaderFromResponse(const vector<uint8_t>& response);

/**
 * \brief Analyze and print details from the received DNS response.
 * \param response Vector containing the bytes of the DNS response.
 */
void analyzeResponse(const vector<uint8_t>& response);

#endif // DNSRESOLVER_DNS_H
