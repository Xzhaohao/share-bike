#include "logger.h"
#include <iostream>
#include <log4cpp/FileAppender.hh>
#include <log4cpp/OstreamAppender.hh>
#include <log4cpp/PatternLayout.hh>
#include <log4cpp/RemoteSyslogAppender.hh>
#include <log4cpp/PropertyConfigurator.hh>

Logger Logger::_instance_;

bool Logger::init(const std::string &log_conf_file) {
    try {
        log4cpp::PropertyConfigurator::configure(log_conf_file);
    } catch (log4cpp::ConfigureFailure &failure) {
        std::cerr << "load log config file" << log_conf_file << " failed with result: " << failure.what() << std::endl;
        return false;
    }

    _category_ = &log4cpp::Category::getRoot();
    return true;
}
