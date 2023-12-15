#pragma once
#include <iostream>
#include "utils.h"

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
        request_ = request;
    }
    const int& request() const {
        return request_;
    }
private:
    int request_;
};