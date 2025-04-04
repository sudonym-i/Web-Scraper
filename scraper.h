#ifndef SCRAPER_H
#define SCRAPER_H
#include <string>

const int MAX_LINE = 512; // Max length of each chain
const int MAX_LENGTH = 52; // max length of crawl-chain

const char OUTPUT_SEPERATOR = '\n';

class DataList{

public:

    ~DataList();

    DataList( int argc, char url[MAX_LINE][MAX_LINE], char start[MAX_LINE][MAX_LINE], char end[MAX_LINE][MAX_LINE] );

    void write(std::ostream &out);
 
private:

	struct node{
		std::string *data;
		node *next = nullptr;
	};

    node * head = nullptr;

    bool equal_str(char *&one, const char two[MAX_LINE]);

    static size_t WriteCallback(void *contents, size_t size, size_t nmemb, std::string *output);

    std::string *scrape(int argc, const char url_char[MAX_LINE], 
			char start[MAX_LINE], char end[MAX_LINE]);    

    std::string *breakpoints(std::string &html_content, const char start[MAX_LINE], const char end[MAX_LINE]);
        
};

#endif
