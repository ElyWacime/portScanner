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
PortScanner::PortScanner() { std::cout << "constructor called;" << std::endl; }

PortScanner::~PortScanner() { std::cout << "destructor called;" << std::endl; }

// utils functions
std::string PortScanner::grabBanner(const std::string &ip, int port) {
  // create a socket
  int sock = socket(AF_INET, SOCK_STREAM, 0);
  if (sock < 0) {
    return "Error creating socket;";
  }

  // set up the target sockaddr_in structure
  sockaddr_in target{};
  target.sin_family = AF_INET;
  target.sin_port = htons(port);
  inet_pton(AF_INET, ip.c_str(), &target.sin_addr);

  // attempt a connection to the target
  if (connect(sock, (struct sockaddr *)&target, sizeof(target)) < 0) {
    close(sock);
    return "Connection failed;";
  }

  if (checkForHTTP(sock, ip)) {
    close(sock);
    return "HTTP";
  }
  if (checkForFTP(sock)) {
    close(sock);
    return "ftp";
  }
  if (checkForSSH(sock)) {
    close(sock);
    return "ssh";
  }

  // close the socket
  close(sock);
  return "No service detecled;";
}

bool PortScanner::checkForHTTP(int sock, std::string ip) {
  // send a basic http header to see if the server talks http protocol
  std::string header = "GET / HTTP/1.1\r\nHost: " + ip + "\r\n\r\n";
  send(sock, header.c_str(), strlen(header.c_str()), 0);

  // read the benner response to see if the target speaks http protocol
  char buffer[1024];
  memset(buffer, 0, sizeof(buffer));
  recv(sock, buffer, sizeof(buffer), 0);
  if (strncmp(buffer, "HTTP/", 5) == 0)
    return true;
  else
    return false;
}

bool PortScanner::checkForFTP(int sock) {
  char buffer[1024];
  memset(buffer, 0, sizeof(buffer));
  recv(sock, buffer, sizeof(buffer), 0);

  return (strncmp(buffer, "220", 3) == 0);
}

bool PortScanner::checkForSSH(int sock) {
  char buffer[1024];
  memset(buffer, 0, sizeof(buffer));
  recv(sock, buffer, sizeof(buffer), 0);

  return (strncmp(buffer, "SSH-", 4) == 0);
}

// scanning functions
void PortScanner::scanSinglePort(const std::string &ip, int port) {
  (void)port;
  (void)ip;
  return;
}
