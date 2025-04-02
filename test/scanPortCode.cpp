#include <iostream>    // For std::cout, std::cerr
#include <sys/socket.h> // For socket functions
#include <arpa/inet.h>  // For inet_pton, sockaddr_in
#include <unistd.h>     // For close()
#include <fcntl.h>      // For fcntl() to set non-blocking mode
#include <errno.h>      // For errno (error codes)

bool isPortOpen(const std::string &ip, int port, int timeout = 2) {
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        std::cerr << "Error creating socket\n";
        return false;
    }

    sockaddr_in target{};
    target.sin_family = AF_INET;
    target.sin_port = htons(port);
    inet_pton(AF_INET, ip.c_str(), &target.sin_addr);

    fcntl(sock, F_SETFL, O_NONBLOCK);

    int result = connect(sock, (struct sockaddr *)&target, sizeof(target));
    if (result == 0) {
        close(sock);
        return true;
    }

    if (errno == EINPROGRESS) {
        fd_set set;
        FD_ZERO(&set);
        FD_SET(sock, &set);

        timeval timeoutStruct{};
        timeoutStruct.tv_sec = timeout;
        timeoutStruct.tv_usec = 0;

        if (select(sock + 1, nullptr, &set, nullptr, &timeoutStruct) > 0) {
            int error;
            socklen_t len = sizeof(error);
            getsockopt(sock, SOL_SOCKET, SO_ERROR, &error, &len);

            close(sock);
            return (error == 0);
        }
    }

    close(sock);
    return false;
}

int main() {
    std::string ip = "192.168.1.1";
    int port = 80;

    if (isPortOpen(ip, port)) {
        std::cout << "Port " << port << " is open!\n";
    } else {
        std::cout << "Port " << port << " is closed or filtered.\n";
    }

    return 0;
}

