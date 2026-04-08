#include "crawler.h"
#include "checker.h"
#include "parser.h"
#include <curl/curl.h>
#include <queue>
#include <set>
#include <map>
#include <iostream>

// ── same writeCallback you already have in main.cpp ──────────────────
static size_t writeCallback(void* contents, size_t size,
                             size_t nmemb, std::string* output) {
    output->append((char*)contents, size * nmemb);
    return size * nmemb;
}

static std::string fetchHTML(const std::string& url) {
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

// ── helper: extract domain from a URL ────────────────────────────────
// "https://example.com/page" → "example.com"
static std::string getDomain(const std::string& url) {
    size_t start = url.find("://");
    if (start == std::string::npos) return url;
    start += 3;
    size_t end = url.find("/", start);
    if (end == std::string::npos) return url.substr(start);
    return url.substr(start, end - start);
}

// ── BFS crawler ───────────────────────────────────────────────────────
std::vector<PageResult> crawl(const std::string& startURL,
                               int maxDepth, int maxPages) {

    std::vector<PageResult> results;

    // BFS frontier: stores {url, depth}
    std::queue<std::pair<std::string, int>> frontier;

    // Visited set — never crawl the same URL twice
    std::set<std::string> visited;

    // Adjacency list — graph of who links to whom
    std::map<std::string, std::vector<std::string>> graph;

    std::string baseDomain = getDomain(startURL);

    frontier.push({startURL, 0});
    visited.insert(startURL);

    while (!frontier.empty() && (int)results.size() < maxPages) {

        auto [currentURL, depth] = frontier.front();
        frontier.pop();

        std::cout << "\n[Crawling] " << currentURL 
                  << "  (depth " << depth << ")" << std::endl;

        // ── fetch & parse ─────────────────────────────────────────
        std::string html = fetchHTML(currentURL);
        std::vector<std::string> links = extractLinks(html, currentURL);

        // ── build this page's result ──────────────────────────────
        PageResult page;
        page.url = currentURL;
        page.statusCode = checkURL(currentURL);
        page.links = links;
        results.push_back(page);

        // ── store in adjacency list (graph) ───────────────────────
        graph[currentURL] = links;

        // ── enqueue unvisited internal links ──────────────────────
        if (depth < maxDepth) {
            for (const auto& link : links) {
                // Only follow links that stay on the same domain
                if (getDomain(link) == baseDomain &&
                    visited.find(link) == visited.end()) {

                    visited.insert(link);
                    frontier.push({link, depth + 1});
                    std::cout << "  [queued] " << link << std::endl;
                }
            }
        }

        // ── print all found links with their status ───────────────
        for (const auto& link : links) {
            int code = checkURL(link);
            std::string tag;
            if      (code == 200)              tag = "OK";
            else if (code >= 300 && code < 400) tag = "REDIRECT";
            else if (code == 404)              tag = "NOT FOUND";
            else if (code >= 500)              tag = "SERVER ERROR";
            else                               tag = "UNKNOWN";

            std::cout << "  [" << code << " " << tag << "] " 
                      << link << std::endl;
        }
    }

    // ── print adjacency list summary ──────────────────────────────────
    std::cout << "\n===== GRAPH (Adjacency List) =====\n";
    for (const auto& [page, outLinks] : graph) {
        std::cout << page << "\n";
        for (const auto& l : outLinks)
            std::cout << "  --> " << l << "\n";
    }

    return results;
}
