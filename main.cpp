#include <string>
#include <iostream>
#include <fstream>
#include "scraper.h"

//filepath to the crawlchain txt file. The file with all the commands.
const char CHAIN_PATH[MAX_LENGTH] = "../chain.txt";

// CHAIN commands (for READING crawlchain.txt)
const char OPEN_CHAIN = '{';
const char CLOSE_CHAIN = '}';
const char SEPERATOR = ','; // <-- the seperator char for items in 'crawlchain.txt'

void parse_crawlchain(int &argc, char url[MAX_LENGTH][MAX_LINE], 
		char start[MAX_LENGTH][MAX_LINE], char end[MAX_LENGTH][MAX_LINE], 
		char output_name[MAX_LENGTH]);

int main(){    
//TODO arg error handling
    int argc;
    char output_name[MAX_LENGTH];
    char url[MAX_LENGTH][MAX_LINE];
    char start[MAX_LENGTH][MAX_LINE];
    char end[MAX_LENGTH][MAX_LINE];

    parse_crawlchain(argc, url, start, end, output_name);
    //extract neccesary information from the crawlchain.txt file
    
    DataList crawledData(argc, url, start, end);

    std::ofstream outfile(output_name);

    crawledData.write(outfile);
    
    outfile.close();

    return 0;
} // END main


//recursive function to handle potential blank lines inside the brackets for the crawlchain - more redundency
bool feedline(std::istream &crawlchain, char start[MAX_LENGTH][MAX_LINE], 
		char end[MAX_LENGTH][MAX_LINE], char url[MAX_LENGTH][MAX_LINE], int i){
    
    if( crawlchain.eof() || (crawlchain.peek() == CLOSE_CHAIN) ){
        return false;
    }else{
        crawlchain.getline(start[i], MAX_LINE, SEPERATOR);
        crawlchain.getline(end[i], MAX_LINE, SEPERATOR);
        crawlchain.getline(url[i], MAX_LINE);    
        return true;
    
    }
}
 

//uses syntax to parse info from chain.txt
void parse_crawlchain(int &argc, char url[MAX_LENGTH][MAX_LINE], 
		char start[MAX_LENGTH][MAX_LINE], char end[MAX_LENGTH][MAX_LINE], 
		char output_name[MAX_LENGTH]){

    std::ifstream crawlchain(CHAIN_PATH);

    if (!crawlchain.is_open()) {
        std::cerr << "Failed to open the file." << std::endl;
        return;
    }

    char garbage[MAX_LINE] = {};
    char garby;

  // index 0 for redundency, for the case where a space is included after char accidentally or something
    while(garbage[0] != OPEN_CHAIN){
        crawlchain.getline(garbage, MAX_LINE);
    } // find my initializer line
    
    int i = 0;

    while(feedline(crawlchain, start, end, url, i)){ 
	    i++; 
    }
    crawlchain.getline(garbage, MAX_LINE, ':');
    crawlchain.get(garby);
    crawlchain.getline(output_name, MAX_LENGTH, ';'); 
    argc = i;
    crawlchain.close();
    return;
}

