#include <string>
#include <iostream>
#include <fstream>
#include "scraper.h"

//filepath to the crawlchain txt file. The file with all the commands.
const char CHAIN_PATH[MAX_LENGTH] = "../crawlchain.txt";
const char OUTPUT_NAME[MAX_LENGTH] = "course_names.csv";

// Sets which characters will trigger the reader to start interpreting
// chain commands and when to end (Still must be on their OWN line)
const char OPEN_CHAIN = '{';
const char CLOSE_CHAIN = '}';
const char SEPERATOR = ','; // <-- the seperator char for items in 'crawlchain.txt'

void parse_crawlchain(int &argc, char url[MAX_LENGTH][MAX_LINE], char start[MAX_LENGTH][MAX_LINE], char end[MAX_LENGTH][MAX_LINE]);

int main(){    
//TODO arg error handling
    int argc;

    char url[MAX_LENGTH][MAX_LINE];
    char start[MAX_LENGTH][MAX_LINE];
    char end[MAX_LENGTH][MAX_LINE];

    parse_crawlchain(argc, url, start, end);
    //extract neccesary information from the crawlchain.txt file
    
    DataList crawledData(argc, url, start, end);

    std::ofstream outfile(OUTPUT_NAME);

    crawledData.write(outfile);
    
    outfile.close();

    return 0;
} // END main




//uses syntax to parse info from crawlchain
void parse_crawlchain(int &argc, char url[MAX_LENGTH][MAX_LINE], char start[MAX_LENGTH][MAX_LINE], char end[MAX_LENGTH][MAX_LINE]){
    char garbage[MAX_LINE] = {};
    std::ifstream crawlchain(CHAIN_PATH);

    if (!crawlchain.is_open()) {
        std::cerr << "Failed to open the file." << std::endl;
        return;
    }
    while(garbage[0] != OPEN_CHAIN){
        crawlchain.getline(garbage, MAX_LINE);
    } // find my initializer line
    int i = 0;
    while(!crawlchain.eof() && (crawlchain.peek() != CLOSE_CHAIN) ){
        crawlchain.getline(start[i], MAX_LINE, SEPERATOR);
        crawlchain.getline(end[i], MAX_LINE, SEPERATOR);
        crawlchain.getline(url[i], MAX_LINE);
        i++;
    }
    argc = i;
    return;
}
