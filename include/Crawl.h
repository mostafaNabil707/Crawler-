#ifndef CRAWL_H
#define CRAWL_H

#include <iostream>
#include "curl\curl.h"
#include <string>
#include <queue>

struct memory {
          char *response;
          size_t size;
        };

using namespace std;

class Crawl
{
    public:
        Crawl();
        virtual ~Crawl();
        void start_crawling();
        void add_link(const char *);
    protected:

    private:

        memory chunk = {0};
        CURL *curl;

        std::queue<string> links;

};

#endif // CRAWL_H
