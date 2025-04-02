#include "portScanner.hpp"
#include <arpa/inet.h> // For inet_pton, sockaddr_in
#include <errno.h>     // For errno (error codes)
#include <fcntl.h>     // For fcntl() to set non-blocking mode
#include <iostream>
#include <netinet/in.h>
#include <string.h>
#include <string>
#include <sys/socket.h> // For socket functions
#include <sys/socketvar.h>
#include <unistd.h> // For close()

// constructor
PortScanner::PortScanner() {
  std::cout << "constructor called;" << std::endl;
  this->ip = "127.0.0.1";
  for (int i = 0; i < 65535; i++) {
    this->ports.push_back(i);
  }
}

PortScanner::PortScanner(std::string ip, int port) {
  std::cout << "Single port constructor called;" << std::endl;
  this->ip = ip;
  this->ports.push_back(port);
}

PortScanner::PortScanner(std::string ip, int start, int end) {
  std::cout << "Multipple ports constructor called;" << std::endl;
  this->ip = ip;
  for (int i = start; i < end; i++) {
    this->ports.push_back(i);
  }
}

PortScanner::~PortScanner() { std::cout << "destructor called;" << std::endl; }

// utils functions
std::string PortScanner::grabBanner(const std::string &ip, int port) {

  std::string state = checkIfServerSendToMeFirst(ip, port);
  if (state != "nothing detected from waiting;")
    return state;
  if (checkForFTP(ip, port))
    return "ftp";
  if (checkForSSH(ip, port))
    return "ssh";
  if (checkForHTTP(ip, port))
    return "HTTP";

  return "No service detecled;";
}

// scanning functions
void PortScanner::scanSinglePort() {
  for (std::vector<int>::iterator it = ports.begin(); it != ports.end(); it++) {
    std::string state = grabBanner(this->ip, this->ports[*it]);
    if (state != "No service detecled;") {
      this->scanResults[this->ports[*it]] = "open\t" + state;
    } else {
      this->scanResults[this->ports[*it]] = "closed";
    }
  }
  return;
}

void PortScanner::displayScanResult() const {
  std::cout << "###################################" << std::endl;
  std::cout << "########## SCAN RESULTS ###########" << std::endl;
  std::cout << "###################################" << std::endl;
  std::cout << "IP: " + this->ip << std::endl;
  for (std::vector<int>::const_iterator it = ports.begin(); it != ports.end();
       ++it) {
    std::cout << scanResults.at(*it) << std::endl;
  }
}
