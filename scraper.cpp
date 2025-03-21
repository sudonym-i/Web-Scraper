

#include <iostream>
#include <curl/curl.h>
#include "libxml/HTMLparser.h"
#include "libxml/xpath.h"

// std::string get_request(std::string url) { ... }

int main() {
    // initialize curl globally
    curl_global_init(CURL_GLOBAL_ALL);

    // download the target HTML document 
    // and print it
    std::string html_document = get_request("https://www.scrapingcourse.com/ecommerce/");
    std::cout << html_document;

    // scraping logic...

    // free up the global curl resources
    curl_global_cleanup();

    return 0;
}


std::string get_request(std::string url) {
    // initialize curl locally
    CURL *curl = curl_easy_init();
    std::string result;

    if (curl) {
        // perform the GET request
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, [](void *contents, size_t size, size_t nmemb, std::string *response) {
            ((std::string*) response)->append((char*) contents, size * nmemb);
            return size * nmemb; });
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &result);
        curl_easy_perform(curl);

        // free up the local curl resources
        curl_easy_cleanup(curl);
    }

    return result;
}

