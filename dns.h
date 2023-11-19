/**
 * \file dns.h
 * \brief DNS utility definitions
 * \author Lukas Chmelo
 */

#ifndef DNSRESOLVER_DNS_H
#define DNSRESOLVER_DNS_H

#include "config.h"

#define AA_FLAG_POSITION 10
#define TC_FLAG_POSITION 9
#define RD_FLAG_POSITION 8
#define RA_FLAG_POSITION 7


/**
 * \struct DNSHeader
 * \brief Structure to represent the DNS header.
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
 * \struct DNSQuestion
 * \brief Structure to represent the DNS query question section.
 */
struct DNSQuestion {
    vector<uint8_t> qname;
    uint16_t qtype;
    uint16_t qclass;
};


/**
 * \struct DNSRecord
 * \brief Structure to represent the DNS record.
 */
struct DNSRecord {
    string name;
    uint16_t type;
    uint16_t aclass;
    uint32_t ttl;
    uint16_t rdlength;
    string rdata;
};


struct DNSPacket {
    DNSHeader header;
    vector<DNSQuestion> questions;
    vector<DNSRecord> answers;
    vector<DNSRecord> authority;
    vector<DNSRecord> additional;
};

/**
 * \brief Create a DNS query based on the given domain and recursion option.
 * @param config The DNS config instance.
 * \return Vector containing the bytes of the DNS query.
 */
vector<uint8_t> createQuery(const Config& config);

/**
 * @brief Creates a UDP socket for DNS communication.
 * @return socket The created socket.
 */
int createSocket(bool ipv6=false);

/**
 * @brief Gets the default name servers from the /etc/resolv.conf file.
 * @return vector<string> The default name servers.
 */
vector<string> getDefaultNameServers();

/**
 * @brief Gets the default domain from the /etc/resolv.conf file.
 * @param serverName The name of the server.
 * @param trace - trace mode - print results of this query.
 * @return string The default domain.
 */
string resolveDNSServerName(const string& serverName, bool trace=false);

/**
 * @brief Sends the DNS query to the server using the socket.
 * @param config The DNS config instance.
 * @return size_t The number of bytes sent.
 */
ssize_t sendDNSQuery(int sock, const Config& config);

/**
 * @brief Receives the DNS response from the server using the socket.
 * @param sock The socket to be used for communication.
 * @param timeoutSec The timeout in seconds.
 * @return vector<uint8_t> The DNS response as a byte vector.
 */
vector<uint8_t> receiveDNSResponse(int sock, int timeoutSec);

/**
 * @brief Extracts the name from the provided DNS response.
 * @param packet The full DNS response as a byte vector.
 * @param offset The offset in the packet where the name starts.
 * @return string The extracted name.
 */
string extractName(const vector<uint8_t>& packet, unsigned int& offset);

/**
 * @brief Parses the DNS question section from the provided DNS response.
 * @param response The full DNS response as a byte vector.
 * @param offset The offset in the packet where the name starts.
 * @return a pointer to the DNSQuestion structure
 */
DNSQuestion parseDNSQuestionSection(const vector<uint8_t>& response, unsigned int& offset);

/**
 * @brief Parses the DNS answer from the provided DNS response.
 * @param response
 * @param offset
 * @return a pointer to the DNSRecord structure
 */
DNSRecord* parseDNSAnswer(const vector<uint8_t>& response, unsigned int& offset);


/**
 * \brief Analyze a DNS response and return the response structure.
 * \param response Vector containing the bytes of the DNS response.
 */
DNSPacket analyzeResponse(const vector<uint8_t>& response);

/**
 * @brief Merges the new response with the final response.
 * @param finalResponse The final response.
 * @param newResponse The new response.
 */
void mergeResponses(DNSPacket* finalResponse, const DNSPacket* newResponse);

/**
 * @brief Checks if the target is in the DNS response.
 * @param dnsResponse The DNS response instance to be checked.
 * @param finalResponse The final response instance.
 * @param config The DNS config instance.
 */
bool checkTargetDNSResponse(const DNSPacket& response, const Config& config);

/**
 * @brief Handles the recursion on the client side.
 * @param dnsResponse The DNS response instance to be handled.
 * @param config The DNS config instance.
 */
DNSPacket* handleRecursion(const vector<uint8_t>& response, Config& config, int sock);

/**
 * @brief Prints the DNS response to the standard output.
 * @param response The DNS response instance to be printed.
 */
void printResponse(const DNSPacket& response);

/**
 * @brief Prints the DNS response in trace mode to the standard output.
 * @param response The DNS response instance to be printed.
 * @param serverName The domain name of the server that was queried
 * @param result - if query is the final result
 */
void printTrace(const DNSPacket& response, const string& serverName, bool result=false);

#endif // DNSRESOLVER_DNS_H
