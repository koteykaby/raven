#include <thread>

#include "macros.hpp"
#include "services/http.hpp"
#include "services/udp.hpp"

int main()
{
    int http_port = 80;
    int grfs_auth_port = 21030;
    int grfs_secure_port = 21031;

    std::thread onlineconfigservice(startTCP, http_port, std::string("onlineconfig"));
    std::thread grfs_auth(serveUDP, grfs_auth_port, std::string("grfs_auth"));
    std::thread grfs_secure(serveUDP, grfs_secure_port, std::string("grfs_secure"));

    std::cin.get();

    onlineconfigservice.join();
    grfs_auth.join();
    grfs_secure.join();

    return 0;
}