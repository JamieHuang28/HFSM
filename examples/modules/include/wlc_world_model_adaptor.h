#pragma once
#include <memory>
#include "wlc_world_model_interface.h"
#include "world_model.h"

class WlcWorldModelAdaptor : public WlcWorldModelInterface
{
public:
    WlcWorldModelAdaptor() = default;
    ~WlcWorldModelAdaptor() = default;

    void setWorldModel(const std::shared_ptr<WorldModel>& world_model) {
        world_model_ = world_model;
    }
    virtual bool isInWlcStatus() const override {
        bool ret = world_model_->isInWlcMode() && world_model_->isWlcConnected();
        if (ret) {
            STDLOG("isInWlcStatus");
        } else {
            STDLOG("not isInWlcStatus");
        }
        return ret;
    }
private:
    std::shared_ptr<WorldModel> world_model_;
};