#ifndef SHARE_BIKE_COMMON_LOGGER_H
#define SHARE_BIKE_COMMON_LOGGER_H

#include <string>
#include <log4cpp/Category.hh>

class Logger {
public:
    bool init(const std::string &log_conf_file);

    static Logger *instance() {
        return &_instance_;
    }

    log4cpp::Category *get_handle() {
        return _category_;
    }

protected:
    static Logger _instance_;
    log4cpp::Category *_category_;
};

#define LOG_INFO Logger::instance()->get_handle()->info
#define LOG_DEBUG Logger::instance()->get_handle()->debug
#define LOG_ERROR Logger::instance()->get_handle()->error
#define LOG_WARN Logger::instance()->get_handle()->warn

#endif
