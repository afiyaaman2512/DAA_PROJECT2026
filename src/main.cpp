#include <iostream>
#include "crawler.h"
#include "smart.h"

int main() {
    std::cout << "LinkShield Phase 5 - Smart Fix & Malicious Detection\n\n";

    // ── Test 1: Levenshtein suggestions ──────────────────────────────
    std::cout << "=== Typo Suggestions ===\n";
    std::vector<std::string> knownURLs = {
        "https://example.com/about",
        "https://example.com/contact",
        "https://example.com/login",
        "https://example.com/checkout"
    };

    std::string broken = "https://example.com/abuot";  // typo: about
    std::string suggestion = suggestFix(broken, knownURLs);
    std::cout << "Broken:     " << broken << "\n";
    std::cout << "Suggested:  " << (suggestion.empty() ? "no match" : suggestion) << "\n\n";

    // ── Test 2: Blacklist detection ───────────────────────────────────
    std::cout << "=== Malicious Domain Check ===\n";
    std::vector<std::string> testURLs = {
        "https://google.com/search",
        "https://malware.com/download",
        "https://example.com/page",
        "https://phishing-site.net/login"
    };

    for (const auto& url : testURLs) {
        bool toxic = isMalicious(url);
        std::cout << (toxic ? "[TOXIC]  " : "[CLEAN]  ") << url << "\n";
    }

    // ── Test 3: Priority queue ────────────────────────────────────────
    std::cout << "\n=== Priority Queue (scan order) ===\n";
    std::vector<std::string> pagesToScan = {
        "https://example.com/about",
        "https://example.com/login",
        "https://example.com/blog",
        "https://example.com/checkout",
        "https://example.com/contact",
        "https://example.com/admin"
    };

    auto pq = buildPriorityQueue(pagesToScan);
    while (!pq.empty()) {
        auto task = pq.top(); pq.pop();
        std::cout << "[Priority " << task.priority << "] " 
                  << task.url << "\n";
    }

    return 0;
}
