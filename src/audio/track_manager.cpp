#include "audio/track_manager.h"

namespace loftili {

TrackManager::TrackManager() : device_options(), device_credentials() {
  log = new Logger("TrackManager");
}

TrackManager::~TrackManager() {
  delete log;
}

std::string TrackManager::endpoint() {
  std::stringstream ss;
  int device_id = device_credentials.deviceId();
  ss << (device_options.api_host != "" ? device_options.api_host : LOFTILI_API_HOME);
  ss << "/queues/" << device_id << "/pop";
  return ss.str();
}

void TrackManager::initialize(Credentials init_creds, Options init_opts) {
  device_credentials = init_creds;
  device_options = init_opts;
}

std::string TrackManager::pop() {
  string first_track = track_urls.front();
  track_urls.pop();
  return first_track;
}

QUEUE_STATUS TrackManager::status() {
  return QUEUE_STATUS_EMPTY;
}

QUEUE_STATUS TrackManager::fetch() {
  log->info("fetching track queue");
  Request request;
  Response response("", 0);

  request.url = endpoint();
  request.method = "POST";
  request.addHeader("x-loftili-device-auth", device_credentials.token());
  request.send(&response);

  if(response.status == 204) {
    log->info("track queue was empty!");
    return QUEUE_STATUS_EMPTY;
  }

  if(response.status != 200) {
    log->info("track queue fetch returned non 200 status code, failing");
    log->info((char*) response.content);
    return QUEUE_STATUS_ERRORED;
  }

  rapidjson::Document popped_track_info;
  popped_track_info.Parse<0>((char*) response.content);

  bool can_use = popped_track_info.IsObject();

  if(!can_use)
    return QUEUE_STATUS_ERRORED;

  can_use = popped_track_info["streaming_url"].IsString();

  if(!can_use)
    return QUEUE_STATUS_ERRORED;

  string track_url = (string) popped_track_info["streaming_url"].GetString();
  track_urls.push(track_url);

  return QUEUE_STATUS_FULL;
}

}
