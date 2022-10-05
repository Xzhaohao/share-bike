#include "dispatch_msg_service.h"
#include <algorithm>
#include "protobuf/bike.pb.h"
#include "event/events_def.h"

DispatchMsgService *DispatchMsgService::DMS_ = nullptr;
int DispatchMsgService::clientNumber = 0;
std::queue<iEvent *> DispatchMsgService::response_events;
pthread_mutex_t DispatchMsgService::queue_mutext;
NetworkInterface *DispatchMsgService::NTIF_ = nullptr;

DispatchMsgService::DispatchMsgService() : tp(nullptr) {

}

DispatchMsgService::~DispatchMsgService() = default;

BOOL DispatchMsgService::open() {
    svr_exit_ = false;

    thread_mutex_create(&queue_mutext);
    LOG_DEBUG("线程池初始化......");
    tp = thread_pool_init();
    LOG_DEBUG("当前线程池中的线程数为：tp->threads=%d", tp->threads);
    LOG_DEBUG("线程池能处理的最大任务数为：tp->max_queue=%d", tp->max_queue);
    LOG_DEBUG("线程池初始化成功！");
    return tp ? TURE : FALSE;
}

void DispatchMsgService::close() {
    svr_exit_ = true;

    thread_pool_destroy(tp);
    thread_mutex_destroy(&queue_mutext);
    subscribers_.clear();

    tp = nullptr;
}

void DispatchMsgService::subscribe(u32 eid, iEventHandler *handler) {
    auto iter = subscribers_.find(eid);
    if (iter != subscribers_.end()) {

        auto hdl_iter = std::find(iter->second.begin(), iter->second.end(), handler);
        if (hdl_iter == iter->second.end()) {
            iter->second.push_back(handler);

        }
    } else {
        subscribers_[eid].push_back(handler);
    }
}

void DispatchMsgService::unsubscribe(u32 eid, iEventHandler *handler) {
    auto iter = subscribers_.find(eid);
    if (iter != subscribers_.end()) {
        auto hdl_iter = std::find(iter->second.begin(), iter->second.end(), handler);
        if (hdl_iter != iter->second.end()) {
            iter->second.erase(hdl_iter);//相当于擦除键值//
        }
    }
}

i32 DispatchMsgService::enqueue(iEvent *ev) {
    if (nullptr == ev) {
        return -1;
    }
    auto *cs = (ConnectSession *) ev->get_args();
    LOG_DEBUG("将客户端[%s][%p]请求投递到任务队列中.......", cs->remote_ip, cs->bev);
    thread_task_t *task = thread_task_alloc(0);
    task->handler = DispatchMsgService::svc;
    task->ctx = ev;

    return thread_task_post(tp, task);
}

void DispatchMsgService::svc(void *argv) {
    DispatchMsgService *dms = DispatchMsgService::getInstance();

    auto *ev = (iEvent *) argv;
    if (!dms->svr_exit_) {
        LOG_DEBUG("DispatchMsgService::svc....\n");
        printf("DispatchMsgService::svc....\n");
        printf("!ev->eid=%d\n", ev->get_eid());

        iEvent *rsp = dms->process(ev);
        if (rsp) {
            rsp->dump(std::cout);
            rsp->set_args(ev->get_args());

        } else {
            //生成终止响应事件
            rsp = new ExitRspEv();
            rsp->set_args(ev->get_args());
        }

        auto *cs = (ConnectSession *) rsp->get_args();
        LOG_DEBUG("正在将事件[%s][%p]放入响应事件队列当中.......", cs->remote_ip, cs->bev);
        thread_mutex_lock(&queue_mutext);
        int size = rsp->Bytesize();
        response_events.push(rsp); //无论什么事件，都统一放入响应事件队列当中
        thread_mutex_unlock(&queue_mutext);
    }
}

iEvent *DispatchMsgService::process(const iEvent *ev) {
    LOG_DEBUG("DispatchMsgService::process - ev: %p\n", ev);

    if (ev == nullptr) {
        return nullptr;
    }
    u32 eid = ev->get_eid();
    LOG_DEBUG("DispatchMsgService::process-eid: %u\n", eid);
    printf("DispatchMsgService::process-eid: %u\n", eid);
    if (eid == EEVNETID_UNKOWN) {
        LOG_WARN("DispatchMsgService: unknow event id %d\n", eid);
        printf("DispatchMsgService: unknow event id %d\n", eid);
        return nullptr;
    }
    auto handlers = subscribers_.find(eid);
    if (handlers == subscribers_.end()) {
        LOG_WARN("DispatchMsgService：no any event handler subscribed: %d\n", eid);
        printf("DispatchMsgService：no any event handler subscribed: %d\n", eid);
        return nullptr;
    }
    iEvent *rsp = NULL;
    for (auto handler : handlers->second) {
        LOG_DEBUG("DispatchMsgService：get handler: %s\n", handler->get_name().c_str());
        printf("DispatchMsgService: get handler: %s\n", handler->get_name().c_str());
        printf("!ev: eid=%d\n", ev->get_eid());

        rsp = handler->handle(ev);//可以使用vector或list返回多个 rsp
    }
    return rsp;
}

DispatchMsgService *DispatchMsgService::getInstance() {
    if (DMS_ == nullptr) {
        DMS_ = new DispatchMsgService();

    }
    return DMS_;
}

void DispatchMsgService::setAddClientNumber() {
    ++clientNumber;
}

void DispatchMsgService::setDecClientNumber() {
    --clientNumber;
}


int DispatchMsgService::getClientNumber() {
    return clientNumber;
}

iEvent *DispatchMsgService::parseEvent(const char *message, u32 len, u16 eid) {
    if (!message) {
        LOG_ERROR("DispatchMsgService::parseEvent - message is null[eid=%d]\n", eid);
        return nullptr;
    }
    switch (eid) {
        case EVENT_GET_MOBILE_CODE_REQ: {// 获取手机验证码
            tutorial::mobile_request mr;
            mr.set_mobile(message);
            LOG_DEBUG("客户端正在尝试获取手机验证码......");
            LOG_DEBUG("正在从客户端序列化的massage中解析数据.......");
            if (mr.ParseFromArray(message, len))// 解析数据
            {
                LOG_DEBUG("解析数据完成.客户端手机号为：%s", mr.mobile().c_str());
                auto *ev = new MobileCodeReqEv(mr.mobile());
                return ev;
            }
            break;
        }
        case EVENT_LOGIN_REQ: {// 登陆
            LOG_DEBUG("客户端正在尝试登陆......");
            LOG_DEBUG("正在从客户端序列化的massage中解析数据.......");
            tutorial::login_request lr;
            // 调试这里粗问题
            lr.set_icode(eid);
            if (lr.ParseFromArray(message, len)) {
                LOG_DEBUG("解析数据完成.客户端手机号为：%s， 验证码为：%d", lr.mobile().c_str(), lr.icode());
                LoginReqEv *ev = new LoginReqEv(lr.mobile(), lr.icode());
                return ev;
            }
            break;
        }
        case EVENT_LIST_ACCOUNT_RECORDS_REQ: {// 获取账户信息请求
            LOG_DEBUG("客户端结束骑行，正在获取账户信息......");
            LOG_DEBUG("正在从客户端序列化的massage中解析数据.......");
            tutorial::list_account_records_request recordRequest;
            if (recordRequest.ParseFromArray(message, len)) {

                RecordsRequestEv *ev = new RecordsRequestEv(recordRequest.mobile());
                return ev;
            }
            break;
        }
            //获取账户信息响应
        case EVENT_ACCOUNT_RECORDS_RSP: {
            LOG_DEBUG("客户端结束骑行，账户信息已返回......");
            LOG_DEBUG("正在从客户端序列化的massage中解析数据.......");
            tutorial::list_account_records_response larr;
            if (larr.ParseFromArray(message, len)) {
                const tutorial::list_account_records_response_account_record &ar = larr.records(0);

                RecordsResponseEv *ev = new RecordsResponseEv(ar.);
                return ev;
            }
            break;
        }

        default:
            break;
    }

    return nullptr;
}

void DispatchMsgService::handleAllResponseEvent(NetworkInterface *interface) {
    bool done = false;
    while (!done) {
        iEvent *ev = nullptr;
        thread_mutex_lock(&queue_mutext);
        if (!response_events.empty()) {
            printf("!response_events.empty()\n");
            // 拿出响应的第一个事件
            ev = response_events.front();

            int i = ev->Bytesize();
            response_events.pop();
        } else {
            done = true;
        }
        thread_mutex_unlock(&queue_mutext);

        if (!done) {
            u32 EventId = ev->get_eid();
            printf("get_eid()......\nEventId=%d\n", EventId);
            if (EventId == EVENT_GET_MOBILE_CODE_RSP) {// 获取手机验证码响应
                LOG_DEBUG(
                        "正在获取手机验证码响应--DispatchMsgService::handleAllResponseEvent - id:EVENT_GET_MOBILE_CODE_RSP\n");
                printf("DispatchMsgService::handleAllResponseEvent - id:EVENT_GET_MOBILE_CODE_RSP\n");

                // 发送响应信息
                sendPesponseMessage(ev, EVENT_GET_MOBILE_CODE_RSP, interface);

            } else if (EventId == EVENT_LOGIN_RSP) {// 登陆响应
                LOG_DEBUG("正在获取手机登陆响应--DispatchMsgService::handleAllResponseEvent - id:EVENT_LOGIN_RSP\n");
                sendPesponseMessage(ev, EVENT_LOGIN_RSP, interface);
            } else if (EventId == EVNET_EXIT_RSP) {// 退出响应
                LOG_DEBUG("退出响应--ev->get_eid() == EVNET_EXIT_RSP\n");
                auto *cs = (ConnectSession *) ev->get_args();
                cs->response = ev;
                interface->send_response_message(cs);
            }
        }
    }
}

void DispatchMsgService::sendPesponseMessage(iEvent *ev, EventID Eid, NetworkInterface *interface) {
    auto *cs = (ConnectSession *) ev->get_args();
    cs->response = ev;

    // 系列化请求数据
    cs->message_len = ev->Bytesize();
    cs->write_buf = new char[cs->message_len + MESSAGE_HEADER_LEN];

    // 组装头部
    memcpy(cs->write_buf, MESSAGE_HEADER_ID, 4);
    *(u16 *) (cs->write_buf + 4) = Eid;
    *(i32 *) (cs->write_buf + 6) = cs->message_len;
    LOG_DEBUG("正在序列化数据......");
    ev->SerializeToArray(cs->write_buf + MESSAGE_HEADER_LEN, cs->message_len);

    interface->send_response_message(cs);
}

