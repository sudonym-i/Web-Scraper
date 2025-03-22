#include <string>
#include <iostream>
#include <fstream>
#include "scraper.h"
#include "object.h"

const char START[IND_SIZE] = "<h1>";
const char END[IND_SIZE] = "</h1>";

void write(std::ostream &out, node *head);
void clear(node *&head);

int main(int argc, char* argv[]){
    //TODO arg error handling

    std::ofstream outfile("op.csv");
    node *head = new node;
    head->data = scrape(argc, argv[1], START, END);

    node *current = head;
    for(int i = 2; i < argc; i++){
        current->next = new node;
        current = current->next;
        current->data = scrape(argc, argv[i], START, END);
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
        out << *current->data << '\n';
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