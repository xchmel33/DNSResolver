#include "dns.h"

vector<uint8_t> createQuery(const Config& config) {
    vector<uint8_t> buffer;

    // Initialize DNS Header
    DNSHeader header = {};
    header.id = htons(12345);  // Just a random ID
    header.qd_count = htons(1);
    uint16_t flags = 0;
    flags |= (1 << 8); // RD flag is the 9th bit in the flags field
    if (!config.recursion) {
        flags &= ~(1 << 8); // Clear the RD bit if recursion is not desired
    }
    header.flags = htons(flags);

    // Convert domain name into DNS format
    size_t pos = 0;
    while (pos < config.address.size()) {
        size_t nextPos = config.address.find('.', pos);
        if (nextPos == string::npos) {
            nextPos = config.address.size();
        }
        uint8_t len = nextPos - pos;
        buffer.push_back(len);
        for (size_t i = pos; i < nextPos; ++i) {
            buffer.push_back(config.address[i]);
        }
        pos = nextPos + 1;
    }
    buffer.push_back(0);

    Question q = {};
    q.qtype = htons(config.queryType);
    q.qclass = htons(1); // IN class

    // Add DNS header to the buffer
    buffer.insert(buffer.begin(), reinterpret_cast<uint8_t*>(&header), reinterpret_cast<uint8_t*>(&header) + sizeof(DNSHeader));

    // Add the query type and class after the domain name
    buffer.insert(buffer.end(), reinterpret_cast<uint8_t*>(&q.qtype), reinterpret_cast<uint8_t*>(&q.qtype) + 4); // sizeof(qtype) + sizeof(qclass) = 4

    //    cout << "query buffer: ";
    //    for (uint8_t byte : buffer) {
    //        cout << static_cast<int>(byte) << " ";
    //    }
    //    cout << endl;

    return buffer;
}

int createSocket() {;
    int sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0) {
        return -1;
    }
    return sock;
}

vector<string> getDefaultNameServers() {
    vector<string> nameServers;
    ifstream resolvConf("/etc/resolv.conf");
    string line;
    regex nameserverRegex(R"(nameserver\s+(\S+))");  // Regex to match nameserver lines
    smatch match;

    if (!resolvConf.is_open()) {
        cerr << "No DNS servers found in /etc/resolv.conf" << endl;
        return nameServers;
    }

    while (getline(resolvConf, line)) {
        if (regex_search(line, match, nameserverRegex) && match.size() > 1) {
            // The first sub-match is the IP address of the nameserver
            nameServers.push_back(match[1].str());
        }
    }

    if (nameServers.empty()) {
        cerr << "No DNS servers found in /etc/resolv.conf" << endl;
    }

    return nameServers;
}

string resolveDNSServerName(const string& serverName) {

    // Get the system's default nameservers
    auto nameServers = getDefaultNameServers();
    if (nameServers.empty()) {
        cerr << "No DNS servers found in /etc/resolv.conf" << endl;
        return "";
    }

    // Use the first nameserver from the list for the query
    Config config;
    config.serverIP = nameServers[0];
    config.address = serverName;
    config.queryType = A;
    config.recursion = true;

    // Create the socket
    int sock = createSocket();
    if (sock < 0) {
        cerr << "Failed to create socket" << endl;
        return "";
    }

    // Send the query
    if (sendDNSQuery(sock, config) < 0) {
        cerr << "Failed to send DNS query" << endl;
        close(sock);
        return "";
    }

    // Receive the response
    vector<uint8_t> response = receiveDNSResponse(sock, 5);
    if (response.empty()) {
        cerr << "Failed to receive DNS response" << endl;
        close(sock);
        return "";
    }

    close(sock);

    // Parse the response to get the IP address
    auto dnsResponse = analyzeResponse(response);
    if (!dnsResponse.answers.empty()) {
        return dnsResponse.answers[0].rdata;
    }

    return "";
}

ssize_t sendDNSQuery(int sock, const Config& config) {

    sockaddr_in serverAddr = {};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(config.port);  // Use port from arguments or default to 53

    if (isValidIP(config.serverIP)) {
        // If serverIP is a valid IP, use it directly
        serverAddr.sin_addr.s_addr = inet_addr(config.serverIP.c_str());
    } else {
        // If serverIP is a domain, resolve it to an IP address
        string record = resolveDNSServerName(config.serverIP);
        if(record.empty()) {
            cerr << "DNS server name resolution failed" << endl;
            close(sock);
            return -1;
        }

        // Convert the IP address from string to a network address structure
        if (inet_pton(AF_INET, record.c_str(), &(serverAddr.sin_addr)) <= 0) {
            cerr << "Invalid IP address format" << endl;
            close(sock);
            return -1;
        }
    }

    // Generate a DNS query based on the given domain and recursion option
    auto query = createQuery(config);

    // Send the DNS query to the server using the socket
    ssize_t sentBytes = sendto(sock, query.data(), query.size(), 0,
                               reinterpret_cast<sockaddr*>(&serverAddr), sizeof(serverAddr));
    cout << "Sent DNS query to " << config.serverIP << ":" << config.port << endl;

    return sentBytes;
}

vector<uint8_t> receiveDNSResponse(int sock, int timeoutSec) {
    // Prepare a buffer to receive the DNS response and retrieve it
    vector<uint8_t> response(512);

    // Set up a file descriptor set for select
    fd_set readFDS;
    FD_ZERO(&readFDS);
    FD_SET(sock, &readFDS);

    // Set up a timeout
    struct timeval timeout;
    timeout.tv_sec = timeoutSec;
    timeout.tv_usec = 0;

    // Wait for a response or a timeout
    int ret = select(sock + 1, &readFDS, NULL, NULL, &timeout);
    if (ret == -1) {
        perror("select");
        response.clear(); // Clear the response to indicate an error
    } else if (ret == 0) {
        cout << "No response received within the timeout period" << endl;
        response.clear(); // Clear the response to indicate a timeout
    } else {
        // Receive the response from the DNS server
        ssize_t bytesReceived = recv(sock, response.data(), response.size(), 0);
        if (bytesReceived < 1) {
            perror("recv");
            response.clear(); // Clear the response to indicate an error
        } else {
            // Resize buffer to the actual number of bytes received
            response.resize(bytesReceived);
        }
    }

    return response;
}

Question parseQuestionSection(const vector<uint8_t>& response, unsigned int& offset) {
    Question question;
    // Parse QNAME
    while (response[offset] != 0) {  // The domain name is ended with 0
        unsigned int label_length = response[offset];
        for (unsigned int i = 1; i <= label_length; ++i) {
            question.qname.push_back(response[offset + i]);
        }
        question.qname.push_back('.');  // Separate domain levels with dot
        offset += label_length + 1;  // Move offset to the end of this label
    }
    question.qname.push_back(0); // Add the 0 byte at the end of QNAME
    offset++;  // Skip the 0 byte at the end of QNAME

    // Parse QTYPE
    memcpy(&question.qtype, &response[offset], 2);
    question.qtype = ntohs(question.qtype);
    offset += 2;

    // Parse QCLASS
    memcpy(&question.qclass, &response[offset], 2);
    question.qclass = ntohs(question.qclass);
    offset += 2;

    return question;
}

DNSAnswer* parseDNSAnswer(const vector<uint8_t>& response, unsigned int& offset) {
    DNSAnswer* answer = new DNSAnswer;

    answer->name = extractName(response, offset);

    // Parse the type, class, ttl, and rdlength fields
    memcpy(&answer->type, &response[offset], 2);
    answer->type = ntohs(answer->type);
    offset += 2;

    memcpy(&answer->aclass, &response[offset], 2);
    answer->aclass = ntohs(answer->aclass);
    offset += 2;

    memcpy(&answer->ttl, &response[offset], 4);
    answer->ttl = ntohl(answer->ttl);
    offset += 4;

    memcpy(&answer->rdlength, &response[offset], 2);
    answer->rdlength = ntohs(answer->rdlength);
    offset += 2;

    // A Record
    if (answer->type == 1) {
        // Make sure rdlength is 4 for an A record
        if (answer->rdlength == 4) {
            // Convert the IP address from bytes to a string format
            answer->rdata = to_string(static_cast<unsigned char>(response[offset])) + "." +
                            to_string(static_cast<unsigned char>(response[offset + 1])) + "." +
                            to_string(static_cast<unsigned char>(response[offset + 2])) + "." +
                            to_string(static_cast<unsigned char>(response[offset + 3]));
            offset += 4;
        } else {
            throw runtime_error("Invalid rdlength for A record");
        }
    }
    // NS Record
    if (answer->type == 2) {
        answer->rdata = extractName(response, offset);
    }

    // CNAME Record
    if (answer->type == 5) {
        answer->rdata = extractName(response, offset);
    }

    // PTR Record
    if (answer->type == 12) {
        answer->rdata = extractName(response, offset);
    }

    return answer;
}

string extractName(const vector<uint8_t>& packet, unsigned int& offset) {
    string name;
    bool jumped = false;
    unsigned int jumpOffset = 0;
    int safetyCounter = 0;

    while (true) {
        if(safetyCounter++ > 100) {
            throw runtime_error("Possible malformed packet - processing name takes too long");
        }
        if(offset >= packet.size()) {
            throw runtime_error("Offset out of bounds while processing name");
        }

        uint8_t length = packet[offset];
        if (length == 0) {
            if (!jumped) offset++;
            break;
        }
        if ((length & 0xC0) == 0xC0) {
            if (!jumped) {
                jumped = true;
                jumpOffset = offset + 2;  // Set jumpOffset to byte following pointer for later use
            }
            offset = ((length & 0x3F) << 8) | packet[offset + 1];
            continue;
        }
        if (offset + length >= packet.size()) {
            throw runtime_error("Offset + length out of bounds while processing name");
        }
        name.append(packet.begin() + offset + 1, packet.begin() + offset + 1 + length);
        name.push_back('.');
        offset += (length + 1);
    }
    if (name.back() == '.') name.pop_back();
    offset = jumped ? jumpOffset : offset;
    return name;
}

DNSResponse analyzeResponse(const vector<uint8_t>& response) {
    // Declare and initialize the DNSResponse object
    DNSResponse dnsResponse;

    // Parse the header
    DNSHeader* header = (DNSHeader*)response.data();
    dnsResponse.header.id = ntohs(header->id);
    dnsResponse.header.flags = ntohs(header->flags);
    dnsResponse.header.qd_count = ntohs(header->qd_count);
    dnsResponse.header.an_count = ntohs(header->an_count);
    dnsResponse.header.ns_count = ntohs(header->ns_count);
    dnsResponse.header.ar_count = ntohs(header->ar_count);

    // Offset to keep track of our position in the response
    unsigned int offset = sizeof(DNSHeader);

    // Parse the question section
    for (int i = 0; i < dnsResponse.header.qd_count; ++i) {
        Question question = parseQuestionSection(response, offset);
        dnsResponse.questions.push_back(question);
    }

    // Parse the answer section
    for (int i = 0; i < dnsResponse.header.an_count; ++i) {
        DNSAnswer* answer = parseDNSAnswer(response, offset);
        dnsResponse.answers.push_back(*answer);
        delete answer; // Don't forget to free the allocated memory!
    }

    // Parse the authority section
    for (int i = 0; i < dnsResponse.header.ns_count; ++i) {
        DNSAnswer* authority = parseDNSAnswer(response, offset);
        dnsResponse.authority.push_back(*authority);
        delete authority; // Free the allocated memory
    }

    // Parse the additional section
    for (int i = 0; i < dnsResponse.header.ar_count; ++i) {
        DNSAnswer* additional = parseDNSAnswer(response, offset);
        dnsResponse.additional.push_back(*additional);
        delete additional; // Free the allocated memory
    }

    // Return the filled DNSResponse structure
    return dnsResponse;
}

void mergeResponses(DNSResponse* finalResponse, const DNSResponse* newResponse) {
    if (!finalResponse || !newResponse) {
        return; // If either is null, we cannot merge them.
    }

//    cout << "Merging responses:\nfinal:" << endl;
//    printResponse(*finalResponse);
//    cout << "new:" << endl;
//    printResponse(*newResponse);
//    cout << endl << endl;

    // Append answers from newResponse to finalResponse
    finalResponse->answers.insert(
            finalResponse->answers.end(),
            newResponse->answers.begin(),
            newResponse->answers.end()
    );

    // Append authority records from newResponse to finalResponse
    finalResponse->authority.insert(
            finalResponse->authority.end(),
            newResponse->authority.begin(),
            newResponse->authority.end()
    );

    // Append additional records from newResponse to finalResponse
    finalResponse->additional.insert(
            finalResponse->additional.end(),
            newResponse->additional.begin(),
            newResponse->additional.end()
    );

    // Update header fields like count of answers, authority, and additional records.
    // This is a simplistic approach; adjust this logic based on how you need to use these counts.
    finalResponse->header.an_count += newResponse->header.an_count;
    finalResponse->header.ns_count += newResponse->header.ns_count;
    finalResponse->header.ar_count += newResponse->header.ar_count;
}

DNSResponse* handleRecursion(const vector<uint8_t>& response, Config& config, int sock, DNSResponse* finalResponse) {

    auto* dnsResponse = new DNSResponse(analyzeResponse(response));

    if (!config.recursion || dnsResponse->header.flags & (1 << RA_FLAG_POSITION)) {
        // If the response already includes IP addresses in the additional section, use them
        if (!dnsResponse->additional.empty()) {
            for (const auto &additional: dnsResponse->additional) {
                if (additional.type == 1) { // A record
                    config.serverIP = dnsResponse->additional[0].rdata;
                    ssize_t sentBytes = sendDNSQuery(sock, config);
                    if (sentBytes == -1) {
                        continue;
                    }
                    vector<uint8_t> nextResponse = receiveDNSResponse(sock, 5);
                    if (response.empty()) {
                        continue;
                    }
                    // todo: think and decide if this needs to be here
                    if (finalResponse) {
                        mergeResponses(finalResponse, dnsResponse);
                        delete dnsResponse; // as we have merged the responses, we can delete the temporary one
                        dnsResponse = finalResponse; // point to the merged response
                    }
                    // Recursively handle the next response, pass finalResponse to accumulate answers
                    return handleRecursion(nextResponse, config, sock, dnsResponse);
                }
                // You may want to handle type AAAA (IPv6) as well
            }
            delete dnsResponse;
            return nullptr;
        } else if (!dnsResponse->authority.empty()) {
            // If there's no IP address, check the authority section for NS records
            for (const auto &nsRecord: dnsResponse->authority) {
                if (nsRecord.type == 2) { // NS record
                    string nsIP = resolveDNSServerName(nsRecord.rdata);
                    if (nsIP.empty()) {
                        continue;
                    }
                    config.serverIP = nsIP;
                    ssize_t sentBytes = sendDNSQuery(sock, config);
                    if (sentBytes == -1) {
                        continue;
                    }
                    vector<uint8_t> nextResponse = receiveDNSResponse(sock, 5);
                    if (response.empty()) {
                        continue;
                    }
                    // todo: think and decide if this needs to be here
                    if (finalResponse) {
                        mergeResponses(finalResponse, dnsResponse);
                        delete dnsResponse; // as we have merged the responses, we can delete the temporary one
                        dnsResponse = finalResponse; // point to the merged response
                    }
                    // Recursively handle the next response, pass finalResponse to accumulate answers
                    return handleRecursion(nextResponse, config, sock, dnsResponse);
                }
            }
            delete dnsResponse;
            return nullptr;
        } else if (!dnsResponse->answers.empty()) {
            for (const auto &answer: dnsResponse->answers) {
                if (answer.type == 5) { // CNAME record
                    // If a CNAME record is present with no additional or authority information,
                    // resolve the CNAME target
                    // This would also involve sending another DNS query for the CNAME target
                }
            }
            delete dnsResponse;
            return nullptr;
        }
    }

    // If finalResponse is not provided, this is the first call and dnsResponse is the final one
    if (!finalResponse) {
        finalResponse = dnsResponse;
    } else {
        // Merge this response into the finalResponse
        mergeResponses(finalResponse, dnsResponse);
        delete dnsResponse; // Delete the temporary response
    }

    return finalResponse;
}

void printResponse(const DNSResponse& response) {
    // Header flags parsing
    bool aa = response.header.flags & (1 << AA_FLAG_POSITION);
    bool rd = response.header.flags & (1 << RD_FLAG_POSITION);
    bool ra = response.header.flags & (1 << RA_FLAG_POSITION);
    bool tc = response.header.flags & (1 << TC_FLAG_POSITION);

    cout << "Authoritative: " << (aa ? "Yes" : "No")
         << ", Recursive: " << (rd ? "Yes" : "No")
         << ", Recursion Available: " << (ra ? "Yes" : "No")
         << ", Truncated: " << (tc ? "Yes" : "No") << endl;

    // Question Section
    cout << "Questions (" << response.header.qd_count << "):\n";
    for (const auto& question : response.questions) {
        cout << "  " << qnameToString(question.qname) << ", "
             << typeToString(question.qtype) << ", "
             << classToString(question.qclass) << endl;
    }

    // Answer Section
    cout << "Answer section (" << response.header.an_count << "):\n";
    for (const auto& answer : response.answers) {
        cout << "  " << answer.name << ", "
             << typeToString(answer.type) << ", IN, "
             << answer.ttl << ", " << answer.rdata << endl;
    }

    // Authority Section
    cout << "Authority section (" << response.header.ns_count << "):\n";
    for (const auto& authority : response.authority) {
        cout << "  " << authority.name << ", "
             << typeToString(authority.type) << ", IN, "
             << authority.ttl << ", " << authority.rdata << endl;
    }

    // Additional Section
    cout << "Additional section (" << response.header.ar_count << "):\n";
    for (const auto& additional : response.additional) {
        cout << "  " << additional.name << ", "
             << typeToString(additional.type) << ", IN, "
             << additional.ttl << ", " << additional.rdata << endl;
    }
}
