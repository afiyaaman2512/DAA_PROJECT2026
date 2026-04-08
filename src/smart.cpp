#include "smart.h"
#include <algorithm>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <climits>

// ── Levenshtein distance ──────────────────────────────────────────────
// Classic DP approach: O(m*n) time, O(m*n) space
int levenshtein(const std::string& a, const std::string& b) {
    int m = a.size(), n = b.size();
    
    // dp[i][j] = edit distance between a[0..i-1] and b[0..j-1]
    std::vector<std::vector<int>> dp(m + 1, std::vector<int>(n + 1, 0));

    // Base cases: transforming to/from empty string
    for (int i = 0; i <= m; i++) dp[i][0] = i;
    for (int j = 0; j <= n; j++) dp[0][j] = j;

    for (int i = 1; i <= m; i++) {
        for (int j = 1; j <= n; j++) {
            if (a[i-1] == b[j-1]) {
                dp[i][j] = dp[i-1][j-1];       // no op needed
            } else {
                dp[i][j] = 1 + std::min({
                    dp[i-1][j],     // deletion
                    dp[i][j-1],     // insertion
                    dp[i-1][j-1]    // substitution
                });
            }
        }
    }
    return dp[m][n];
}

// ── Suggest closest fix ───────────────────────────────────────────────
std::string suggestFix(const std::string& brokenURL,
                        const std::vector<std::string>& knownURLs) {
    if (knownURLs.empty()) return "";

    std::string best = knownURLs[0];
    int bestDist = INT_MAX;

    for (const auto& url : knownURLs) {
        int dist = levenshtein(brokenURL, url);
        if (dist < bestDist) {
            bestDist = dist;
            best = url;
        }
    }

    // Only suggest if reasonably close (threshold = 10)
    if (bestDist > 10) return "";
    return best;
}

// ── Extract domain from URL ───────────────────────────────────────────
static std::string extractDomain(const std::string& url) {
    size_t start = url.find("://");
    if (start == std::string::npos) return url;
    start += 3;
    size_t end = url.find("/", start);
    if (end == std::string::npos) return url.substr(start);
    return url.substr(start, end - start);
}

// ── Blacklist check ───────────────────────────────────────────────────
bool isMalicious(const std::string& url, const std::string& blacklistPath) {
    std::ifstream file(blacklistPath);
    if (!file.is_open()) {
        std::cerr << "[Warning] Could not open blacklist: " 
                  << blacklistPath << std::endl;
        return false;
    }

    std::string domain = extractDomain(url);
    std::string line;

    while (std::getline(file, line)) {
        // Trim whitespace
        line.erase(0, line.find_first_not_of(" \t\r\n"));
        line.erase(line.find_last_not_of(" \t\r\n") + 1);

        if (!line.empty() && domain == line) return true;
    }
    return false;
}

// ── Priority assignment ───────────────────────────────────────────────
int assignPriority(const std::string& url) {
    // Sensitive financial/auth pages → highest priority
    if (url.find("/login")    != std::string::npos ||
        url.find("/checkout") != std::string::npos ||
        url.find("/payment")  != std::string::npos)  return 10;

    // Admin/account pages → high priority
    if (url.find("/admin")   != std::string::npos ||
        url.find("/account") != std::string::npos)   return 8;

    // Everything else → normal
    return 1;
}

// ── Build priority queue ──────────────────────────────────────────────
std::priority_queue<URLTask> buildPriorityQueue(
    const std::vector<std::string>& urls) {

    std::priority_queue<URLTask> pq;
    for (const auto& url : urls) {
        pq.push({url, assignPriority(url)});
    }
    return pq;
}
