#ifndef SHARE_BIKE_NETWORK_INTERFACE_H
#define SHARE_BIKE_NETWORK_INTERFACE_H

#include <event.h>
#include <event2/event.h>
#include <event2/listener.h>
#include <event2/bufferevent.h>
#include <string>
#include "global_def.h"
#include "event/event.h"

#define MESSAGE_HEADER_LEN 10
#define MESSAGE_HEADER_ID "FBEB"

enum class SESSION_STATUS {
    SS_REQUEST,
    SS_RESPONSE
};

enum class MESSAGE_STATUS {
    MS_READ_HEADER = 0,
    MS_READ_MESSAGE = 1,    // 消息传输未开始
    MS_READ_DONE = 2,       // 消息传输完毕
    MS_SENDING = 3          // 消息传输中
};

typedef struct _ConnectSession {
    char remote_ip[32];

    SESSION_STATUS session_stat;

    iEvent *request;
    MESSAGE_STATUS req_stat;

    iEvent *response;
    MESSAGE_STATUS res_stat;

    u16 eid;    //保存当前请求的事件id
    i32 fd;        //保存当前传送的文件句柄

    struct bufferevent *bev;//缓存去
    u32 message_len;        //已经读写消息的长度
    u32 read_message_len;   //已经读取的消息长度

    char *read_buf; //保存读消息的缓冲区
    char header[MESSAGE_HEADER_LEN + 1];//保存头部，10字节+1字节
    char *write_buf;
    u32 sent_len;   //已经发送的长度
    u32 read_header_len; //已读取的头部长度
} ConnectSession;

class NetworkInterface {
public:
    NetworkInterface();

    ~NetworkInterface();

    bool start(int port);

    void close();

    void session_reset(ConnectSession *cs);

    static void listener_cb(struct evconnlistener *listener, evutil_socket_t fd,
                            struct sockaddr *sock, int socklen, void *arg);

    static void handle_request(struct bufferevent *bev, void *arg); //读请求回调
    static void handle_response(struct bufferevent *bev, void *arg);//回复请求回调
    static void handle_error(struct bufferevent *bev, short event, void *arg);

    void network_event_dispatch();

    void send_response_message(ConnectSession *cs);

private:
    struct evconnlistener *listener_;
    struct event_base *base_;

    ConnectSession *m_cs;
};


#endif
