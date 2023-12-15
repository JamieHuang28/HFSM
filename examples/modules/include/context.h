#pragma once
#include <memory>
#include "world_model.h"

// data shared between FSM states and outside code
struct Context {
	unsigned cycleCount;
  bool is_finish;
  std::pair<bool, std::string> result;
  std::shared_ptr<WorldModel> world_model;
};