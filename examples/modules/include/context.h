#pragma once

// data shared between FSM states and outside code
struct Context {
  int request;
	unsigned cycleCount;
  bool is_finish;
  std::pair<bool, std::string> result;
};