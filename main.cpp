#include "scraper.h"
#include "object.h"
#include <string>


const char START[IND_SIZE] = "<h1>";
const char END[IND_SIZE] = "</h1>";

int main(int argc, char* argv[]){

    std::string *one = scrape(argc, argv, START, END);
    return 0;
}

