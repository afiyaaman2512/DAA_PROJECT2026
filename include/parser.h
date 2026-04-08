#ifndef PARSER_H
#define PARSER_H

#include <string>
#include <vector>

// Takes raw HTML and the base URL of the page
// Returns a list of all absolute URLs found on the page
std::vector<std::string> extractLinks(const std::string& html, 
                                       const std::string& baseURL);

// Helper: converts a relative URL to absolute
// e.g. "/about" + "https://example.com" → "https://example.com/about"
std::string resolveURL(const std::string& href, const std::string& baseURL);


#endif
