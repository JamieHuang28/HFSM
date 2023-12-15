#pragma once
#include "utils.h"

class BasicParkinBehavior {
public:
    BasicParkinBehavior() = default;
    ~BasicParkinBehavior() = default;

    bool isFinish() const {
        STDLOGINVOKE("");
        return prompt<YN>("isFinish?");
    }
};