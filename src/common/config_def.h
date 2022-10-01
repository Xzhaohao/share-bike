#ifndef SHARE_BIKE_COMMON_CONFIG_DEF_H
#define SHARE_BIKE_COMMON_CONFIG_DEF_H

#include <string>

typedef struct st_env_config {
    // 数据库配置
    std::string db_ip;
    unsigned short db_port{};
    std::string db_user;
    std::string db_password;
    std::string db_name;

    // 服务器配置
    unsigned short server_port{};

    st_env_config() = default;

    st_env_config(
            const std::string &db_ip, unsigned short db_port,
            const std::string &db_user, const std::string &db_password,
            const std::string &db_name, unsigned short server_port
    ) {
        this->db_ip = db_ip;
        this->db_port = db_port;
        this->db_user = db_user;
        this->db_password = db_password;
        this->db_name = db_name;
        this->server_port = server_port;
    };

    st_env_config &operator=(const st_env_config &config) {
        if (this != &config) {
            this->db_ip = config.db_ip;
            this->db_port = config.db_port;
            this->db_user = config.db_user;
            this->db_password = config.db_password;
            this->db_name = config.db_name;
            this->server_port = config.server_port;
        }
        return *this;
    }
} _st_env_config;

#endif