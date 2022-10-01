#ifndef SHARE_BIKE_COMMON_INI_CONFIG_H
#define SHARE_BIKE_COMMON_INI_CONFIG_H

#include "config_def.h"

class IniConfig {
private:
    st_env_config _config;
    bool _is_loaded;

public:
    IniConfig();

    ~IniConfig();

    bool loadfile(const std::string &path);

    const st_env_config &get_config();
};

#endif