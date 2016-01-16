#ifndef BERT_NETTHREADPOOL_H
#define BERT_NETTHREADPOOL_H

#include <deque>
#include <vector>
#include <unistd.h>
#include <memory>
#include <atomic>
#include <mutex>
#include "Poller.h"
#include "Threads/ThreadPool.h"

inline long GetCpuNum()
{
    return  sysconf(_SC_NPROCESSORS_ONLN);
}

class   Socket;
typedef std::shared_ptr<Socket> PSOCKET;

namespace Internal
{


class NetThread
{
public:
    NetThread();
    virtual ~NetThread();

    bool IsAlive() const  {  return running_; }
    void Stop()           {  running_ = false;}

    void AddSocket(PSOCKET , uint32_t event);
    void ModSocket(PSOCKET , uint32_t event);
    void RemoveSocket(std::deque<PSOCKET>::iterator & iter);

protected:
    std::unique_ptr<Poller>        poller_;
    std::vector<FiredEvent > firedEvents_;    
    std::deque<PSOCKET>      tasks_;
    void  _TryAddNewTasks();

private:
    std::atomic<bool> running_;

    std::mutex   mutex_;
    typedef std::vector<std::pair<std::shared_ptr<Socket>, uint32_t> >    NewTasks; 
    NewTasks     newTasks_; 
    std::atomic<int> newCnt_;
    void _AddSocket(PSOCKET , uint32_t  event);
};

class RecvThread : public NetThread
{
public:
    void Run();
};

class SendThread : public NetThread
{
public:
    void Run();
};


///////////////////////////////////////////////
class NetThreadPool
{
    std::shared_ptr<RecvThread> recvThread_;
    std::shared_ptr<SendThread> sendThread_;

public:
    bool AddSocket(PSOCKET , uint32_t event);
    bool StartAllThreads();
    void StopAllThreads();
    
    void EnableRead(const std::shared_ptr<Socket>& sock);
    void EnableWrite(const std::shared_ptr<Socket>& sock);
    void DisableRead(const std::shared_ptr<Socket>& sock);
    void DisableWrite(const std::shared_ptr<Socket>& sock);

    static NetThreadPool& Instance()
    {
        static  NetThreadPool    pool;
        return  pool;
    }
};

}

#endif

