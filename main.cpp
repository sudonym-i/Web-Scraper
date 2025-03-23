#include <string>
#include <iostream>
#include <fstream>
#include "scraper.h"
#include "object.h"

//max dimensions for the crawlchain file
const int MAX_LINE = 64;
const int MAX_LENGTH = 10;

void write(std::ostream &out, node *head);
void clear(node *&head);
void parse_crawlchain(int &argc, char argv[MAX_LENGTH][IND_SIZE], char start[MAX_LENGTH][IND_SIZE], char end[MAX_LENGTH][IND_SIZE]);

int main(){
    //TODO arg error handling
    int argc;
    char argv[MAX_LENGTH][IND_SIZE];
    char start[MAX_LENGTH][IND_SIZE];
    char end[MAX_LENGTH][IND_SIZE];
    parse_crawlchain(argc, argv, start, end);
  

    std::ofstream outfile("op.csv");
    node *head = new node;
    head->data = scrape(argc, argv[0], start[0], end[0]);
    node *current = head;
    for(int i = 1; i < argc; i++){
        current->next = new node;
        current = current->next;
        current->data = scrape(argc, argv[i], start[i], end[i]);
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

void parse_crawlchain(int &argc, char argv[MAX_LENGTH][IND_SIZE], char start[MAX_LENGTH][IND_SIZE], char end[MAX_LENGTH][IND_SIZE]){
    char garbage[MAX_LINE];
    std::ifstream crawlchain("../crawlchain.txt");

    if (!crawlchain.is_open()) {
        std::cerr << "Failed to open the file." << std::endl;
        return;
    }

    crawlchain.getline(garbage, MAX_LINE);
    int i = 0;
    while(!crawlchain.eof() && (crawlchain.peek() != ';') ){
        crawlchain.getline(start[i], MAX_LINE, ',');
        crawlchain.getline(end[i], MAX_LINE, ',');
        crawlchain.getline(argv[i], MAX_LINE);
        i++;
    }
    argc = i;
}