#include <iostream>
#include "Crawl.h"
#include "Store.h"


using namespace std;

int main()
{
    Crawl crawler;
    Store storer;

    crawler.add_link("https://ytoo.org/");
    crawler.add_link("www.wiby.me");
    //crawler.add_link("https://pugixml.org/docs/manual.html");

    crawler.start_crawling();
    storer.commit_store();


    return 0;
}
