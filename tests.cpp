#include <cassert>
#include "config.h"
#include "dns.h"

// Test function for parsing arguments
void testParseArguments() {
    char* argv1[] = {
            (char*)"dns",
            (char*)"-r",
            (char*)"-s",
            (char*)"8.8.8.8",
            (char*)"-p",
            (char*)"53",
            (char*)"www.example.com"
    };
    int argc1 = 7;
    Config config1 = parseArguments(argc1, argv1);

    assert(config1.recursion == true);
    assert(config1.serverIP == "8.8.8.8");
    assert(config1.port == 53);
    assert(config1.address == "www.example.com");

    cout << "testParseArguments passed." << endl;
}

// Test function for creating a DNS query
void testCreateQuery() {
    auto query = createQuery("www.example.com", true);

    // Basic check to see if the query is not empty
    assert(!query.empty());

    // TODO: You can add more detailed checks here (like if the query contains the domain name, etc.)

    cout << "testCreateQuery passed." << endl;
}

// Test function for analyzing a response (this is a mock test, assuming you have a method to decode the DNS header)
void testAnalyzeResponse() {
    // A mock DNS response (this is just a simple example; in a real-world scenario, you would use a real response or a more complex mock)
    vector<uint8_t> mockResponse = { /*... fill with sample data ...*/ };

    // Mock function to get the DNSHeader from a response (assuming you have it in your main code)
    DNSHeader header = getDNSHeaderFromResponse(mockResponse);

    assert(header.id == 12345);  // Check the ID (based on the sample above)
    assert(header.qd_count == 1);  // Check question count

    // TODO: Add more checks as needed

    cout << "testAnalyzeResponse passed." << endl;
}

// TODO: Add more test functions as needed...

int main() {
    testParseArguments();
    testCreateQuery();
    testAnalyzeResponse();
    // TODO: Call other test functions here...

    cout << "All tests passed." << endl;
    return 0;
}
