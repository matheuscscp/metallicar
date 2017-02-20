/*
 * ByteQueue.cpp
 *
 *  Created on: Jul 21, 2014
 *      Author: Pimenta
 */

// this
#include "ByteQueue.hpp"

using namespace std;

ByteQueue::ByteQueue(uint32_t size) {
  buf.resize(size);
}

uint32_t ByteQueue::size() const {
  return buf.size();
}

void ByteQueue::resize(uint32_t size) {
  buf.resize(size);
}

void* ByteQueue::ptr() const {
  return buf.size() ? (void*)&buf[0] : nullptr;
}

ByteQueue& ByteQueue::push(const ByteQueue& data) {
  buf.insert(buf.end(), (uint8_t*)data.ptr(), ((uint8_t*)data.ptr()) + data.size());
  return *this;
}

ByteQueue& ByteQueue::push(const void* data, uint32_t maxlen) {
  buf.insert(buf.end(), (uint8_t*)data, ((uint8_t*)data) + maxlen);
  return *this;
}

ByteQueue& ByteQueue::push(const string& data, bool withoutNullTermination) {
  buf.insert(buf.end(), (uint8_t*)data.c_str(), ((uint8_t*)data.c_str()) + data.size());
  if (!withoutNullTermination)
    buf.push_back(uint8_t(0));
  return *this;
}

void ByteQueue::pop(ByteQueue& data) {
  uint32_t total = data.size() <= buf.size() ? data.size() : buf.size();
  if (!total)
    return;
  memcpy(data.ptr(), (const void*)&buf[0], total);
  buf.erase(buf.begin(), buf.begin() + total);
  data.resize(total);
}

uint32_t ByteQueue::pop(void* data, uint32_t maxlen) {
  uint32_t total = maxlen <= buf.size() ? maxlen : buf.size();
  if (!total)
    return 0;
  memcpy(data, (const void*)&buf[0], total);
  buf.erase(buf.begin(), buf.begin() + total);
  return total;
}

string ByteQueue::pop(uint32_t maxlen) {
  string data(maxlen <= buf.size() ? maxlen : buf.size(), '0');
  if (data.size()) {
    memcpy((void*)data.c_str(), (const void*)&buf[0], data.size());
    buf.erase(buf.begin(), buf.begin() + data.size());
  }
  return data;
}
