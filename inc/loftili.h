#ifndef _LOFTILI_H
#define _LOFTILI_H

#define LOFTILI_DEFAULT_PORT 8888
#define LOFTILI_POSTBUFFERSIZE 512
#define LOFTILI_EMPTY_RESPONSE "{}"
#define LOFTILI_EMPTY_RESPONSE_LEN strlen(LOFTILI_EMPTY_RESPONSE)
#define LOFTILI_API_HOME "http://api.loftili.com"
#define LOFTILI_REGISTRATION_URL LOFTILI_API_HOME "/registration"

#include <iostream>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <map>
#include <queue>
#include <curl/curl.h>
#include "config.h"
#include "headers.h"
#include "codes.h"
#include "rapidjson/rapidjson.h"
#include "rapidjson/document.h"
#include "rapidjson/prettywriter.h"
#include "rapidjson/stringbuffer.h"

typedef std::map<std::string, std::string> HttpHeaders;
typedef std::pair<std::string, std::string> HttpHeader;
typedef std::pair<std::string, int> ControllerMethod;
typedef rapidjson::UTF8<> utf8_str;
typedef rapidjson::GenericStringBuffer<utf8_str> JsonBuffer;

/* forward declare libmicrohttpd classes */
class MHD_Connection;

struct ahc_info {
  void* cls;
  MHD_Connection* connection;
  const char* url;
  const char* method;
  const char* version;
  const char* data;
  size_t* data_size;
  void** con_cls;
};

enum PLAYER_STATE {
  PLAYER_STATE_STOPPED,
  PLAYER_STATE_BUFFERING,
  PLAYER_STATE_PLAYING,
  PLAYER_STATE_ERRORED
};

enum PLAYER_ERROR {
  PLAYER_ERROR_NONE,
  PLAYER_ERROR_QUEUE_EMPTY,
  PLAYER_ERROR_QUEUE_LOAD,
  PLAYER_ERROR_UNKNOWN
};

struct Track {
  std::string track_url;
  int track_id;
};

#endif
