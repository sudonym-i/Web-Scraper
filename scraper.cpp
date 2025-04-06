#include <iostream>
#include <string>
#include <curl/curl.h>
#include <sstream>
#include "scraper.h"

DataList::~DataList(){
   //Erases entire linked list from heap
    node *next_node = nullptr;
    if(head != nullptr){
        while( head->next != nullptr ){
            next_node = head->next;
            delete head->data;
            delete head;
            head = next_node;
        }
        delete head->data;
        delete head;
        head = nullptr;
    }
}


DataList::DataList( int argc, char url[MAX_LINE][MAX_LINE], char start[MAX_LINE][MAX_LINE], char end[MAX_LINE][MAX_LINE]){
    head = new node; 
    head->data = scrape(argc, url[0], start[0], end[0]);
    node *current = head; 
    for(int i = 1; i < argc; i++){
        current->next = new node;
        current = current->next;
        current->data = scrape(argc, url[i], start[i], end[i]);
    }
}


//writes the entire linked list to ostream
void DataList::write(std::ostream &out){
    node *current = head;
    while(current != nullptr){
        out << *current->data;
        current = current->next;
    }
}


//parses into csv
std::string *DataList::breakpoints(std::string &html_content, const char start[MAX_LINE], const char end[MAX_LINE]){
    char *current = &html_content[0];
    std::string *inside = new std::string;

    while(*current != '\0'){

        //if we find the initiator tag...
        if( equal_str(current, start) ){

            //then we keep adding content until we reach the terminator tag
            while( !(equal_str(current, end)) ){
                (*inside).push_back(*current);
                current++;
            }
            (*inside).push_back(OUTPUT_SEPERATOR);

        }
    current++;
    }

    return inside;
}


//looks for keywords in html file
bool DataList::equal_str(char *&one, const char two[MAX_LINE]){
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
    

// Callback function to write the response data
size_t DataList::WriteCallback(void *contents, size_t size, size_t nmemb, std::string *output) {
    size_t total_size = size * nmemb;
    output->append((char*)contents, total_size);
    return total_size;
}


//Main function which scrapes the given websites using CURL
std::string *DataList::scrape(int argc, const char url_char[MAX_LINE], 
		char start[MAX_LINE], char end[MAX_LINE]) {    
    
    CURL *curl;
    CURLcode res;
    std::string *html_content = new std::string;

    //print message, displays tage and url
    std::cout << '\n' << "\033[35m" <<  "  url: " << "\033[00m" << url_char << std::endl;
    std::cout << "\033[35m" << "  start: " << "\033[00m" << start << std::endl;
    std::cout << "\033[35m" << "  end: " << "\033[00m" << end  << std::endl;

    curl_global_init(CURL_GLOBAL_ALL);

    if(curl) {

        // Set the URL to fetch
        std::string url = url_char;
        curl_easy_setopt(curl_easy_init(), CURLOPT_URL, url.c_str());

        // Set the callback function to write the response data into a string
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, html_content);

        // Perform the request
        res = curl_easy_perform(curl);

        // Parse the raw html file, returning only the stuff between the 'breakpoints'
	// NOTE: 'breakpoints' returns pointer to new string alloc'd on heap
        std::string *parsed = breakpoints(*html_content, start, end);
        
        //error checking
        if(res != CURLE_OK) {

            // in case of error, delete everything and return nullptr
            delete parsed;
            delete html_content;
            std::cerr << "'curl_easy_perform' failed: " << curl_easy_strerror(res) << '\n' << std::endl;
            return nullptr;

        } else {

            // Delete the raw html, return pointer to our parsed html data
            delete html_content;
            curl_easy_cleanup(curl);
            curl_global_cleanup();
            std::cout << "\033[32m" << "Success\n" << "\033[0m" << std::endl;
            return parsed;

        } // end consitional control

    }

    //Extra error checking for cases outside of the conditional
    std::cout << "failed. Unknown error.\n" << std::endl;
    return nullptr;
}

