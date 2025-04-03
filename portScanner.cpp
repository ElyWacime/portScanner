#include "portScanner.hpp"
#include <arpa/inet.h> // For inet_pton, sockaddr_in
#include <cstddef>
#include <errno.h> // For errno (error codes)
#include <fcntl.h> // For fcntl() to set non-blocking mode
#include <fstream>
#include <functional>
#include <iostream>
#include <netinet/in.h>
#include <regex>
#include <string.h>
#include <string>
#include <sys/socket.h> // For socket functions
#include <sys/socketvar.h>
#include <thread>
#include <unistd.h> // For close()

// constructor
PortScanner::PortScanner(std::string ip, int start, int end,
                         std::string filePath) {
  std::cout << "Multipple ports constructor called;" << std::endl;
  this->errorOutFile.open(filePath.c_str(), std::ios::out);
  this->ip = ip;
  for (int i = start; i <= end; i++) {
    this->ports.push_back(i);
  }
}

PortScanner::~PortScanner() {
  std::cout << "destructor called;" << std::endl;
  this->errorOutFile.close();
}

// utils functions
std::string PortScanner::grabBanner(int port) {

  std::string state = checkIfServerSendToMeFirst(this->ip, port);
  if (state != "nothing detected from waiting;")
    return state;
  if (checkForFTP(this->ip, port))
    return "ftp";
  if (checkForSSH(this->ip, port))
    return "ssh";
  if (checkForHTTP(this->ip, port))
    return "HTTP";

  return "No service detecled;";
}

// scanning functions
void PortScanner::routine(int port) {
  std::string state = grabBanner(port);
  this->lock.lock();
  if (state != "No service detecled;" && state != "Connection failed;") {
    this->scanResults[port] = "open\t" + state;
  } else {
    this->scanResults[port] = "closed";
  }
  this->lock.unlock();
}

void PortScanner::scanAvailablePorts() {
  const int portRange = 10000;
  size_t totalPorts = this->ports.size();
  for (size_t i = 0; i < totalPorts; i += portRange) {
    int start = i;
    int end = std::min(i + portRange, totalPorts);
    for (int j = start; j < end; j++) {
      threadsGrabBanner.push_back(
          std::thread(std::bind(&PortScanner::routine, this, this->ports[j])));
    }
    for (std::vector<std::thread>::iterator it = threadsGrabBanner.begin();
         it != threadsGrabBanner.end(); ++it) {
      it->join();
    }
    this->threadsGrabBanner.clear();
  }
}

void PortScanner::displayScanResult() const {
  std::cout << "###################################" << std::endl;
  std::cout << "########## SCAN RESULTS ###########" << std::endl;
  std::cout << "###################################\n" << std::endl;
  std::cout << "IP: " + this->ip << std::endl;

  for (std::map<int, std::string>::const_iterator it = scanResults.begin();
       it != scanResults.end(); ++it) {
    size_t found = it->second.find("open");
    if (found != std::string::npos) {
      std::cout << "port: " << it->first << "\tstatus: " << it->second
                << std::endl;
    } else {
      this->errorOutFile << "port: " << it->first << "\tstatus: " << it->second
                         << std::endl;
    }
  }
}
