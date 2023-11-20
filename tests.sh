#!/bin/sh
# List of test cases
# ./dns -r -s kazi.fit.vutbr.cz www.fit.vut.cz
# ./dns -r -s kazi.fit.vutbr.cz www.github.com
# ./dns -r -s kazi.fit.vutbr.cz nes.fit.vutbr.cz
# ./dns -t -s kazi.fit.vutbr.cz overleaf.com
# ./dns -t -s kazi.fit.vutbr.cz overleaf.com
# ./dns -t -s 2606:4700:58::adf5:3b68 overleaf.com
# ./dns -t -s 192.55.83.30 www.overleaf.com
# ./dns -t -s 8.8.8.8 nes.fit.vutbr.cz
# ./dns -t -s 8.8.4.4 nes.fit.vutbr.cz
# ./dns -t -s 1.1.1.1 nes.fit.vutbr.cz
# ./dns -s kazi.fit.vutbr.cz www.fit.vut.cz
# ./dns -s kazi.fit.vutbr.cz www.github.com
# ./dns -s kazi.fit.vutbr.cz nes.fit.vutbr.cz
# ./dns -r -x -s kazi.fit.vutbr.cz 147.229.9.26
# ./dns -r -x -s kazi.fit.vutbr.cz 140.82.121.4
# ./dns -r -x -s kazi.fit.vutbr.cz 147.229.8.16
# ./dns -x -s kazi.fit.vutbr.cz 147.229.9.26
# ./dns -x -s kazi.fit.vutbr.cz 140.82.121.4
# ./dns -x -s kazi.fit.vutbr.cz 147.229.8.16
# ./dns -6 -r -s kazi.fit.vutbr.cz www.fit.vut.cz
# todo fix segmentation fault
# ./dns -x -s kazi.fit.vutbr.cz 2001:67c:1220:809:0:0:93e5:91a
# ./dns -x -s 8.8.8.8 -r 2001:67c:1220:809:0:0:93e5:91a
# todo fix segmentation fault
# ./dns -6 -s kazi.fit.vutbr.cz www.github.com
test1="A www.fit.vut.cz recursive; dig @kazi.fit.vutbr.cz www.fit.vut.cz +all;./dns -r -s kazi.fit.vutbr.cz www.fit.vut.cz"
test2="A www.github.com recursive; dig @kazi.fit.vutbr.cz www.github.com +all;./dns -r -s kazi.fit.vutbr.cz www.github.com"
test3="A nes.fit.vutbr.cz recursive; dig @kazi.fit.vutbr.cz nes.fit.vutbr.cz +all;./dns -r -s kazi.fit.vutbr.cz nes.fit.vutbr.cz"
test4="A nes.fit.vutbr.cz arg order 1; dig @kazi.fit.vutbr.cz nes.fit.vutbr.cz +all;./dns -s kazi.fit.vutbr.cz nes.fit.vutbr.cz -r"
test5="A nes.fit.vutbr.cz arg order 2; dig @kazi.fit.vutbr.cz nes.fit.vutbr.cz +all;./dns nes.fit.vutbr.cz -s kazi.fit.vutbr.cz -r"
test6="A www.fit.vut.cz non-recursive; dig @kazi.fit.vutbr.cz www.fit.vut.cz +all;./dns -s kazi.fit.vutbr.cz www.fit.vut.cz"
test7="A www.github.com non-recursive; dig @kazi.fit.vutbr.cz www.github.com +all;./dns -s kazi.fit.vutbr.cz www.github.com"
test8="A nes.fit.vutbr.cz non-recursive; dig @kazi.fit.vutbr.cz nes.fit.vutbr.cz +all;./dns -s kazi.fit.vutbr.cz nes.fit.vutbr.cz"
test9="PTR 142.251.37.100 recursive;dig @kazi.fit.vutbr.cz -x 142.251.37.100 +all;./dns -s kazi.fit.vutbr.cz 142.251.37.100 -r -x"
test10="PTR 147.229.9.26 recursive; dig @kazi.fit.vutbr.cz -x 147.229.9.26 +all;./dns -r -x -s kazi.fit.vutbr.cz 147.229.9.26"
test11="PTR 140.82.121.4 recursive; dig @kazi.fit.vutbr.cz -x 140.82.121.4 +all;./dns -r -x -s kazi.fit.vutbr.cz 140.82.121.4"
test12="PTR 147.229.8.16 recursive; dig @kazi.fit.vutbr.cz -x 147.229.8.16 +all;./dns -r -x -s kazi.fit.vutbr.cz 147.229.8.16"
test13="PTR 2001:67c:1220:809:0:0:93e5:91a recursive; dig @kazi.fit.vutbr.cz -x 2001:67c:1220:809:0:0:93e5:91a +all;./dns -r -x -s kazi.fit.vutbr.cz 2001:67c:1220:809:0:0:93e5:91a"
test14="PTR 2001:67c:1220:808::93e5:810 recursive; dig @kazi.fit.vutbr.cz -x 2001:67c:1220:808::93e5:810 +all;./dns -r -x -s kazi.fit.vutbr.cz 2001:67c:1220:808::93e5:810"
test15="PTR 147.229.9.26 non-recursive; dig @kazi.fit.vutbr.cz -x 147.229.9.26 +all;./dns -x -s kazi.fit.vutbr.cz 147.229.9.26"
test16="PTR 140.82.121.4 non-recursive; dig @kazi.fit.vutbr.cz -x 140.82.121.4 +norecurse +all;./dns -x -s kazi.fit.vutbr.cz 140.82.121.4"
test17="PTR 147.229.8.16 non-recursive; dig @kazi.fit.vutbr.cz -x 147.229.8.16 +all;./dns -x -s kazi.fit.vutbr.cz 147.229.8.16"
test18="PTR 2001:67c:1220:809:0:0:93e5:91a non-recursive; dig @kazi.fit.vutbr.cz -x 2001:67c:1220:809:0:0:93e5:91a +all;./dns -x -s kazi.fit.vutbr.cz 2001:67c:1220:809:0:0:93e5:91a"
test19="PTR 2001:67c:1220:808::93e5:810 non-recursive; dig @kazi.fit.vutbr.cz -x 2001:67c:1220:808::93e5:810 +all;./dns -x -s kazi.fit.vutbr.cz 2001:67c:1220:808::93e5:810"
test20="AAAA www.fit.vut.cz recursive; dig @kazi.fit.vutbr.cz www.fit.vut.cz AAAA +all;./dns -6 -r -s kazi.fit.vutbr.cz www.fit.vut.cz"
test21="AAAA www.github.com recursive; dig @kazi.fit.vutbr.cz www.github.com AAAA +all;./dns -6 -r -s kazi.fit.vutbr.cz www.github.com"
test22="AAAA nes.fit.vutbr.cz recursive; dig @kazi.fit.vutbr.cz nes.fit.vutbr.cz AAAA +all;./dns -6 -r -s kazi.fit.vutbr.cz nes.fit.vutbr.cz"
test23="AAAA www.fit.vut.cz non-recursive; dig @kazi.fit.vutbr.cz www.fit.vut.cz AAAA +all;./dns -6 -s kazi.fit.vutbr.cz www.fit.vut.cz"
test24="AAAA www.github.com non-recursive; dig @kazi.fit.vutbr.cz www.github.com AAAA +all;./dns -6 -s kazi.fit.vutbr.cz www.github.com"
test25="AAAA nes.fit.vutbr.cz non-recursive; dig @kazi.fit.vutbr.cz nes.fit.vutbr.cz AAAA +all;./dns -6 -s kazi.fit.vutbr.cz nes.fit.vutbr.cz"
test26="A overleaf.com ipv6 server non-recursive; dig @2606:4700:58::adf5:3b68 overleaf.com +all; ./dns -s 2606:4700:58::adf5:3b68 overleaf.com"
test27="A www.overleaf.com custom server non-recursive; dig @192.55.83.30 www.overleaf.com +all; ./dns -s 192.55.83.30 www.overleaf.com"
test28="A overleaf.com non-recursive; dig @kazi.fit.vutbr.cz overleaf.com +all; ./dns -s kazi.fit.vutbr.cz overleaf.com"
test29="A nes.fit.vutbr.cz Google DNS 1 non-recursive; dig @8.8.8.8 nes.fit.vutbr.cz +all; ./dns -s 8.8.8.8 nes.fit.vutbr.cz"
test30="A nes.fit.vutbr.cz Google DNS 2 non-recursive; dig @8.8.4.4 nes.fit.vutbr.cz +all; ./dns -s 8.8.4.4 nes.fit.vutbr.cz"

# Function to compare two records
compare_records() {
    local dns_record="$1"
    local dig_record="$2"

    # Parsing dig_record
    local dig_domain=$(echo "$dig_record" | awk '{print $1}' | tr -d '[:space:]')
    local dig_class=$(echo "$dig_record" | awk '{print $3}' | tr -d '[:space:]')
    local dig_type=$(echo "$dig_record" | awk '{print $4}' | tr -d '[:space:]')
    local dig_ip=$(echo "$dig_record" | awk '{print $5}' | tr -d '[:space:]')

    # Parsing dns_record
    local dns_domain=$(echo "$dns_record" | awk -F', ' '{print $1}' | tr -d '[:space:]')
    local dns_type=$(echo "$dns_record" | awk -F', ' '{print $2}' | tr -d '[:space:]')
    local dns_class=$(echo "$dns_record" | awk -F', ' '{print $3}' | tr -d '[:space:]')
    local dns_ip=$(echo "$dns_record" | awk -F', ' '{print $5}' | tr -d '[:space:]')

    # Comparing values
    if [ "$dig_domain" != "$dns_domain" ]; then
        return 1
    fi
    if [ "$dig_type" != "$dns_type" ]; then
        return 1
    fi
    if [ "$dig_ip" != "$dns_ip" ]; then
        return 1
    fi
    if [ "$dig_class" != "$dns_class" ]; then
        return 1
    fi

    return 0
}

# Function to split a string into an array by newlines
split_to_array() {
    local prefix=$1
    local string=$2
    local i=0
    while IFS= read -r line; do
        eval "$prefix$i='$line'"
        i=$((i + 1))
    done <<EOF
$string
EOF
}

# Colors
green='\033[0;32m'
red='\033[0;31m'
orange='\033[0;33m'
blue='\033[0;34m'
no_color='\033[0m'

index=1
failed=0
success=0
while eval "test=\$test$index" ; do
    [ -z "$test" ] && break

    # Splitting 'test' into components
    IFS=';' read -r test_name dig_cmd dns_cmd <<EOF
$test
EOF

    # Save the output of dig command into a variable
    dig_output=$(eval "$dig_cmd")

    dig_answer_count=$(echo "$dig_output" |  awk 'match($0, /ANSWER: ([0-9]+)/) {print substr($0, RSTART + 8, RLENGTH - 8)}')
    dig_authority_count=$(echo "$dig_output" |  awk 'match($0, /AUTHORITY: ([0-9]+)/) {print substr($0, RSTART + 11, RLENGTH - 11)}')
    dig_additional_count=$(echo "$dig_output" |  awk 'match($0, /ADDITIONAL: ([0-9]+)/) {print substr($0, RSTART + 12, RLENGTH - 12)}')

    dig_answer=$(echo "$dig_output" | awk '/^;; ANSWER SECTION:/,/^$/ { if (!/;; ANSWER SECTION:/ && !/^$/) print }')

    # Save the output of test command into a variable
    dns_output=$(eval "$dns_cmd")

    # validate output (check for sigsegv, infinite loop, etc.)
    dns_question_count=$(echo "$dns_output" | awk -F '[()]' '/Questions/{print $2}')
    dns_answer_count=$(echo "$dns_output" | awk -F '[()]' '/Answer section/{print $2}')
    dns_authority_count=$(echo "$dns_output" | awk -F '[()]' '/Authority section/{print $2}')
    dns_additional_count=$(echo "$dns_output" | awk -F '[()]' '/Additional section/{print $2}')

    if [ "$dns_question_count" == "" ]; then
        echo -e "$index. $test_name $red FAILED $no_color"
        failed=$((failed + 1))
        index=$((index + 1))
        continue
    fi

    dns_answer=$(echo "$dns_output" | awk '/^Answer section \(/ {flag=1; next} /Authority section \(/ {flag=0} flag')

    # Convert outputs into arrays of records
    split_to_array "dig_answer_record_" "$dig_answer"
    split_to_array "dns_answer_record_" "$dns_answer"

    result=false

    # Compare counts
    if [ "$dig_answer_count" == "$dns_answer_count" ]; then
        result=true
    fi

    # sometimes dig fails to retrieve records, but dns does not
    if [ "$dig_answer_count" == "0" ] && [ "$dns_answer_count" != "0" ]; then
        result=true
    fi

    # Compare each record in the ANSWER section
    i=0
    while eval "dig_record=\$dig_answer_record_$i" && [ -n "$dig_record" ]; do
        eval "dns_record=\$dns_answer_record_$i"
        if compare_records "$dns_record" "$dig_record"; then
            result=true
        fi
        i=$((i + 1))
    done


    if [ "$result" = "true" ]; then
         echo -e "$blue""$index. $test_name $green OK $no_color"
         success=$((success + 1))
     else
         echo -e "$blue""$index. $test_name $red FAILED $no_color"
         failed=$((failed + 1))
     fi
     index=$((index + 1))
done

echo -e "$green""Tests passed: $success $no_color"
echo -e "$red""Tests failed: $failed $no_color"
