#pragma once

#include <arpa/inet.h> // For inet_pton, sockaddr_in
#include <errno.h>     // For errno (error codes)
#include <fcntl.h>     // For fcntl() to set non-blocking mode
#include <fstream>
#include <iostream>
#include <map>
#include <mutex>
#include <ostream>
#include <string>
#include <sys/socket.h> // For socket functions
#include <sys/socketvar.h>
#include <thread>
#include <unistd.h> // For close()
#include <vector>

class PortScanner {
private:
  mutable std::fstream errorOutFile;
  std::map<int, std::string> scanResults;
  std::string ip;
  std::vector<int> ports;
  std::mutex lock;
  std::vector<std::thread> threadsGrabBanner;

public:
  // constractions
  // PortScanner(std::string filePath = "output.txt");
  // PortScanner(std::string ip, int port, std::string filePath = "output.txt");
  PortScanner(std::string ip, int start, int end,
              std::string filePath = "output.txt");
  ~PortScanner();
  // scanning functions
  void scanAvailablePorts();
  void displayScanResult() const;

  // utils functions
  std::string checkIfServerSendToMeFirst(std::string ip, int port);
  std::string grabBanner(int port);
  void routine(int port);
  bool checkForHTTP(std::string ip, int port);
  bool checkForSSH(std::string ip, int port);
  bool checkForFTP(std::string ip, int port);

  // getters;
  std::map<int, std::string> getScanResult() const { return this->scanResults; }
  std::string getIP() const { return this->ip; };
};

std::ostream &operator<<(std::ostream &out, const PortScanner &instance);
