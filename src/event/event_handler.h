#ifndef SHARE_BIKE_EVENT_HANDLER_H
#define SHARE_BIKE_EVENT_HANDLER_H

#include "event.h"

class iEventHandler {
public:
    // 虚函数，将根据指针类型指向的‘对象类型’确定调用的函数，而非指针的类型
    virtual iEvent *handle(const iEvent *ev) { return nullptr; };

    iEventHandler() {};

    virtual ~iEventHandler() {};

    std::string &get_name() { return name_; }


private:
    std::string name_;

};

#endif
