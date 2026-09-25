#include "Crawl.h"
#include "curl/curl.h"
#include <stdlib.h>
#include <string.h> /* for memcpy */
#include <queue>
#include "html_parser.hpp"

#define CRAWL_LIM 10000

using namespace std;

//curl call back
static size_t cb(char *data, size_t size, size_t nmemb, void *clientp)
{
  size_t realsize = nmemb;
  struct memory *mem = (struct memory *)clientp;

  char *ptr = (char *)realloc(mem->response, mem->size + realsize + 1);
  if(!ptr)
    return 0;  /* out of memory */

  mem->response = ptr;
  memcpy(&(mem->response[mem->size]), data, realsize);
  mem->size += realsize;
  mem->response[mem->size] = 0;

  return realsize;
}

Crawl::Crawl()
{
    //ctor
    this->curl = curl_easy_init();

    curl_easy_setopt(this->curl, CURLOPT_WRITEFUNCTION, cb);
    curl_easy_setopt(this->curl, CURLOPT_WRITEDATA, (void *)&chunk);
}

Crawl::~Crawl()
{
    curl_easy_cleanup(this->curl);
    //free(this->chunk.response);
    //dtor
}

void Crawl::add_link(const  char *link)
{
    this->links.push(link);
    cout << "pushed: " << link << endl;
}


void Crawl::start_crawling()
{
            string current_link;
            size_t n_crawled = 0;

            cout << "Crawling..." << endl;

            while(n_crawled < CRAWL_LIM && this->links.size()){


                current_link = this->links.front();
                this->links.pop();

                this->chunk.response = (char *)malloc(1);
                this->chunk.size = 0;

                cout << "@" << current_link << endl;
                curl_easy_setopt(this->curl, CURLOPT_URL, current_link.c_str());

                if(curl_easy_perform(this->curl) == CURLE_OK){

                    HtmlParser parser;
                    shared_ptr<HtmlDocument> doc = parser.Parse(this->chunk.response, this->chunk.size);
                    std::vector<shared_ptr<HtmlElement>> x = doc->GetElementByTagName("a");

                    cout << "crawled ^_^ "<< endl;

                    for(int i = 0; i < x.size(); i++){
                        std::cout << x[i]->GetAttribute("href").c_str() << std::endl;
                        this->add_link(x[i]->GetAttribute("href").c_str());
                       }


                }else{

                    cout << "crawl failed :(" << endl;

                }
                cout << "*===================*" <<  endl;

                if(this->chunk.size){
                    free(this->chunk.response);
                    this->chunk.size = 0;
                }

                n_crawled++;
            }
}
