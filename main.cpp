#include "portScanner.hpp"
#include <cstdio>

int main(void) {

  PortScanner instance;
  std::string ip = "127.0.0.1";
  printf("%s\n", instance.grabBanner(ip, 21).c_str());

  return 0;
}
