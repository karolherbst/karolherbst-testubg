#include <iostream>
 
#include <unistd.h>
 
#include <curl/curl.h>
 
using namespace std;
 
class Downloader
{
public:
    template <class C>
    void downloadFile(const char* const url, size_t size, C& ref, void (C::*callback)(void *, size_t, size_t
));
protected:
    void handleDownload(const char* const url, size_t size, CurlWrapperCallback * cwc);
};
 
struct CurlCallback
{
    void callback(void *, size_t, size_t);
    typedef void (CurlCallback::*funcPtr)(void *, size_t, size_t);
};
 
struct CurlWrapperCallback
{
    CurlCallback * ref;
    CurlCallback::funcPtr callback;
};
 
static size_t curlWrapper(void *buffer, size_t sz, size_t n, CurlWrapperCallback *f)
{
    std::cout << "curlWRapper try to call callback" << std::endl;
    (f->ref->*f->callback)(buffer, sz, n);
    delete f;
    return n;
}
 
template <class C>
void
Downloader::downloadFile(const char* const url, size_t size, C& ref, void (C::*callback)(void *, size_t, size_t
))
{
     std::cout << "downloadFile" << std::endl;
     CurlWrapperCallback * cwc = new CurlWrapperCallback();
     cwc->ref = reinterpret_cast<CurlCallback *>(&ref);
     cwc->callback = reinterpret_cast<CurlCallback::funcPtr>(callback);
     handleDownload(url, size, cwc);
}

void
Downloader::handleDownload(const char* const url, size_t size, CurlWrapperCallback * cwc)
{
     CURL *curl = curl_easy_init();
     curl_easy_setopt(curl, CURLOPT_URL, url);
     curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, curlWrapper);
     curl_easy_setopt(curl, CURLOPT_WRITEDATA, cwc);
     
     CURLcode curlCode = curl_easy_perform(curl);
     if(curlCode != CURLE_OK)
     {
         std::cout << "error occured:  " << curl_easy_strerror(curlCode) << std::endl;
     }
     else
     {
         std::cout << "no error while downloading" << std::endl;
     }
     curl_easy_cleanup(curl);
}
 
class A
{
public:
    void handleDownload(void *, size_t, size_t);
};
 
void
A::handleDownload(void * buffer, size_t, size_t)
{
    std::cout << "got some data" << std::endl;
    std::cout << static_cast<const char*>(buffer) << std::endl;
}
 
int main()
{
    A a;
    Downloader d;
    const char url[] = "http://google.de";
    d.downloadFile(url, sizeof(url), a, &A::handleDownload);
    sleep(2);
    return 0;
}
