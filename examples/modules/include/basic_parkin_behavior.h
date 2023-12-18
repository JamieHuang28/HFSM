#pragma once
#include "utils.h"

class BasicParkinBehavior {
public:
    BasicParkinBehavior() = default;
    ~BasicParkinBehavior() = default;

    bool isFinish() const {
        STDLOGFUNCTION("");
        return prompt<YN>("isFinish?");
    }
};