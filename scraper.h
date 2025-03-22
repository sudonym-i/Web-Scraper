#ifndef SCRAPER_H
#define SCRAPER_H
#include <string>


const int IND_SIZE = 32;

std::string *scrape(int argc, char* argv[], const char start[IND_SIZE], const char end[IND_SIZE]);

#endif
