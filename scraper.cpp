std::string get_request(std::string url) {
    // initialize curl locally
    CURL *curl = curl_easy_init();
    std::string result;

    if (curl) {
        // perform the GET request
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, [](void *contents, size_t size, size_t nmemb, std::string *response) {
            ((std::string*) response)->append((char*) contents, size * nmemb);
            return size * nmemb; });
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &result);
        curl_easy_perform(curl);

        // free up the local curl resources
        curl_easy_cleanup(curl);
    }

    return result;
}
