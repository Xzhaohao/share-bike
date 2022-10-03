#ifndef SHARE_BIKE_EVENTS_DEF_H
#define SHARE_BIKE_EVENTS_DEF_H

#include "event.h"
#include "../protobuf/bike.pb.h"

class MobileCodeReqEv : public iEvent {
public:
    MobileCodeReqEv(const std::string &mobile) : iEvent(EVENT_GET_MOBILE_CODE_REQ, iEvent::generate_seq_no()) {
        msg_.set_mobile(mobile);
    };

    const std::string &get_mobile() { return msg_.mobile(); };

    virtual std::ostream &dump(std::ostream &out) const;

private:
    tutorial::mobile_request msg_;
};

class MobileCodeRspEv : public iEvent {
public:
    MobileCodeRspEv(i32 code, i32 icode) :iEvent(EVENT_GET_MOBILE_CODE_RSP, iEvent::generate_seq_no()) {
        msg_.set_code(code);    // 代号
        msg_.set_icode(icode);  // 验证码
        msg_.set_data(getReasonByErrorCode(code));
    }

    const i32 get_code() { return msg_.code(); };

    const i32 get_icode() { return msg_.icode(); };

    const std::string &get_data() { return msg_.data(); };

    virtual std::ostream &dump(std::ostream &out) const;

private:
    tutorial::mobile_response msg_;
};

#endif // SHARE_BIKE_EVENTS_DEF_H
