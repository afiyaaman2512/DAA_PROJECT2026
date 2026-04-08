#ifndef SMART_H
#define SMART_H

#include <string>
#include <vector>
#include <queue>

// Levenshtein distance between two strings
int levenshtein(const std::string& a, const std::string& b);

// Given a broken URL, suggest the closest match from a list of known good URLs
std::string suggestFix(const std::string& brokenURL, 
                        const std::vector<std::string>& knownURLs);

// Check if a URL's domain is in the blacklist file
bool isMalicious(const std::string& url, 
                  const std::string& blacklistPath = "data/blacklist.txt");

// Priority item — higher priority = scanned first
struct URLTask {
    std::string url;
    int priority;  // higher = more urgent

    // Min-heap flipped to max-heap
    bool operator<(const URLTask& other) const {
        return priority < other.priority;
    }
};

// Assign priority score to a URL
// /login, /checkout, /payment get priority 10
// /admin, /account get priority 8
// everything else gets priority 1
int assignPriority(const std::string& url);

// Build a priority queue from a list of URLs
std::priority_queue<URLTask> buildPriorityQueue(
    const std::vector<std::string>& urls);

#endif
