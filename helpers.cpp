//
// Created by lukXmelo on 05/11/2023.
//

#include "helpers.h"

bool isValidIPv4(const string &ip) {
    stringstream ss(ip);
    string segment;
    int count = 0;

    while (getline(ss, segment, '.')) {
        count++;
        try {
            int segmentValue = stoi(segment);
            if (count > 4 || segment.empty() || segmentValue < 0 || segmentValue > 255 ||
                (segment.size() > 1 && segment[0] == '0')) {
                return false;
            }
        } catch (const invalid_argument& ia) {
            // Catch invalid argument exception (e.g., non-numeric string)
            return false;
        } catch (const out_of_range& oor) {
            // Catch out of range exception (e.g., number too large)
            return false;
        } catch (...) {
            // Catch any other exception
            return false;
        }
    }

    return count == 4;
}

bool isValidIPv6(const string &ip) {
    stringstream ss(ip);
    string segment;
    int count = 0;
    bool doubleColon = false;

    while (getline(ss, segment, ':')) {
        if (segment.empty()) {
            if (doubleColon) { // More than one '::'
                return false;
            }
            doubleColon = true;
            continue;
        }
        count++;
        if (count > 8) {
            return false;
        }
        try {
            unsigned long segmentValue = stoul(segment, nullptr, 16);
            if (segmentValue > 0xFFFF) {
                return false;
            }
        } catch (const invalid_argument&) {
            return false;
        } catch (const out_of_range&) {
            return false;
        }
    }

    return doubleColon ? count <= 8 : count == 8;
}

string typeToString(uint16_t type) {
    static const map<uint16_t, string> typeMap = {
            {1, "A"},
            {2, "NS"},
            {5, "CNAME"},
            {6, "SOA"},
            {11, "WKS"},
            {12, "PTR"},
            {13, "HINFO"},
            {15, "MX"},
            {28, "AAAA"},
    };

    auto it = typeMap.find(type);
    if (it != typeMap.end())
        return it->second;
    else
        return "UNKNOWN";
}

string qnameToString(const vector<uint8_t>& qname) {
    string result;
    for (size_t i = 0; i < qname.size(); ++i) {
        if (qname[i] == 0) break; // End of the QNAME
        if (qname[i] < 32 || qname[i] >= 127) {
            // Non-printable characters (e.g., the length byte in QNAME)
            result.push_back('.');
        } else {
            // Printable characters
            result.push_back(static_cast<char>(qname[i]));
        }
    }
    return result;
}

// Function to convert DNS class values to string
string classToString(uint16_t qclass) {
    switch(qclass) {
        case 1: return "IN";  // Internet
            // Add more DNS class cases here if necessary
        default: return "UNKNOWN";
    }
}

string reverseIPv4(const string& ip) {
    vector<string> octets;
    stringstream ss(ip);
    string octet;

    while (getline(ss, octet, '.')) {
        octets.push_back(octet);
    }

    reverse(octets.begin(), octets.end());
    return join(octets, '.') + ".in-addr.arpa";
}

string reverseIPv6(const string& ip) {
    string reversedIP;
    vector<string> blocks;
    istringstream iss(ip);
    string block;

    // Split the IPv6 address into blocks
    while (getline(iss, block, ':')) {
        blocks.push_back(block);
    }

    // Pad the blocks to ensure each is 4 digits
    auto it = find(blocks.begin(), blocks.end(), "");
    if (it != blocks.end()) {
        auto dist = distance(blocks.begin(), it);
        blocks.erase(it);
        while (blocks.size() < 8) {
            blocks.insert(blocks.begin() + dist, "0000");
        }
    }

    // Ensure each block is 4 characters long, padding with zeros if necessary
    for (auto& blk : blocks) {
        while (blk.length() < 4) {
            blk = "0" + blk;
        }
    }

    // Reverse each hex digit and separate by dots
    for (auto rit = blocks.rbegin(); rit != blocks.rend(); ++rit) {
        for (auto ch = rit->rbegin(); ch != rit->rend(); ++ch) {
            reversedIP += *ch;
            reversedIP += '.';
        }
    }

    return reversedIP + "ip6.arpa";
}

string reverseIP(const string& ip) {
    if (ip.find(".in-addr.arpa") != string::npos || ip.find(".ip6.arpa") != string::npos) {
        return ip;
    }

    if (isValidIPv4(ip)) {
        return reverseIPv4(ip);
    } else if (isValidIPv6(ip)) {
        return reverseIPv6(ip);
    } else {
        cerr << "Invalid IP address format: " << ip << endl;
        return "";
    }
}

string formatIPv6(const vector<uint8_t>& response, int offset) {
    vector<string> blocks;
    stringstream ipv6;

    for (int i = 0; i < 16; i += 2) {
        unsigned int block = (response[offset + i] << 8) + response[offset + i + 1];
        stringstream hexBlock;
        hexBlock << hex << block;
        blocks.push_back(hexBlock.str());
    }

    // Find the longest sequence of zero blocks
    int longestZeroSequenceLength = 0;
    int longestZeroSequenceStart = -1;
    int currentZeroSequenceLength = 0;
    int currentZeroSequenceStart = -1;

    for (size_t i = 0; i < blocks.size(); ++i) {
        if (blocks[i] == "0") {
            if (currentZeroSequenceLength == 0) {
                currentZeroSequenceStart = int(i);
            }
            currentZeroSequenceLength++;
        } else {
            if (currentZeroSequenceLength > longestZeroSequenceLength) {
                longestZeroSequenceLength = currentZeroSequenceLength;
                longestZeroSequenceStart = currentZeroSequenceStart;
            }
            currentZeroSequenceLength = 0;
        }
    }

    // Check if the last sequence is the longest
    if (currentZeroSequenceLength > longestZeroSequenceLength) {
        longestZeroSequenceLength = currentZeroSequenceLength;
        longestZeroSequenceStart = currentZeroSequenceStart;
    }

    // Construct the shortened IPv6 address
    for (size_t i = 0; i < blocks.size(); ++i) {
        if (int(i) == longestZeroSequenceStart) {
            ipv6 << "::";
            i += longestZeroSequenceLength - 1; // Skip the zero blocks
        } else {
            ipv6 << blocks[i];
            if (i < blocks.size() - 1 && int(i) != longestZeroSequenceStart - 1) {
                ipv6 << ":";
            }
        }
    }

    return ipv6.str();
}

string join(const vector<string>& vec, char delim) {
    stringstream ss;
    for (size_t i = 0; i < vec.size(); ++i) {
        if (i != 0) {
            ss << delim;
        }
        ss << vec[i];
    }
    return ss.str();
}
