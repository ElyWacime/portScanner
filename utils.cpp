#include "portScanner.hpp"
#include <arpa/inet.h>
#include <cstring>
#include <iostream>
#include <sys/socket.h>
#include <unistd.h>
#include <vector>

bool PortScanner::checkForHTTP(std::string ip, int port) {
  // create a socket
  int sock = socket(AF_INET, SOCK_STREAM, 0);
  if (sock < 0) {
    std::cerr << "check for http: Error creating socket;";
    return false;
  }

  // set up the target sockaddr_in structure
  sockaddr_in target{};
  target.sin_family = AF_INET;
  target.sin_port = htons(port);
  inet_pton(AF_INET, ip.c_str(), &target.sin_addr);

  // attempt a connection to the target
  if (connect(sock, (struct sockaddr *)&target, sizeof(target)) < 0) {
    close(sock);
    std::cerr << "check for http: Connection failed;";
    return false;
  }
  // Set a timeout for recv()
  struct timeval timeout;
  timeout.tv_sec = 1; // Timeout after 5 seconds
  timeout.tv_usec = 0;
  setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout));

  // send a basic http header to see if the server talks http protocol
  std::string header = "GET / HTTP/1.1\r\nHost: " + ip + "\r\n\r\n";
  send(sock, header.c_str(), strlen(header.c_str()), 0);

  // read the benner response to see if the target speaks http protocol
  char buffer[1024];
  memset(buffer, 0, sizeof(buffer));
  int bytesReceived = recv(sock, buffer, sizeof(buffer), 0);
  if (bytesReceived != 0 && strncmp(buffer, "HTTP/", 5) == 0)
    return true;
  else if (bytesReceived == 0)
    std::cerr << "http check: Connection closed by server.\n";
  else {
    std::cerr << "http check: Recv timed out or failed.\n";
  }
  return false;
}

std::string PortScanner::checkIfServerSendToMeFirst(std::string ip, int port) {
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
  // Set a timeout for recv()
  struct timeval timeout;
  timeout.tv_sec = 2; // Timeout after 5 seconds
  timeout.tv_usec = 0;
  setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout));

  char buffer[1024];
  memset(buffer, 0, sizeof(buffer));
  int bytesReceived = recv(sock, buffer, sizeof(buffer), 0);

  if (bytesReceived > 0 && strncmp(buffer, "220", 3) == 0) {
    close(sock);
    return "ftp"; // FTP detected
  } else if (bytesReceived > 0 && strncmp(buffer, "ssh-", 4) == 0) {
    close(sock);
    return "ssh";
  } else if (bytesReceived == 0) {
    std::cerr << "waiting from server check: Connection closed by server.\n";
  } else {
    std::cerr << "waiting from server check: Recv timed out or failed.\n";
  }
  close(sock);
  return "nothing detected from waiting;";
}

bool PortScanner::checkForFTP(std::string ip, int port) {
  // create a socket
  int sock = socket(AF_INET, SOCK_STREAM, 0);
  if (sock < 0) {
    std::cerr << "check fot ftp: Error creating socket;";
    return false;
  }

  // set up the target sockaddr_in structure
  sockaddr_in target{};
  target.sin_family = AF_INET;
  target.sin_port = htons(port);
  inet_pton(AF_INET, ip.c_str(), &target.sin_addr);

  // attempt a connection to the target
  if (connect(sock, (struct sockaddr *)&target, sizeof(target)) < 0) {
    close(sock);
    std::cerr << "check for ftp: Connection failed;";
    return false;
  }

  // Set a timeout for recv()
  struct timeval timeout;
  timeout.tv_sec = 1; // Timeout after 5 seconds
  timeout.tv_usec = 0;
  setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout));

  char buffer[1024];
  memset(buffer, 0, sizeof(buffer));
  int bytesReceived = recv(sock, buffer, sizeof(buffer), 0);

  if (bytesReceived > 0 && strncmp(buffer, "220", 3) == 0) {
    close(sock);
    return true; // FTP detected
  } else if (bytesReceived == 0) {
    close(sock);
    std::cerr << "ftp check: Connection closed by server.\n";
  } else {
    std::cerr << "ftp check: Recv timed out or failed.\n";
  }
  close(sock);
  return false;
}

bool PortScanner::checkForSSH(std::string ip, int port) {
  // create a socket
  int sock = socket(AF_INET, SOCK_STREAM, 0);
  if (sock < 0) {
    std::cerr << "check for ssh: Error creating socket;";
    return false;
  }

  // set up the target sockaddr_in structure
  sockaddr_in target{};
  target.sin_family = AF_INET;
  target.sin_port = htons(port);
  inet_pton(AF_INET, ip.c_str(), &target.sin_addr);

  // attempt a connection to the target
  if (connect(sock, (struct sockaddr *)&target, sizeof(target)) < 0) {
    close(sock);
    std::cerr << "check for ssh: Connection failed;";
    return false;
  }

  // Set a timeout for recv()
  struct timeval timeout;
  timeout.tv_sec = 1; // Timeout after 5 seconds
  timeout.tv_usec = 0;
  setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout));

  std::string ssh_probe = "SSH-2.0-Scanner\r\n";
  send(sock, ssh_probe.c_str(), ssh_probe.size(), 0);

  char buffer[1024];
  memset(buffer, 0, sizeof(buffer));
  int bytesReceived = recv(sock, buffer, sizeof(buffer) - 1, 0);

  if (bytesReceived > 0 && strncmp(buffer, "SSH-", 4) == 0) {
    close(sock);
    return true; // SSH detected
  } else if (bytesReceived == 0) {
    std::cerr << "ssh check: Connection closed by server.\n";
  } else {
    std::cerr << "ssh check: Recv timed out or failed.\n";
  }
  close(sock);
  return false;
}
