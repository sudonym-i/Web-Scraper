#include <iostream>
#include <fstream>
#include <curl/curl.h>
#include "libxml/HTMLparser.h"
#include "libxml/xpath.h"
#include <vector>

std::string get_request(std::string url) {
    // initialize curl locally
    CURL *curl = curl_easy_init();
    std::string result;

    if (curl) {
        // perform the GET request
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, [](void *contents, size_t size, size_t nmemb, std::string *response) {
            ((std::string*) response)->append((char*) contents, size * nmemb);
            return size * nmemb; 
        });
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &result);
        curl_easy_perform(curl);

        // free up the local curl resources
        curl_easy_cleanup(curl);
    }

    return result;
}

// to store the scraped data of interest
// for each product
struct Product {
    std::string url;
    std::string image;
    std::string name;
    std::string price;
};

int main() {
    // initialize curl globally
    curl_global_init(CURL_GLOBAL_ALL);
    // retrieve the HTML content of the target page
    std::string html_document = get_request("https://www.scrapingcourse.com/ecommerce/");
   
    // parse the HTML document returned by the server
    htmlDocPtr doc = htmlReadMemory(html_document.c_str(), html_document.length(), nullptr, nullptr, HTML_PARSE_NOERROR);

    // initialize the XPath context for libxml2
    // to the entire document
    xmlXPathContextPtr context = xmlXPathNewContext(doc);
    // get the product HTML elements
    xmlXPathObjectPtr product_html_elements = xmlXPathEvalExpression((xmlChar *) "//li[contains(@class, 'product')]", context);

    // to store the scraped products
    std::vector<Product> products;

    // iterate the list of product HTML elements
    for (int i = 0; i < product_html_elements->nodesetval->nodeNr; ++i) {
        // get the current element of the loop
        xmlNodePtr product_html_element = product_html_elements->nodesetval->nodeTab[i];

        // set the context to restrict XPath selectors
        // to the children of the current element
        xmlXPathSetContextNode(product_html_element, context);
        xmlNodePtr url_html_element = xmlXPathEvalExpression((xmlChar *) ".//a", context)->nodesetval->nodeTab[0];
        std::string url = std::string(reinterpret_cast<char *>(xmlGetProp(url_html_element, (xmlChar *) "href")));
        xmlNodePtr image_html_element = xmlXPathEvalExpression((xmlChar *) ".//a/img", context)->nodesetval->nodeTab[0];
        std::string image = std::string(reinterpret_cast<char *>(xmlGetProp(image_html_element, (xmlChar *) "src")));
        xmlNodePtr name_html_element = xmlXPathEvalExpression((xmlChar *) ".//a/h2", context)->nodesetval->nodeTab[0];
        std::string name = std::string(reinterpret_cast<char *>(xmlNodeGetContent(name_html_element)));
        xmlNodePtr price_html_element = xmlXPathEvalExpression((xmlChar *) ".//a/span", context)->nodesetval->nodeTab[0];
        std::string price = std::string(reinterpret_cast<char *>(xmlNodeGetContent(price_html_element)));

        Product product = {url, image, name, price};
        products.push_back(product);
    }

    // free up libxml2 resources
    xmlXPathFreeContext(context);
    xmlFreeDoc(doc);

    // create the CSV file of output
    std::ofstream csv_file("products.csv");

    // populate it with the header
    csv_file << "url,image,name,price" << std::endl;

    // populate the CSV output file
    for (int i = 0; i < products.size(); ++i) {
        // transform a Product instance to a
        // CSV string record
        Product p = products.at(i);
        std::string csv_record = p.url + "," + p.image + "," + p.name + "," + p.price;
        csv_file << csv_record << std::endl;
    }

    // free up the resources for the CSV file
    csv_file.close();

    // free up the global curl resources
    curl_global_cleanup();

    return 0;
}
