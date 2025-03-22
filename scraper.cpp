#include <iostream>
#include <string>
#include <curl/curl.h>

// Callback function to write the response data
size_t WriteCallback(void *contents, size_t size, size_t nmemb, std::string *output) {
    size_t total_size = size * nmemb;
    output->append((char*)contents, total_size);
    return total_size;
}

int main() {
    // Initialize curl globally
    CURL *curl;
    CURLcode res;
    std::string html_content;

    // Initialize curl
    curl_global_init(CURL_GLOBAL_ALL);
    curl = curl_easy_init();

    if(curl) {
        // Set the URL to fetch
        std::string url = "https://sudonym-i.github.io/Portfolio/";
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());

        // Set the callback function to write the response data into a string
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &html_content);

        // Perform the request
        res = curl_easy_perform(curl);

        // Check if the request was successful
        if(res != CURLE_OK) {
            std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
        } else {
            // Output the HTML content of the page
            std::cout << html_content << std::endl;
        }

        // Clean up curl
        curl_easy_cleanup(curl);
    }

    // Clean up curl globally
    curl_global_cleanup();

    return 0;
}
