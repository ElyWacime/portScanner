#include "portScanner.hpp"
#include <cstdio>

int main(void) {

  PortScanner instance;
  std::string ip = "127.0.0.2";
  printf("%s\n", instance.grabBanner(ip, 8080).c_str());

  return 0;
}
