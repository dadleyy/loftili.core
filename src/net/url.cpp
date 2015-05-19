#include "net/url.h"

namespace loftili {

namespace net {

Url::Url(const char* url_string) {
  const char *protocol_break = strstr(url_string, "://");

  if(protocol_break == nullptr) {
    protocol_break = url_string;
  } else {
    int protocol_size = protocol_break - url_string;
    m_protocol = std::string(url_string, protocol_size);
    protocol_break += 3;
  }

  const char *host_break = strstr(protocol_break, "/");
  int host_size;

  if(host_break == nullptr) {
    host_size = strlen(protocol_break);
    m_host = std::string(protocol_break, host_size);
    return;
  } else  {
    host_size = host_break - protocol_break;
  }

  m_host = std::string(protocol_break, host_size);
  const char *port_break;
  char *end;
  if((port_break = strchr(m_host.c_str(), ':')) != nullptr) {
    m_port = std::strtol(m_host.substr((port_break + 1) - m_host.c_str(), std::string::npos).c_str(), &end, 10);
    m_host = m_host.substr(0, port_break - m_host.c_str());
  } else
    m_port = 80;
  const char *path_break = strchr(host_break, '\0');
  if(path_break == nullptr) return;
  int path_size = path_break - host_break;
  m_path = std::string(host_break, path_size);
}

}

}