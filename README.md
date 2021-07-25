# NetworkFileDownloader
A simple application that will allow clients to download files from a server based on the filename. Current a single client is supported.

**Usage**:
1. Compile the logger
`g++ -c logger.cpp`
2. Compile the server and client
`g++ logger.o server.cpp -o server`
`g++ logger.o client.cpp -o client`
3. Execute server
`./server`
4. Execute client
`./client`

To make life easier, makefile will be available soon!

**Future work**
1. Support multithreading.
2. File transfer in blocks.
3. Use OOP.
