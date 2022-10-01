#include "ini_config.h"
#include "config_def.h"
#include "logger.h"

#define START_FAILED -1
#define INIT_LOG_FAILED -2
#define INIT_CONFIG_FAILED -3

int main(int argc, char **argv) {
    if (argc < 2) {
        printf("please input share_bike <config file path> <log file path>!\n");
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
    LOG_INFO("[database] ip: %s port: %d user: %s password: %s db: %s\n",
             conf_args.db_ip.c_str(),
             conf_args.db_port,
             conf_args.db_user.c_str(),
             conf_args.db_password.c_str(),
             conf_args.db_name.c_str()
    );

    return 0;
}