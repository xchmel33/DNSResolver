#!/bin/sh

# Function to run a test case and check its status
run_test() {
    echo "Running test: $1"
    eval $1
    if [ $? -eq 0 ]; then
        echo -e "PASS\n"
    else
        echo -e "FAIL\n"
    fi
}

# Ensure the executable exists
if [ ! -f ./dns ]; then
    echo "dns executable not found!"
    exit 1
fi

# List of test cases
# You would need to replace server_ip, port, and address placeholders with actual values
tests=(
    "./dns -r -s kazi.fit.vutbr.cz www.fit.vut.cz"
    "./dns -r -s kazi.fit.vutbr.cz www.github.com"
    "./dns -r -x -s kazi.fit.vutbr.cz 147.229.9.26"
    "./dns -r -x -s kazi.fit.vutbr.cz 140.82.121.4"
    "./dns -r -s  address"
    "./dns -x -s server_ip address"
    "./dns -6 -s server_ip address"
    "./dns -s server_ip -p port address"
)

# Run all test cases
for test in "${tests[@]}"; do
    run_test "$test"
done
