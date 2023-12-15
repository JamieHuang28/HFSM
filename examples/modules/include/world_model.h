#pragma once
#include <iostream>
#include "utils.h"

enum RequestType {
    NONE = 0,
    PARKIN = 1,
    OFF = 2,
    WAIT = 3
};

class WorldModel {
public:
    WorldModel() = default;
    ~WorldModel() = default;

    void update() {
        std::cout << "WorldModel::update()" << std::endl;
    }

    bool isInWlcMode() const {
        return prompt<YN>("isInWlcMode?");
    }
    bool isWlcConnected() const {
        return prompt<YN>("isWlcConnected?");
    }

    void setRequest(int request) {
        request_ = static_cast<RequestType>(request);
    }
    const RequestType& request() const {
        return request_;
    }
private:
    RequestType request_;
};