# 📎DNS Resolver

Custom implementation of DNS Resolver in C++ for ISA (Network Applications and Network Administration) course at FIT VUT Brno.
### 🐈‍⬛ Version control:
https://github.com/xchmel33/DNSResolver

### 📝 Author: 
Lukas Chmelo

####
## 🚀 Usage

####
### 🛠 Build
To build DNS Resolver:
```bash
make all
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
make test
```
