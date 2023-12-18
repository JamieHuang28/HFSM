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
            STDLOGPROCESS("isInWlcStatus");
        } else {
            STDLOGPROCESS("not isInWlcStatus");
        }
        return ret;
    }
    virtual RequestType request() const override {
        return world_model_->request();
    }
private:
    std::shared_ptr<WorldModel> world_model_;
};