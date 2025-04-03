#include "portScanner.hpp"
#include <cstdio>

int main(void) {

  PortScanner instance("127.0.0.1", 10, 100);
  // std::string ip = "127.0.0.1";
  // printf("%s\n", instance.grabBanner(ip, 22).c_str());
  // printf(">>>> : %s\n",
  // instance.checkIfServerSendToMeFirst("127.0.0.1", 22).c_str());
  instance.scanAvailablePorts();
  instance.displayScanResult();
  // printf("%d\n", instance.checkForHTTP(instance.getIP(), 80));
  // std::cout << instance << std::endl;

  return 0;
}
