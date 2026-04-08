#include <iostream>
#include "checker.h"

int main() {
    std::cout << "LinkShield starting..." << std::endl;

    // Test URLs
    std::string urls[] = {
        "https://www.google.com",
        "https://httpstat.us/404",
        "https://httpstat.us/500"
    };

    for (const auto& url : urls) {
        int code = checkURL(url);
        std::cout << "[" << code << "] " << url << std::endl;
    }

    return 0;
}