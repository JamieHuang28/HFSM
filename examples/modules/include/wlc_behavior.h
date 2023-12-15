#pragma once
#include <memory>
#include "utils.h"
#include "wlc_world_model_interface.h"
#include "context.h"

class WlcBehavior {
public:
    WlcBehavior(const std::shared_ptr<WlcWorldModelInterface> wlc_wm_itf) : wlc_world_model_interface_(wlc_wm_itf) {}
    ~WlcBehavior() {}

    void disableFinishFlag(Context &context) {
        STDLOGINVOKE(": don not finish when in wlc mode");
        context.is_finish = !wlc_world_model_interface_->isInWlcStatus() && context.is_finish;
    }
    bool isFinish() const {
        if (wlc_world_model_interface_->isInWlcStatus() && wlc_world_model_interface_->request() != 3) {
            return false;
        } else {
            return true;
        }
    }
private:
    std::shared_ptr<WlcWorldModelInterface> wlc_world_model_interface_;
};