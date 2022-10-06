#include "ini_config.h"
#include "config_def.h"
#include "logger.h"
#include "protobuf/bike.pb.h"
#include "network_interface.h"
#include "dispatch_msg_service.h"
#include "sqlconnection.h"
#include "sql_tables.h"
#include "bus_processor.h"

using namespace std;

#define START_FAILED -1
#define INIT_LOG_FAILED -2
#define INIT_CONFIG_FAILED -3

int main(int argc, char **argv) {
    if (argc < 2) {
        printf("please input share_bike <config file path> <log file path>!\n");
        printf("example: ./share_bike ../../conf/application.ini ../../conf/alo/conf\n");
        return START_FAILED;
    }

    // 初始化日志
    if (!Logger::instance()->init(std::string(argv[2]))) {
        fprintf(stderr, "init log module failed.\n");
        return INIT_LOG_FAILED;
    }

    // 初始化项目配置文件
    IniConfig config;
    if (!config.loadfile(std::string(argv[1]))) {
        LOG_ERROR("load %s failed.\n", argv[1]);
        return INIT_CONFIG_FAILED;
    }

    st_env_config conf_args = config.get_config();

    std::shared_ptr<MysqlConnection> mysqlconn(new MysqlConnection);

    if (!mysqlconn->Init(conf_args.db_ip.c_str(), conf_args.db_port, conf_args.db_user.c_str(),
                         conf_args.db_password.c_str(), conf_args.db_name.c_str())) {
        LOG_ERROR("Database init failed. exit!\n");
        return -4;
    }

    // 连接数据库
    BusinessProcessor busPro(mysqlconn);
    busPro.init();

    DispatchMsgService *DMS = DispatchMsgService::getInstance();
    DMS->open();

    auto *NTIF = new NetworkInterface();
    NTIF->start(conf_args.server_port);
    LOG_DEBUG("等待客户端的连接.......\n\n");
    printf("等待客户端的连接.......\n\n");
    while (1) {
        NTIF->network_event_dispatch();
    }

    return 0;
}