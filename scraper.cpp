#include <iostream>
#include <string>
#include <curl/curl.h>
#include <sstream>

//points that detemine what char to start and end reads at
//for example <a> , </a>
const char START = '>';
const char END = '<';

size_t WriteCallback(void *contents, size_t size, size_t nmemb, std::string *output);
void set_points(char &start, char &end);
std::string *breakpoints(std::string &html_content, const char start, const char end);


int main(int argc, char* argv[]) {
    
    // Initialize curl globally
    CURL *curl;
    CURLcode res;
    std::string *html_content = new std::string;

    // Initialize curl
    curl_global_init(CURL_GLOBAL_ALL);
    curl = curl_easy_init();

    if(curl) {
        // Set the URL to fetch
        std::string url = argv[1];
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());

        // Set the callback function to write the response data into a string
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, html_content);

        // Perform the request
        res = curl_easy_perform(curl);
        // Check if the request was successful
        if(res != CURLE_OK) {
            std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
        } else {
            // Output the HTML content of the page
            std::cout << *breakpoints(*html_content, START, END) << std::endl;
        }
        // Clean up curl
        curl_easy_cleanup(curl);
    }

    // Clean up curl globally
    curl_global_cleanup();
    return 0;
}

// Callback function to write the response data
size_t WriteCallback(void *contents, size_t size, size_t nmemb, std::string *output) {
    size_t total_size = size * nmemb;
    output->append((char*)contents, total_size);
    return total_size;
}


std::string *breakpoints(std::string &html_content, const char start, const char end){
    char *current = &html_content[0];
    std::string *inside = new std::string;
    while(*current != '\0'){

        if((*current == start) && (*(current+1) != end) ){
            while( (*current != end) && (*current != '\0')){
                (*inside).push_back(*current);
                current++;
            }
            (*inside).push_back(',');
        }
    current++;
    }
    return inside;
}
