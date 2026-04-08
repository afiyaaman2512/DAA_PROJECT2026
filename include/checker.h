#ifndef CHECKER_H
#define CHECKER_H

#include <string>

// Takes a URL, sends HTTP HEAD request, returns status code
// Returns 0 if the request completely failed (no internet, invalid URL)
int checkURL(const std::string& url);

#endif
