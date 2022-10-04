#ifndef SHARE_BIKE_USER_EVENT_HANDLER_H
#define SHARE_BIKE_USER_EVENT_HANDLER_H

#include "../threadpool/thread.h"
#include "event_handler.h"
#include <map>

class UserEventHandler : public iEventHandler {
public:
    UserEventHandler();

    virtual ~UserEventHandler();

    virtual iEvent *handle(const iEvent *ev);

private:
    MobileCodeRspEv *handle_mobile_code_req(MobileCodeReqEv *ev);

    i32 code_gen();

    LoginResEv *handle_login_req(LoginReqEv *ev);

private:
    // std::string mobile_;
    std::map<std::string, i32> m2c_;  // first is mobile, second is code
    pthread_mutex_t pm_;
};

#endif
