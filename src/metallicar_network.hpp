/*
 * metallicar_network.hpp
 *
 *  Created on: Jul 21, 2014
 *      Author: Pimenta
 */

#ifndef METALLICAR_NETWORK_HPP_
#define METALLICAR_NETWORK_HPP_

// lib
#include "SDL_net.h"

// local
#include "ByteQueue.hpp"

namespace metallicar {

class Address {
  public:
    uint32_t ip;
    uint16_t port;
    
    Address(uint32_t ip = 0, uint16_t port = 0);
    Address(const std::string& ip, const std::string& port = "");
    static Address local();
    
    std::string toString() const;
    static uint32_t ntohl(uint32_t ip);
    static uint32_t htonl(uint32_t ip);
    static uint16_t ntohs(uint16_t port);
    static uint16_t htons(uint16_t port);
    
    bool isPrivateNetwork();
    static bool isPrivateNetwork(uint32_t ip);
};

class UDPSocket {
  private:
    int sd;
    uint32_t maxlen;
  public:
    UDPSocket(uint32_t maxlen);
    UDPSocket(const std::string& port, uint32_t maxlen);
    UDPSocket(const Address& multicastAddress, uint32_t maxlen);
    ~UDPSocket();
    void send(const Address& address, const ByteQueue& data);
    ByteQueue recv(Address& address);
};

class TCPSocket {
  protected:
    TCPsocket sd;
    TCPSocket();
  public:
    virtual ~TCPSocket();
};

class TCPConnection : public TCPSocket {
  public:
    TCPConnection(const Address& addr);
    
    TCPConnection& send(const ByteQueue& data);
    TCPConnection& send(const void* data, uint32_t maxlen);
    TCPConnection& send(
      const std::string& data,
      bool withoutNullTermination = false
    );
    template <typename T> TCPConnection& send(T data) {
      return send((const void*)&data, sizeof(T));
    }
    template <typename T> TCPConnection& send(const std::vector<T>& data) {
      if (sd) {
        uint32_t tmp = data.size();
        SDLNet_TCP_Send(sd, (const void*)&tmp, sizeof(uint32_t));
        SDLNet_TCP_Send(sd, (const void*)&data[0], sizeof(T)*data.size());
      }
      return *this;
    }
    
    void recv(ByteQueue& data);
    uint32_t recv(void* data, uint32_t maxlen);
    std::string recv(uint32_t maxlen);
    template <typename T> T recv() {
      T data = 0;
      recv(&data, sizeof(T));
      return data;
    }
    template <typename T> std::vector<T> recv(uint32_t* size) {
      std::vector<T> data(recv<uint32_t>());
      if (size)
        *size = data.size();
      recv((void*)&data[0], sizeof(T)*data.size());
      return data;
    }
};

template <>
inline TCPConnection& TCPConnection::send<std::string>(std::string data) {
  return send(data, false);
}

template <>
inline TCPConnection& TCPConnection::send<std::string>(
  const std::vector<std::string>& data
) {
  if (sd) {
    uint32_t tmp = data.size();
    SDLNet_TCP_Send(sd, (const void*)&tmp, sizeof(uint32_t));
    for (auto& str : data) {
      SDLNet_TCP_Send(sd, (const void*)str.c_str(), str.size() + 1);
    }
  }
  return *this;
}

template <> inline std::string TCPConnection::recv<std::string>() {
  std::string data;
  char c;
  while ((c = recv<char>()) != '\0')
    data += c;
  return data;
}

template <>
inline std::vector<std::string> TCPConnection::recv<std::string>(
  uint32_t* size
) {
  std::vector<std::string> data(recv<uint32_t>());
  if (size)
    *size = data.size();
  for (auto& str : data)
    str = recv<std::string>();
  return data;
}

class TCPServer : public TCPSocket {
  public:
    TCPServer(const std::string& port);
    TCPConnection* accept();
};

} // namespace metallicar

#endif /* METALLICAR_NETWORK_HPP_ */
