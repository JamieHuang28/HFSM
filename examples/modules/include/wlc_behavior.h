#pragma once
#include <memory>
#include "utils.h"
#include "wlc_world_model_interface.h"

class WlcBehavior {
public:
    WlcBehavior(const std::shared_ptr<WlcWorldModelInterface> wlc_wm_itf) : wlc_world_model_interface_(wlc_wm_itf) {}
    ~WlcBehavior() {}
    
    bool isFinish() const {
        STDLOGFUNCTION(": don not finish when in wlc mode");
        if (wlc_world_model_interface_->isInWlcStatus() && wlc_world_model_interface_->request() != RequestType::WAIT) {
            return false;
        } else {
            return true;
        }
    }
private:
    std::shared_ptr<WlcWorldModelInterface> wlc_world_model_interface_;
};