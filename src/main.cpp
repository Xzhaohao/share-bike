#include "ini_config.h"
#include "config_def.h"

int main(int argc, char **argv) {
    if (argc != 2) {
        printf("请带上配置文件的路径!\n");
        return -1;
    }

    IniConfig config;
    if (!config.loadfile(std::string(argv[1]))) {
        printf("load %s failed.\n", argv[1]);
        return -2;
    }

    st_env_config conf_args = config.get_config();
    printf("[database] ip: %s port: %d user: %s password: %s db: %s\n",
           conf_args.db_ip.c_str(),
           conf_args.db_port,
           conf_args.db_user.c_str(),
           conf_args.db_password.c_str(),
           conf_args.db_name.c_str()
    );

    return 0;
}