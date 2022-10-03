#include "event_type.h"

static ErrorReason EER[] =
        {
                {CODE_SUCCESS,            "OK."},
                {CODE_INVALID_MSG,        "Invalid message."},
                {CODE_INVALID_DATA,       "Invalid data."},
                {CODE_METHOD_NOT_ALLOWED, "Method not allowed."},
                {CODE_PROCESS_FAILED,     "Process failed."},
                {CODE_BIKE_IS_TOOK,       "Bike is took."},
                {CODE_BIKE_IS_RUNNING,    "Bike is running."},
                {CODE_BIKE_IS_DAMAGED,    "Bike is damaged."},
                {CODE_NULL,               "Undefined"}
        };

const char *getReasonByErrorCode(i32 code) {
    i32 i = 0;
    for (i = 0; EER[i].code != CODE_NULL; i++) {
        if (EER[i].code == code) {
            return EER[i].reason;
        }
    }
    return EER[i].reason;
}