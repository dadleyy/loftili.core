#include "communication/request.h"

namespace loftili {

size_t Request::receiver(char* ptr, size_t size, size_t nmemb, void* userdata) {
  size_t realsize = size * nmemb;
  Response* res = (Response*) userdata;
  res->content = realloc(res->content, res->length + realsize + 1);
  void* end = &((char*)res->content)[res->length];
  memcpy(end, ptr, realsize);
  res->length += realsize;
  ((char*)res->content)[res->length] = 0;
  return realsize;
}

Request::Request() : url(""), method(""), connection(0) { 
}

Request::Request(ahc_info info) {
  url = info.url;
  method = info.method;
  connection = info.connection;
}

Request::Request(std::string u, std::string m) : url(u), method(m), connection(0) {
}

Request::~Request() {
}

void Request::addHeader(std::string name, std::string value) {
  std::pair<std::string,std::string> new_header = std::make_pair(name, value);
  headers.push_back(new_header);
}

void Request::send(Response* res) {
  curl = curl_easy_init();
  curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
  curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &Request::receiver);
  curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void*) res);

  bool has_headers = headers.size() > 0;
  struct curl_slist* header_list = NULL;

  if(method == "POST") {
    curl_easy_setopt(curl, CURLOPT_POST, 1);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, "{}");
  }

  if(has_headers) {
    int header_count = headers.size();

    for(int i = 0; i < header_count; i++) {
      std::pair<std::string, std::string> header = (std::pair<std::string, std::string>) headers[i];
      std::stringstream header_stream;
      header_stream << header.first << ": " << header.second;
      const char* header_full = header_stream.str().c_str();
      header_list = curl_slist_append(header_list, header_full);
    }

    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, header_list);
  }

  long http_code = 0;
  curl_easy_perform(curl);
  curl_slist_free_all(header_list);
  curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &http_code);
  curl_easy_cleanup(curl);
  res->status = (int) http_code;
}

char* Request::query(std::string key) {
  char* value = NULL;

  value = (char*) MHD_lookup_connection_value(connection, MHD_GET_ARGUMENT_KIND, key.c_str());

  if(value == NULL)
    return NULL;

  return value;
}

void Request::send(Json* doc, Response* res) {
  curl = curl_easy_init();
  curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
  curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &Request::receiver);
  curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void*) res);

  if(method == "POST") {
    curl_easy_setopt(curl, CURLOPT_POST, 1);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, doc->buffer());
  }

  struct curl_slist* header_list = NULL;
  CURLcode http_code;

  header_list = curl_slist_append(header_list, "Content-Type: application/json");
  curl_easy_setopt(curl, CURLOPT_HTTPHEADER, header_list);

  curl_easy_perform(curl);
  curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &http_code);

  curl_slist_free_all(header_list);

  curl_easy_cleanup(curl);
  res->status = (int) http_code;
}

}
