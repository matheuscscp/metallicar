/*
 * ByteQueue.hpp
 *
 *  Created on: Jul 21, 2014
 *      Author: Pimenta
 */

#ifndef BYTEQUEUE_HPP_
#define BYTEQUEUE_HPP_

// standard
#include <vector>
#include <cstring>
#include <cstdint>
#include <string>

class ByteQueue {
  private:
    std::vector<uint8_t> buf;
  public:
    ByteQueue(uint32_t size = 0);
    uint32_t size() const;
    void resize(uint32_t size);
    void* ptr() const;
    
    ByteQueue& push(const ByteQueue& data);
    ByteQueue& push(const void* data, uint32_t maxlen);
    ByteQueue& push(const std::string& data, bool withoutNullTermination = false);
    template <typename T> ByteQueue& push(T data) {
      buf.insert(buf.end(), (uint8_t*)&data, ((uint8_t*)&data) + sizeof(T));
      return *this;
    }
    template <typename T> ByteQueue& push(const std::vector<T>& data) {
      push(uint32_t(data.size()));
      buf.insert(buf.end(), (uint8_t*)&data[0], ((uint8_t*)&data[0]) + sizeof(T)*data.size());
      return *this;
    }
    
    void pop(ByteQueue& data);
    uint32_t pop(void* data, uint32_t maxlen);
    std::string pop(uint32_t maxlen);
    template <typename T> T pop() {
      T data = 0;
      uint32_t total = sizeof(T) <= buf.size() ? sizeof(T) : buf.size();
      if (!total)
        return data;
      memcpy((void*)&data, (const void*)&buf[0], total);
      buf.erase(buf.begin(), buf.begin() + total);
      return data;
    }
    template <typename T> std::vector<T> pop(uint32_t* size) {
      std::vector<T> data(pop<uint32_t>());
      if (size)
        *size = data.size();
      pop((void*)&data[0], sizeof(T)*data.size());
      return data;
    }
};

template <> inline ByteQueue& ByteQueue::push<std::string>(std::string data) {
  buf.insert(buf.end(), (uint8_t*)data.c_str(), ((uint8_t*)data.c_str()) + data.size());
  buf.push_back(uint8_t(0));
  return *this;
}

template <> inline ByteQueue& ByteQueue::push<std::string>(const std::vector<std::string>& data) {
  push(uint32_t(data.size()));
  for (auto& str : data)
    push(str);
  return *this;
}

template <> inline std::string ByteQueue::pop<std::string>() {
  std::string data;
  char c;
  while ((c = pop<char>()) != '\0')
    data += c;
  return data;
}

template <> inline std::vector<std::string> ByteQueue::pop<std::string>(uint32_t* size) {
  std::vector<std::string> data(pop<uint32_t>());
  if (size)
    *size = data.size();
  for (auto& str : data)
    str = pop<std::string>();
  return data;
}

#endif /* BYTEQUEUE_HPP_ */
