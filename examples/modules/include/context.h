#pragma once

// data shared between FSM states and outside code
class Context {
  int request_;
public:
	unsigned cycleCount;
  bool is_finish;
  std::pair<bool, std::string> result;
public:
  Context() : request_(0), cycleCount(0), is_finish(false) {}
  ~Context() {}
  bool isParkinRequested() const { return request_ == 1; }
  bool isWaitREquested() const { return request_ == 3; }
  bool isOffRequested() const { return request_ == 2; }
  void setRequest(int request) { request_ = request; }
};