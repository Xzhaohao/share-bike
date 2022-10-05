#include "bus_processor.h"

BusinessProcessor::BusinessProcessor(std::shared_ptr<MysqlConnection> sqlconn)
        : mysqlconn_(sqlconn), ueh_(new UserEventHandler()) {

}

BusinessProcessor::~BusinessProcessor() {
    ueh_.reset();
}

bool BusinessProcessor::init() {
    SqlTables tables(mysqlconn_);
    LOG_DEBUG("数据库正在初始化......");

    if (!tables.CreateBikeTable()) {
        LOG_ERROR("create shared_bike table failed!\n");
        printf("create shared_bike table failed!\n");
        return false;
    }
    if (!tables.CreateUserInfo()) {
        LOG_ERROR("create user table failed！\n");
        printf("create user table failed！\n");
        return false;
    }
    LOG_DEBUG("数据库初始化完成......\n");
    return true;
}