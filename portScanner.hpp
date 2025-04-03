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
  std::string ip;
  std::vector<int> ports;

public:
  // constractions
  PortScanner();
  PortScanner(std::string ip, int port);
  PortScanner(std::string ip, int start, int end);
  ~PortScanner();
  // scanning functions
  void scanAvailablePorts();
  void displayScanResult() const;

  // utils functions
  std::string checkIfServerSendToMeFirst(std::string ip, int port);
  std::string grabBanner(const std::string &ip, int port);
  bool checkForHTTP(std::string ip, int port);
  bool checkForSSH(std::string ip, int port);
  bool checkForFTP(std::string ip, int port);

  // getters;
  std::map<int, std::string> getScanResult() const { return this->scanResults; }
};

std::ostream &operator<<(std::ostream &out, const PortScanner &instance);
