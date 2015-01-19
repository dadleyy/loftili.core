#include "audio/player.h"

namespace loftili {

AudioPlayer::AudioPlayer() : current_stream(0) {
  last_error = PLAYER_ERROR_NONE;
  log = new Logger("AudioPlayer");
}

AudioPlayer::~AudioPlayer() {
  if(current_stream)
    delete current_stream;

  current_stream = 0;

  delete log;
}

PLAYER_STATE AudioPlayer::stop() {
  log->info("stopping the audio player");

  if(current_stream)
    delete current_stream;

  current_stream = 0;

  return PLAYER_STATE_STOPPED;
}

PLAYER_STATE AudioPlayer::start() {
  last_error = PLAYER_ERROR_NONE;

  PLAYER_STATE current_state = state(); 

  if(current_state == PLAYER_STATE_PLAYING)
    return current_state;

  if(current_stream)
    stop();

  log->info("starting the audio player");

  return next();
}

PLAYER_STATE AudioPlayer::next() {
  if(current_stream)
    delete current_stream;

  QUEUE_STATUS queue_status = track_queue.fetch();

  if(queue_status != QUEUE_STATUS_FULL) {
    switch(queue_status) {
      case QUEUE_STATUS_ERRORED:
        log->info("ERROR - unable to load queue, queue was QUEUE_STATUS_ERRORED");
        last_error = PLAYER_ERROR_QUEUE_LOAD;
        break;
      case QUEUE_STATUS_EMPTY:
        log->info("ERROR - unable to load queue queue was QUEUE_STATUS_EMPTY");
        last_error = PLAYER_ERROR_QUEUE_EMPTY;
        break;
      default:
        log->info("queue was unknown");
        last_error = PLAYER_ERROR_UNKNOWN;
        break;
    }

    return PLAYER_STATE_ERRORED;
  }

  current_track_info = track_queue.pop();

  log->info("queue ready, starting: ", current_track_info.track_url);

  current_stream = new AudioStream(current_track_info.track_url);

  current_stream->start();

  return PLAYER_STATE_BUFFERING;
}

PLAYER_STATE AudioPlayer::state() {
  PLAYER_STATE calculated_state = PLAYER_STATE_STOPPED;

  if(last_error != PLAYER_ERROR_NONE) {
    log->info("the last error was not NONE, returning PLAYER_STATE_ERRORED");

    if(last_error == PLAYER_ERROR_QUEUE_EMPTY) {
      log->info("the last error PLAYER_ERROR_QUEUE_EMPTY, returning PLAYER_STATE_EMPTY_QUEUE");
      return PLAYER_STATE_EMPTY_QUEUE;
    }

    return PLAYER_STATE_ERRORED;
  }

  if(!current_stream)
    return calculated_state;

  STREAM_STATE stream_state = current_stream->state();

  switch(stream_state) {
    case STREAM_STATE_BUFFERING:
      calculated_state = PLAYER_STATE_BUFFERING;
      break;
    case STREAM_STATE_PLAYING:
      calculated_state = PLAYER_STATE_PLAYING;
      break;
    case STREAM_STATE_ERRORED:
      calculated_state = PLAYER_STATE_STREAM_ERROR;
      break;
    default:
      calculated_state = PLAYER_STATE_STOPPED;
      break;
  }

  return calculated_state;
}

}
