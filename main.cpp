#include <string>
#include <iostream>
#include <fstream>
#include "scraper.h"
#include "object.h"

//max dimensions for the crawlchain file 
// (Capped capacity, as website content can get quite large)
const int MAX_LINE = 512; // Max length of each chain
const int MAX_LENGTH = 52; // max length of crawl-chain

//filepath to the crawlchain txt file. The file with all the commands.
const char CHAIN_PATH[MAX_LENGTH] = "../crawlchain.txt";

// Sets which characters will trigger the reader to start interpreting
// chain commands and when to end (Still must be on their OWN line)
const char OPEN_CHAIN = '{';
const char CLOSE_CHAIN = '}';
const char SEPERATOR = ','; // <-- the seperator char for items in 'crawlchain.txt'

void write(std::ostream &out, node *head);
void clear(node *&head);
void parse_crawlchain(int &argc, char url[MAX_LENGTH][IND_SIZE], char start[MAX_LENGTH][IND_SIZE], char end[MAX_LENGTH][IND_SIZE]);


int main(){
    //TODO arg error handling
    int argc;
    char url[MAX_LENGTH][IND_SIZE];
    char start[MAX_LENGTH][IND_SIZE];
    char end[MAX_LENGTH][IND_SIZE];
    parse_crawlchain(argc, url, start, end);

    std::ofstream outfile("op.csv");
    node *head = new node;
    head->data = scrape(argc, url[0], start[0], end[0]);
    node *current = head;
    for(int i = 1; i < argc; i++){
        current->next = new node;
        current = current->next;
        current->data = scrape(argc, url[i], start[i], end[i]);
    }

    write(outfile, head);
    clear(head);
    outfile.close();
    return 0;
}


//writes the entire linked list to ostream
void write(std::ostream &out, node *head){
    node *current = head;
    while(current != nullptr){
        out << *current->data << std::endl;
        current = current->next;
    }
    out << std::endl;
}

//Erases entire linked list from heap
void clear(node *&head){
    node *next_node = nullptr;
    if(head != nullptr){
        while( head->next != nullptr ){
            next_node = head->next;
            delete head->data;
            delete head;
            head = next_node;
        }
        delete head;
        head = nullptr;
    }
}

//uses syntax to parse info from crawlchain
void parse_crawlchain(int &argc, char url[MAX_LENGTH][IND_SIZE], char start[MAX_LENGTH][IND_SIZE], char end[MAX_LENGTH][IND_SIZE]){
    char garbage[MAX_LINE];
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
}