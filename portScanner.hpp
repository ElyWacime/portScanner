#pragma once

#include <arpa/inet.h> // For inet_pton, sockaddr_in
#include <errno.h>     // For errno (error codes)
#include <fcntl.h>     // For fcntl() to set non-blocking mode
#include <iostream>
#include <map>
#include <ostream>
#include <string>
#include <sys/socket.h> // For socket functions
#include <sys/socketvar.h>
#include <unistd.h> // For close()
#include <vector>

class PortScanner {
private:
  std::string fileToStoreResults;
  std::map<int, std::string> scanResults;

public:
  // constractions
  PortScanner();
  ~PortScanner();
  // scanning functions
  void scanAllPorts(const std::string &ip);
  void scanSinglePort(const std::string &ip, int port);
  void scanListOfPorts(const std::string &ip, std::vector<int> ports);
  void scanRangeOfPorts(const std::string &ip, int start, int end);
  // utils functions
  std::string grabBanner(const std::string &ip, int port);
  bool checkForHTTP(int sock, std::string ip);
  bool checkForSSH(int sock);
  bool checkForFTP(int sock);
};

std::ostream &operator<<(std::ostream &out, const PortScanner &instance);
