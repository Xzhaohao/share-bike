#include "ini_config.h"
#include "config_def.h"
#include "logger.h"
#include "protobuf/bike.pb.h"
#include "event/events_def.h"

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

    return 0;
}