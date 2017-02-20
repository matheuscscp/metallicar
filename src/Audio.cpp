/*
 * Audio.cpp
 *
 *  Created on: Aug 25, 2014
 *      Author: Pimenta
 */

// this
#include "metallicar_asset.hpp"

// standard
#include <list>

// lib
#include "stb_vorbis.h"
#include "al.h"

// local
#include "Path.hpp"
#include "Log.hpp"
#include "String.hpp"

#define NUM_SAMPLES 0x5000

using namespace std;

namespace metallicar {

static list<pair<shared_ptr<Audio>, Thread>> playbacks;
static Lock playbacksLock;
static float volumeSFX = 1.0f, volumeBGM = 1.0f;

static shared_ptr<Audio> play(
  const string& path, int loop, float volume, float* globalVolume
);

struct Audio::Playback {
  shared_ptr<Audio> audio;
  stb_vorbis* handle;
  ALuint source, buffers[2];
  ALenum format;
  ALsizei block_size;
  int loop;
  ALfloat volume;
  ALfloat* globalVolume;
  bool paused, stopped;
  
  Playback(
    const string& path, int loop, float volume, float* globalVolume, int* error
  ) :
  audio(new Audio(this)),
  handle(stb_vorbis_open_filename(
    (char*)Path::get(path).c_str(), error, nullptr)
  ),
  loop(loop),
  volume(volume),
  globalVolume(globalVolume),
  paused(false),
  stopped(false)
  {
    if (!handle) {
      return;
    }
    
    // generating OpenAL source and buffers
    alGenSources(1, &source);
    alGenBuffers(2, buffers);
    
    // setting format and block size
    if (handle->channels == 2) {
      format = AL_FORMAT_STEREO16;
      block_size = 4;
    }
    else {
      format = AL_FORMAT_MONO16;
      block_size = 2;
    }
    
    // loading initial chunks
    ALsizei buffersToQueue = 2;
    for (ALint i = 0; i < 2; i++) {
      ALshort* buf = new short[NUM_SAMPLES*block_size];
      ALsizei samples_read = stb_vorbis_get_samples_short_interleaved(
        handle, handle->channels, buf, NUM_SAMPLES*handle->channels
      );
      if (samples_read == 0) {
        if (i == 0) {
          stb_vorbis_close(handle);
          handle = nullptr;
          return;
        }
        else {
          buffersToQueue = 1;
          break;
        }
      }
      alBufferData(
        buffers[i], format, buf, samples_read*block_size, handle->sample_rate
      );
      delete[] buf;
    }
    
    // play
    alSourcef(source, AL_GAIN, volume*(*globalVolume));
    alSourceQueueBuffers(source, buffersToQueue, buffers);
    alSourcePlay(source);
  }
  
  ~Playback() {
    audio->playbackLock.mutex([this]() { audio->playback = nullptr; });
    
    alDeleteSources(1, &source);
    alDeleteBuffers(2, buffers);
    
    stb_vorbis_close(handle);
  }
  
  void run() {// TODO implement looping playback
    // loop loading more chunks until source is stopped
    ALint state;
    for (
      alGetSourcei(source, AL_SOURCE_STATE, &state);
      state != AL_STOPPED;
      alGetSourcei(source, AL_SOURCE_STATE, &state)
    ) {
      alSourcef(source, AL_GAIN, volume*(*globalVolume));
      
      ALint processed;
      alGetSourcei(source, AL_BUFFERS_PROCESSED, &processed);
      
      if (processed) {
        ALuint tmpBuffers[processed];
        alSourceUnqueueBuffers(source, processed, tmpBuffers);
        for (ALint i = 0; i < processed; i++) {
          ALshort* buf = new short[NUM_SAMPLES*block_size];
          ALsizei samples_read = stb_vorbis_get_samples_short_interleaved(
            handle, handle->channels, buf, NUM_SAMPLES*handle->channels
          );
          alBufferData(
            tmpBuffers[i],format, buf, samples_read*block_size,
            handle->sample_rate
          );
          delete[] buf;
        }
        alSourceQueueBuffers(source, processed, tmpBuffers);
      }
      
      Thread::sleep(1);
    }
    stopped = true;
  }
  
  void pause() {
    if (!paused && !stopped) {
      paused = true;
      alSourcePause(source);
    }
  }
  
  void resume() {
    if (paused && !stopped) {
      paused = false;
      alSourcePlay(source);
    }
  }
  
  void stop() {
    stopped = true;
    alSourceStop(source);
  }
  
  void setVolume(float vol) {
    volume = vol > 1.0f ? 1.0f : (vol < 0.0f ? 0.0f : vol);
  }
  
  float getVolume() {
    return volume;
  }
};

Audio::Audio(Playback* playback) : playback(playback) {
  playbackLock.init();
}

Audio::~Audio() {
  playbackLock.close();
}

void Audio::pause() {
  playbackLock.mutex([this]() { if (playback) playback->pause(); });
}

void Audio::resume() {
  playbackLock.mutex([this]() { if (playback) playback->resume(); });
}

void Audio::stop() {
  playbackLock.mutex([this]() { if (playback) playback->stop(); });
}

float Audio::getVolume() {
  float vol = 0.0f;
  playbackLock.mutexlock();
  if (playback) {
    vol = playback->getVolume();
  }
  playbackLock.unlock();
  return vol;
}

void Audio::setVolume(float vol) {
  playbackLock.mutex([this, vol]() { if (playback) playback->setVolume(vol); });
}

bool Audio::stopped() const {
  return (playback == nullptr);
}

void Audio::init() {
  playbacksLock.init();
}

void Audio::close() {
  playbacksLock.close();
}

shared_ptr<Audio> Audio::playSFX(const string& path, int loop, float volume) {
  return play(path, loop, volume, &volumeSFX);
}

shared_ptr<Audio> Audio::playBGM(const string& path, int loop, float volume) {
  return play(path, loop, volume, &volumeBGM);
}

void Audio::setSFXVolume(float volume) {
  volumeSFX = volume;
}

void Audio::setBGMVolume(float volume) {
  volumeBGM = volume;
}

void Audio::clean() {
  playbacksLock.mutex([]() {
    for (auto it = playbacks.begin(); it != playbacks.end();) {
      if (it->second.running()) {
        it++;
      }
      else {
        it->second.join();
        playbacks.erase(it++);
      }
    }
  });
}

void Audio::clear() {
  playbacksLock.mutex([]() {
    while (playbacks.size()) {
      auto& pbackPair = playbacks.front();
      Audio& audio = *(pbackPair.first.get());
      audio.playbackLock.mutexlock();
      if (audio.playback) {
        audio.playback->stop();
      }
      audio.playbackLock.unlock();
      pbackPair.second.join();
      playbacks.pop_front();
    }
  });
}

static shared_ptr<Audio> play(
  const string& path, int loop, float volume, float* globalVolume
) {
  Audio::Playback* tmp = nullptr;
  
  playbacksLock.mutex([&]() {
    int error = 0;
    
    playbacks.emplace_back(nullptr, Thread([&]() {
      Audio::Playback pback(path, loop, volume, globalVolume, &error);
      tmp = &pback;
      if (!pback.handle) {
        return;
      }
      pback.run();
    }));
    playbacks.back().second.start();
    
    while (!tmp) {
      Thread::sleep(1);
    }
    
    if (error) {
      Log::message(Log::Error, String::from(
        "Audio \"%s\" could not be loaded", Path::get(path).c_str()
      ));
      exit(0);
    }
    
    playbacks.back().first = tmp->audio;
  });
  
  return tmp->audio;
}

} // namespace metallicar
