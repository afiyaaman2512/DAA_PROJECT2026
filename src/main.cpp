#include <iostream>
#include <string>
#include "checker.h"
#include "parser.h"
#include <curl/curl.h>

// Callback function so curl can collect HTML into a string
static size_t writeCallback(void* contents, size_t size, 
                             size_t nmemb, std::string* output) {
    output->append((char*)contents, size * nmemb);
    return size * nmemb;
}

// Fetches full HTML of a page (GET request)
std::string fetchHTML(const std::string& url) {
    CURL* curl = curl_easy_init();
    std::string html;

    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &html);
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
        curl_easy_setopt(curl, CURLOPT_TIMEOUT, 10L);
        curl_easy_perform(curl);
        curl_easy_cleanup(curl);
    }
    return html;
}

int main() {
    std::cout << "LinkShield starting..." << std::endl;

    std::string testURL = "https://example.com";
    
    std::cout << "\nFetching: " << testURL << std::endl;
    std::string html = fetchHTML(testURL);
    
    std::cout << "Extracting links..." << std::endl;
    std::vector<std::string> links = extractLinks(html, testURL);

    std::cout << "Found " << links.size() << " links:" << std::endl;
    for (const auto& link : links) {
        int code = checkURL(link);
        std::cout << "[" << code << "] " << link << std::endl;
    }

    return 0;
}