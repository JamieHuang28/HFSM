#pragma once

#include <hfsm/machine_single.hpp>
#include <iostream>

#ifdef HFSM_ENABLE_LOG_INTERFACE
struct HfsmCoutLogger : hfsm::LoggerInterface {

  static HfsmCoutLogger *Instance() {
    static HfsmCoutLogger instance;
    return &instance;
  }
  // hfsm::LoggerInterface
  void record(const std::type_index & /*state*/, const char *const stateName,
              const Method /*method*/, const char *const methodName) override {
    std::cout << stateName << "::" << methodName << "()\n";
  }
};
#endif
