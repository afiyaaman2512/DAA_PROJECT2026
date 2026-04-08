#pragma once
#include <string>
#include <vector>
#include <map>

struct PageResult {
    std::string url;
    int statusCode;
    std::vector<std::string> links;  // outgoing links from this page
};

// Main BFS crawler function
// startURL   - where to begin
// maxDepth   - how many levels deep to crawl
// maxPages   - hard cap to avoid infinite crawling
std::vector<PageResult> crawl(const std::string& startURL, 
                               int maxDepth = 2, 
                               int maxPages = 20);
                               