#pragma once
#include <memory>
#include "world_model.h"
#include "wlc_behavior.h"
#include "wlc_world_model_adaptor.h"

// data shared between FSM states and outside code
struct Context {
  Context(std::shared_ptr<WorldModel> wm) : is_finish(false), world_model(wm) {
    wlc_wm_adaptor = std::make_shared<WlcWorldModelAdaptor>();
    wlc_wm_adaptor->setWorldModel(world_model);
    wlc_behavior = std::make_shared<WlcBehavior>(wlc_wm_adaptor);
  }
  bool is_finish;
  std::pair<bool, std::string> result;
  std::shared_ptr<WorldModel> world_model;
  std::shared_ptr<WlcWorldModelAdaptor> wlc_wm_adaptor;
  std::shared_ptr<WlcBehavior> wlc_behavior;
};