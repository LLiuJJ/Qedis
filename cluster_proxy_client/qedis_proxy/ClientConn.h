#ifndef BERT_CLIENTCONN_H
#define BERT_CLIENTCONN_H

#include <queue>
#include <string>
#include <vector>
#include <unordered_map>

#include "net/Connection.h"
#include "ServerProtocol.h"

class ClientConn
{
public:
    explicit
    ClientConn(ananas::Connection* conn);

    ananas::PacketLen_t OnRecv(ananas::Connection* conn, const char* data, ananas::PacketLen_t len);

private:
    ServerProtocol proto_;
    ananas::Connection* hostConn_;
    std::string reply_;
};

#endif

