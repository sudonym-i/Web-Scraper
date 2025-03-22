#include <string>
#ifndef OBJECTS_H
#define OBJECTS_H


struct node{
    std::string *data;
    node *next = nullptr;
};

#endif
