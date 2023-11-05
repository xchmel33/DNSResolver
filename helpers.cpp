//
// Created by lukXmelo on 05/11/2023.
//

#include "helpers.h"

bool isValidIP(const string &ip) {
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
        } catch (const std::invalid_argument& ia) {
            // Catch invalid argument exception (e.g., non-numeric string)
            return false;
        } catch (const std::out_of_range& oor) {
            // Catch out of range exception (e.g., number too large)
            return false;
        } catch (...) {
            // Catch any other exception
            return false;
        }
    }

    return count == 4;
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

string reverseIP(const string& ip) {
    // Check if IP is not already reversed
    if (ip.find(".in-addr.arpa") != string::npos) {
        return ip;
    }

    vector<string> octets;
    stringstream ss(ip);
    string octet;

    // Split the IP by dots
    while (getline(ss, octet, '.')) {
        octets.push_back(octet);
    }

    // If no octets were found, return an error message
    if (octets.empty()) {
        cerr << "No octets found in IP address: " << ip << endl;
        return "";
    }

    // Reverse the octets
    reverse(octets.begin(), octets.end());

    // Join the reversed octets with dots and append the special domain for reverse DNS lookups
    string reversedIP = join(octets, '.') + ".in-addr.arpa";
    return reversedIP;
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
