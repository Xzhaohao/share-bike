#ifndef SHARE_BIKE_EVENT_TYPE_H
#define SHARE_BIKE_EVENT_TYPE_H

#include "../global_def.h"

typedef struct ErrorReason_ {
    i32 code;
    const char *reason;
} ErrorReason;

// 事件ID
enum EventID {

    // 获取手机号请求与响应
    EVENT_GET_MOBILE_CODE_REQ       = 0x01,
    EVENT_GET_MOBILE_CODE_RSP       = 0x02,

    // 登录请求与响应
    EVENT_LOGIN_REQ                 = 0x03,
    EVENT_LOGIN_RSP                 = 0x04,

    // 充值
    EVENT_RECHARGE_REQ              = 0x05,
    EVENT_RECHARGE_RSP              = 0x06,

    // 查询余额
    EVENT_GET_ACCOUNT_BALANCE_REQ   = 0x07,
    EVENT_GET_ACCOUNT_BALANCE_RSP   = 0x08,

    // 充值记录
    EVENT_LIST_ACCOUNT_RECORDS_REQ  = 0x09,
    EVENT_ACCOUNT_RECORDS_RSP       = 0x10,

    // 骑行记录
    EVENT_LIST_TRAVELS_REQ          = 0x11,
    EVENT_LIST_TRAVELS_RSP          = 0x12,

    EVENT_EXIT_RSP                  = 0xFE,
    // 未知事件
    EVENT_UNKNOWN                   = 0xFF
};

// 事件响应的错误码
enum ErrorCode {
    CODE_SUCCESS            = 200,
    // 无效消息
    CODE_INVALID_MSG        = 400,
    // 无效数据
    CODE_INVALID_DATA       = 404,
    // 方法不允许
    CODE_METHOD_NOT_ALLOWED = 405,
    // 服务器处理失败
    CODE_PROCESS_FAILED     = 406,

    CODE_BIKE_IS_TOOK       = 407,
    CODE_BIKE_IS_RUNNING    = 408,
    CODE_BIKE_IS_DAMAGED    = 409,
    CODE_NULL               = 0
};

const char *getReasonByErrorCode(i32 code);

#endif
