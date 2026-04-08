#include "parser.h"
#include <gumbo.h>
#include <algorithm>

// Converts relative URLs to absolute
std::string resolveURL(const std::string& href, const std::string& baseURL) {
    if (href.empty()) return "";
    
    // Already absolute
    if (href.substr(0, 4) == "http") return href;
    
    // Skip non-http links (mailto:, javascript:, #anchors)
    if (href[0] == '#') return "";
    if (href.find("mailto:") == 0) return "";
    if (href.find("javascript:") == 0) return "";

    // Get the domain from baseURL (e.g. "https://example.com")
    std::string domain = baseURL;
    size_t pos = domain.find("://");
    if (pos != std::string::npos) {
        pos = domain.find("/", pos + 3);
        if (pos != std::string::npos)
            domain = domain.substr(0, pos);
    }

    // Handle absolute path (starts with /)
    if (href[0] == '/') return domain + href;

    // Handle relative path
    return domain + "/" + href;
}

// Recursively walks the HTML tree looking for <a href="..."> tags
static void extractLinksFromNode(GumboNode* node, 
                                  const std::string& baseURL,
                                  std::vector<std::string>& links) {
    if (node->type != GUMBO_NODE_ELEMENT) return;

    GumboAttribute* href = nullptr;

    // If this is an <a> tag, grab its href
    if (node->v.element.tag == GUMBO_TAG_A) {
        href = gumbo_get_attribute(&node->v.element.attributes, "href");
        if (href) {
            std::string resolved = resolveURL(href->value, baseURL);
            if (!resolved.empty()) {
                links.push_back(resolved);
            }
        }
    }

    // Recurse into children
    GumboVector* children = &node->v.element.children;
    for (unsigned int i = 0; i < children->length; ++i) {
        extractLinksFromNode(
            static_cast<GumboNode*>(children->data[i]), 
            baseURL, 
            links
        );
    }
}

std::vector<std::string> extractLinks(const std::string& html, 
                                       const std::string& baseURL) {
    std::vector<std::string> links;
    
    GumboOutput* output = gumbo_parse(html.c_str());
    extractLinksFromNode(output->root, baseURL, links);
    gumbo_destroy_output(&kGumboDefaultOptions, output);

    return links;
}
