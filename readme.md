# 📎DNS Resolver

This tool sends DNS queries to specified servers and analyzes the response.
### 🐈‍⬛ Version control:
https://github.com/xchmel33/DNSResolver

### 📝 Author: 
Lukas Chmelo

####
## 🚀 Usage

####
### 🛠 Building
To build everything:
```bash
make all
```
To build the main program:
```bash
make
```
To build tests:
```bash
make tests
```
To clean up generated binaries:
```bash
make clean
```

####
### ▶️ Execution
Run main program:
```bash
./dns [-r] [-x] [-6] -s server [-p port] address
```
`-r`: **(Optional)** Request recursion. <br>
`-x`: **(Optional)** Perform a reverse query instead of a direct one. <br>
`-6`: **(Optional)** Query for a AAAA record instead of the default A. <br>
`-s`: **(Required)** Specify the IP address or domain name of the server to send the query to. <br>
`-p`: **(Optional)** Specify the port number to send the query to (default is 53). <br>
`address`: **(Required)** The address you are querying. <br><br>

Run tests:
```bash
./tests
```
