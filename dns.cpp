#include "config.h"
#include "dns.h"

std::vector<uint8_t> createQuery(const std::string& domain, bool recursive) {
    std::vector<uint8_t> buffer;

    // Initialize DNS Header
    DNSHeader header = {};
    header.id = htons(12345);  // Just a random ID
    header.qd_count = htons(1);
    if (recursive) {
        header.flags = htons(1 << 8); // Set RD (Recursion Desired) flag
    }

    // Convert domain name into DNS format
    size_t pos = 0;
    while (pos < domain.size()) {
        size_t nextPos = domain.find('.', pos);
        if (nextPos == std::string::npos) {
            nextPos = domain.size();
        }
        uint8_t len = nextPos - pos;
        buffer.push_back(len);
        for (size_t i = pos; i < nextPos; ++i) {
            buffer.push_back(domain[i]);
        }
        pos = nextPos + 1;
    }
    buffer.push_back(0);

    Question q = {};
    q.qtype = htons(1);  // A record
    q.qclass = htons(1); // IN class

    // Add DNS header to the buffer
    buffer.insert(buffer.begin(), reinterpret_cast<uint8_t*>(&header), reinterpret_cast<uint8_t*>(&header) + sizeof(DNSHeader));

    // Add the query type and class after the domain name
    buffer.insert(buffer.end(), reinterpret_cast<uint8_t*>(&q.qtype), reinterpret_cast<uint8_t*>(&q.qtype) + 4); // sizeof(qtype) + sizeof(qclass) = 4

    std::cout << "query buffer: ";
    for (uint8_t byte : buffer) {
        std::cout << static_cast<int>(byte) << " ";
    }
    std::cout << std::endl;

    return buffer;
}

DNSHeader getDNSHeaderFromResponse(const vector<uint8_t>& response) {
    if(response.size() < sizeof(DNSHeader)) {
        // handle the error, possibly throw an exception or return an error code
        cerr << "Response is too short to contain a valid DNS header." << endl;
        exit(EXIT_FAILURE);
    }

    DNSHeader header;
    memcpy(&header, response.data(), sizeof(DNSHeader));
    return header;
}

void analyzeResponse(const vector<uint8_t>& response) {
    DNSHeader* header = (DNSHeader*)response.data();

    int answerCount = ntohs(header->an_count);
    cout << "Answers count: " << answerCount << endl;

    // Skip header and question
    unsigned int offset = sizeof(DNSHeader) + (response.size() - sizeof(DNSHeader) - sizeof(DNSAnswer) * answerCount);

    for (int i = 0; i < answerCount; ++i) {
        DNSAnswer* answer = (DNSAnswer*)(response.data() + offset);

        // We'll only print A records for now
        if (ntohs(answer->type) == 1) {
            cout << "IP Address: " << inet_ntoa(answer->ipAddress) << endl;
        }

        offset += sizeof(DNSAnswer);
    }
}