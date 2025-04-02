#include "portScanner.hpp"
#include <cstdio>

int main(void) {

  PortScanner instance("127.0.0.1", 22);
  // std::string ip = "127.0.0.1";
  // printf("%s\n", instance.grabBanner(ip, 22).c_str());
  std::cout << "<<<<<<<<<>>>>>>>>:::" << instance.getScanResult()[0]
            << std::endl;
  instance.displayScanResult();

  return 0;
}
