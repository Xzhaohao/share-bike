#include "ini_config.h"
#include <iniparser/iniparser.h>

IniConfig::IniConfig() : _is_loaded(false) {
}

IniConfig::~IniConfig() = default;

bool IniConfig::loadfile(const std::string &path) {
    dictionary *ini;

    ini = iniparser_load(path.c_str());
    if (ini == nullptr) {
        fprintf(stderr, "cannot parse file: %s\n", path.c_str());
        return false;
    }

    const char *ip = iniparser_getstring(ini, "database:ip", "127.0.0.1");
    int port = iniparser_getint(ini, "database:port", 3306);
    const char *user = iniparser_getstring(ini, "database:user", "root");
    const char *password = iniparser_getstring(ini, "database:password", "123456");
    const char *db = iniparser_getstring(ini, "database:db", "share_bike");
    int server_port = iniparser_getint(ini, "server:port", 9090);

    _config = st_env_config(
            std::string(ip), port, std::string(user),
            std::string(password), std::string(db), server_port);

    iniparser_freedict(ini);

    _is_loaded = true;

    return _is_loaded;
}

const st_env_config &IniConfig::get_config() {
    return _config;
}