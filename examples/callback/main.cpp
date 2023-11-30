#include <map>
#include <string>

#define HFSM_ENABLE_LOG_INTERFACE
#include <hfsm/machine_single.hpp>
#include <functional>
#include <memory>
#include "machine_logger.h"

// simpify the definition of State with callback
#ifndef DEFINE_STATE_CLASS
#define DEFINE_STATE_CLASS(classname, baseclassname)                           \
  class classname : public baseclassname {                                     \
  public:                                                                      \
    void enter(const Context &context) {                                       \
      context.callback_on_entry<classname>();                                  \
    }                                                                          \
    void transition(Control &control, Context &context) {                      \
      context.callback_on_transition<classname>(control);                      \
    }                                                                          \
    void update(const Context &context) {                                      \
      context.callback_on_update<classname>();                                 \
    }                                                                          \
    void leave(const Context &context) {                                       \
      context.callback_on_leave<classname>();                                  \
    }                                                                          \
  };
#endif

class ApaStateMachine {
public:
  ApaStateMachine() {
    machine_ = std::make_unique<hfsm::Machine<Context>::PeerRoot<COMPOSITE>>(context_, HfsmCoutLogger::Instance());
    setupCallbackFunctions();
  }
  ~ApaStateMachine() {}

  void update() { machine_->update(); }

private:
  class Context {
    std::map<std::string, std::function<void()>> entry_callback_map;
    std::map<std::string, std::function<void()>> update_callback_map;
    std::map<std::string,
             std::function<void(hfsm::Machine<Context>::Control &)>>
        transition_callback_map;
    std::map<std::string, std::function<void()>> leave_callback_map;

  public:
    template <typename T>
    void register_entry_callback(std::function<void()> func) {
      entry_callback_map[typeid(T).name()] = func;
    }

    template <typename T>
    void register_update_callback(std::function<void()> func) {
      update_callback_map[typeid(T).name()] = func;
    }

    template <typename T>
    void register_transition_callback(
        std::function<void(hfsm::Machine<Context>::Control &)> func) {
      transition_callback_map[typeid(T).name()] = func;
    }

    template <typename T>
    void register_leave_callback(std::function<void()> func) {
      leave_callback_map[typeid(T).name()] = func;
    }

    template <typename T> void callback_on_entry() const {
      if (entry_callback_map.find(typeid(T).name()) !=
          entry_callback_map.end()) {
        entry_callback_map.at(typeid(T).name())();
      } else {
        // std::cout << "entry callback for " << typeid(T).name() << " not
        // found" << std::endl;
      }
    }

    template <typename T> void callback_on_update() const {
      if (update_callback_map.find(typeid(T).name()) !=
          update_callback_map.end()) {
        update_callback_map.at(typeid(T).name())();
      } else {
        // std::cout << "update callback for " << typeid(T).name() << " not
        // found" << std::endl;
      }
    }

    template <typename T>
    void callback_on_transition(hfsm::Machine<Context>::Control &control) {
      if (transition_callback_map.find(typeid(T).name()) !=
          transition_callback_map.end()) {
        transition_callback_map.at(typeid(T).name())(control);
      } else {
        // std::cout << "transition callback for " << typeid(T).name() << " not
        // found" << std::endl;
      }
    }

    template <typename T> void callback_on_leave() const {
      if (leave_callback_map.find(typeid(T).name()) !=
          leave_callback_map.end()) {
        leave_callback_map.at(typeid(T).name())();
      } else {
        // std::cout << "leave callback for " << typeid(T).name() << " not
        // found" << std::endl;
      }
    }
  };

  DEFINE_STATE_CLASS(StateA, hfsm::Machine<Context>::Base);
  DEFINE_STATE_CLASS(StateB, hfsm::Machine<Context>::Base);
  DEFINE_STATE_CLASS(StateC, hfsm::Machine<Context>::Base);

  /*** task state ***/
  // entry callback
  void onEntryStateA() {}
  void onEntryStateB() {}
  void onEntryStateC() {}
  // update callback
  void onUpdateStateA() {}
  void onUpdateStateB() {}
  void onUpdateStateC() {}

  // transition callback
  void onTransitionStateA(hfsm::Machine<Context>::Control &control) {control.changeTo<StateB>();}
  void onTransitionStateB(hfsm::Machine<Context>::Control &control) {control.changeTo<StateC>();}
  void onTransitionStateC(hfsm::Machine<Context>::Control &control) {control.changeTo<StateA>();}

  // leave callback
  void onLeaveStateA() {}
  void onLeaveStateB() {}
  void onLeaveStateC() {}

  using COMPOSITE =
      hfsm::Machine<Context>::CompositePeers<StateA, StateB, StateC>;

  Context context_;
  std::unique_ptr<hfsm::Machine<Context>::PeerRoot<COMPOSITE>> machine_;

  void setupCallbackFunctions() {
    context_.register_entry_callback<StateA>(
      std::bind(&ApaStateMachine::onEntryStateA, this));
    context_.register_entry_callback<StateB>(
      std::bind(&ApaStateMachine::onEntryStateB, this));
    context_.register_entry_callback<StateC>(
      std::bind(&ApaStateMachine::onEntryStateC, this));
    
    context_.register_update_callback<StateA>(
      std::bind(&ApaStateMachine::onUpdateStateA, this));
    context_.register_update_callback<StateB>(
      std::bind(&ApaStateMachine::onUpdateStateB, this));
    context_.register_update_callback<StateC>(
      std::bind(&ApaStateMachine::onUpdateStateC, this));

    context_.register_transition_callback<StateA>(std::bind(
      &ApaStateMachine::onTransitionStateA, this, std::placeholders::_1));
    context_.register_transition_callback<StateB>(std::bind(
      &ApaStateMachine::onTransitionStateB, this, std::placeholders::_1));
    context_.register_transition_callback<StateC>(std::bind(
      &ApaStateMachine::onTransitionStateC, this, std::placeholders::_1));
 
    context_.register_leave_callback<StateA>(
      std::bind(&ApaStateMachine::onLeaveStateA, this));
    context_.register_leave_callback<StateB>(
      std::bind(&ApaStateMachine::onLeaveStateB, this));
    context_.register_leave_callback<StateC>(
      std::bind(&ApaStateMachine::onLeaveStateC, this));
	}
};

int main(int argc, char **argv) {
  ApaStateMachine apa_state_machine;
  for (int i = 0; i < 3; i++) {
    std::cout << "update " << i << std::endl;
    apa_state_machine.update();
  }
  return 0;
}