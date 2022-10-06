#ifndef SHARE_BIKE_USER_SERVICE_H
#define SHARE_BIKE_USER_SERVICE_H

#include <memory>
#include "sqlconnection.h"

class UserService {
public:
    UserService(std::shared_ptr<MysqlConnection> sql_conn);

    bool exist(std::string &mobile);

    bool insert(std::string &mobile);

private:
    std::shared_ptr<MysqlConnection> sql_conn_;

};

#endif
