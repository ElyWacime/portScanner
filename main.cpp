#include "portScanner.hpp"
#include <cstdio>

int main(void) {

  PortScanner instance("djenty.local");
  // std::string ip = "127.0.0.1";
  // printf("%s\n", instance.grabBanner(ip, 22).c_str());
  // printf(">>>> : %s\n",
  // instance.checkIfServerSendToMeFirst("127.0.0.1", 22).c_str());
  instance.scanAvailablePorts();
  instance.displayScanResult();

  return 0;
}
