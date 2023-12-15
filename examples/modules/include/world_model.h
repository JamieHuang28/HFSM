#pragma once
#include <iostream>
#include "utils.h"
#include "context.h"

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

    void setRequest(Context &context) const {
        context.setRequest(prompt<int>("please input request, 0 for none, 1 for ParkIn, 2 for Off, 3 for Wait"));
    }
};