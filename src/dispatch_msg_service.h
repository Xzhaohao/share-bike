/***********************************************************
* 负责分发消息服务模块，其实就是把外部收到的消息，转化为内部事件，也就是data->msg->event的解码过程
* 然后再把事件投递到线池的消息队列，由线程池调用其process方法对事件进行处理，最终调用每个event的handler方法
* 来处理event,此时每个event handler需要subscrive该event后才会被调用到.
***********************************************************/

#ifndef SHARE_BIKE_DISPATCH_MSG_SERVICE_H
#define SHARE_BIKE_DISPATCH_MSG_SERVICE_H

#include <map>
#include <vector>
#include <queue>
#include "event/event.h"
#include "event/event_type.h"
#include "event/event_handler.h"
#include "threadpool/thread_pool.h"
#include "NetworkInterface.h"

class DispatchMsgService {
protected:
    DispatchMsgService();

public:
    virtual ~DispatchMsgService();

    virtual BOOL open();

    virtual void close();

    virtual void subscribe(u32 eid, iEventHandler *handler);

    virtual void unsubscribe(u32 eid, iEventHandler *handler);

    //把事件投递到线程池中进行处理
    virtual i32 enqueue(iEvent *ev);

    //线程池回调函数
    static void svc(void *argv);

    //对具体的事件进行分发处理
    virtual iEvent *process(const iEvent *ev);

    static DispatchMsgService *getInstance();

    static void setAddClientNumber();

    static void setDecClientNumber();

    static int getClientNumber();

    iEvent *parseEvent(const char *message, u32 len, u16 eid);

    void handleAllResponseEvent(NetworkInterface *interface);

    //发送响应信息
    void sendPesponseMessage(iEvent *ev, EventID Eid, NetworkInterface *interface);

protected:
    thread_pool_t *tp;

    static DispatchMsgService *DMS_;
    static int clientNumber;

    typedef std::vector<iEventHandler *> T_EventHandlers;
    typedef std::map<u32, T_EventHandlers> T_EventHandlersMap;
    T_EventHandlersMap subscribers_;

    bool svr_exit_{};

    static std::queue<iEvent *> response_events;
    static pthread_mutex_t queue_mutext;
    static NetworkInterface *NTIF_;
};


#endif //SHARE_BIKE_DISPATCH_MSG_SERVICE_H
