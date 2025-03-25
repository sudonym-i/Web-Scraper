#include <iostream>
#include <string>
#include <curl/curl.h>
#include <sstream>
#include "scraper.h"

//points that detemine what char to start and end reads at
//for example <a> , </a>

bool equal_str(char *&one, const char two[IND_SIZE]);
size_t WriteCallback(void *contents, size_t size, size_t nmemb, std::string *output);
std::string *breakpoints(std::string &html_content, const char start[IND_SIZE], const char end[IND_SIZE]);


std::string *scrape(int argc, const char url_char[IND_SIZE], char start[IND_SIZE], char end[IND_SIZE]) {    
    // Initialize curl globally
    CURL *curl;
    CURLcode res;
    std::string *html_content = new std::string;

    std::cout << '\n' << "\033[35m" <<  "  url: " << "\033[00m" << url_char << std::endl;
    std::cout << "\033[35m" << "  start: " << "\033[00m" << start << std::endl;
    std::cout << "\033[35m" << "  end: " << "\033[00m" << end  << std::endl;

    // Initialize curl
    curl_global_init(CURL_GLOBAL_ALL);
    curl = curl_easy_init();

    if(curl) {
        // Set the URL to fetch
        std::string url = url_char;
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());

        // Set the callback function to write the response data into a string
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, html_content);

        // Perform the request
        res = curl_easy_perform(curl);
        // Check if the request was successful
        std::string *parsed = breakpoints(*html_content, start, end);
        
        if(res != CURLE_OK) {
            delete parsed;
            delete html_content;
            std::cerr << "'curl_easy_perform' failed: " << curl_easy_strerror(res) << '\n' << std::endl;
            return nullptr;
        } else {
            // Output the HTML content of the page
            delete html_content;
            curl_easy_cleanup(curl);
            curl_global_cleanup();
            std::cout << "\033[32m" << "Success\n" << "\033[0m" << std::endl;
            return parsed;
        }
    }
    std::cout << "failed. Unknown error.\n" << std::endl;
    return nullptr;
}

// Callback function to write the response data
size_t WriteCallback(void *contents, size_t size, size_t nmemb, std::string *output) {
    size_t total_size = size * nmemb;
    output->append((char*)contents, total_size);
    return total_size;
}

//parses into csv
std::string *breakpoints(std::string &html_content, const char start[IND_SIZE], const char end[IND_SIZE]){
    char *current = &html_content[0];
    std::string *inside = new std::string;
    while(*current != '\0'){

        if( equal_str(current, start) ){
            while( !(equal_str(current, end)) ){
                (*inside).push_back(*current);
                current++;
            }
            (*inside).push_back(',');
        }
    current++;
    }
    return inside;
}

//looks for keywords in html file
bool equal_str(char *&one, const char two[IND_SIZE]){
    int i = 0;
    while(*one != '\0' && two[i] != '\0'){
        if(*one != two[i]){
            return false;
        }
        i++;
        one++;
    }
    return true;
}