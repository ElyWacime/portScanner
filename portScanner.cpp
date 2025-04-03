#include "portScanner.hpp"
#include <arpa/inet.h> // For inet_pton, sockaddr_in
#include <errno.h>     // For errno (error codes)
#include <fcntl.h>     // For fcntl() to set non-blocking mode
#include <fstream>
#include <functional>
#include <iostream>
#include <netinet/in.h>
#include <string.h>
#include <string>
#include <sys/socket.h> // For socket functions
#include <sys/socketvar.h>
#include <thread>
#include <unistd.h> // For close()

// constructor
PortScanner::PortScanner(std::string filePath) {
  std::cout << "constructor called;" << std::endl;
  this->errorOutFile.open(filePath.c_str(), std::ios::out);
  if (!this->errorOutFile.is_open())
    std::cerr << "can't open error file for output: " << filePath << std::endl;
  this->ip = "127.0.0.1";
  for (int i = 0; i < 65535; i++) {
    this->ports.push_back(i);
  }
}

PortScanner::PortScanner(std::string ip, int port, std::string filePath) {
  std::cout << "Single port constructor called;" << std::endl;
  this->errorOutFile.open(filePath.c_str(), std::ios::out);
  this->ip = ip;
  this->ports.push_back(port);
}

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
  for (std::vector<int>::iterator it = ports.begin(); it != ports.end(); it++) {
    threadsGrabBanner.push_back(
        std::thread(std::bind(&PortScanner::routine, this, *it)));
  }
  for (std::vector<std::thread>::iterator it = threadsGrabBanner.begin();
       it != threadsGrabBanner.end(); ++it) {
    it->join();
  }
  return;
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
    }
  }
}
