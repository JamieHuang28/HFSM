#include <map>
#include <string>

#define HFSM_ENABLE_LOG_INTERFACE
#include "machine_logger.h"
#include "context.h"
#include "utils.h"
#include "world_model.h"

//------------------------------------------------------------------------------

// convenience typedef
using M = hfsm::Machine<Context>;

////////////////////////////////////////////////////////////////////////////////

// forward declared for Red::transition()
struct Wait;
struct ParkIn;
struct Off;

struct Off
	: M::Base
{
	void enter(Context&) {
	}
};

struct ParkIn : M::Base {
  void enter(Context&) {
    STDLOGPROCESS(create ParkingSlot);
    STDLOGPROCESS(openspace_state_machine_-> createMachineParkIn());
    STDLOGPROCESS(get init and target of APA);
    STDLOGPROCESS(set planning_core);
    STDLOGPROCESS(set turn signal by judging the target side);
    STDLOGPROCESS(openspace_state_machine_->changeToStandby());
    STDLOGPROCESS(reset advanced_abandon);
  }

  void transition(Control& control, Context& context) {
    STDLOGPROCESS(update_parking_slot_info and wheelstop collision_check);
    STDLOGPROCESS("if request to wait, change to wait");
    STDLOGPROCESS("----the conditions below are additional----");
    STDLOGPROCESS("if need_re_parking, set failure reason");
    STDLOGPROCESS("if zigzag number over limit, set failure reason");
    STDLOGPROCESS("--------");
    if (context.basic_parkin_behavior->isFinish() && context.wlc_behavior->isFinish()) {
      STDLOGPROCESS(finish_procedure include report failure reason);
      control.changeTo<Wait>();
    }
    STDLOGPROCESS("strategy_hit_wheelstop: finish or replan");
    STDLOGPROCESS("strategy_wheelstop_in_parallel_slot: whether abandon or not");
    STDLOGPROCESS("strategy_blocked_and_reached: whether wait for WLC or not");
    STDLOGPROCESS("strategy_tiny_perpendicular_and_oblique_slot: whether abandon or not");
    STDLOGPROCESS("update parking slot");
    STDLOGPROCESS("dynamic plan");
    STDLOGPROCESS("replan_strategy_when_openspace_fallback");
    STDLOGPROCESS("eplan_strategy_when_block_by_pedestrian");
    STDLOGPROCESS("calculate_behaviors_relating_block");
    STDLOGPROCESS("replan_or_finish_strategy_when_blocked");
    STDLOGPROCESS("strategy_traj_following_finish");
    STDLOGPROCESS("active_replan_strategy_when_first_reverse_gear_switch");
    STDLOGPROCESS("strategy_assumed_blocked");
    STDLOGPROCESS("strategy_during_pause");
  }
};

struct Wait : M::Base {
  void enter(Context&) {
  }

  void transition(Control& control, Context& context) {
    if (context.world_model->request() == RequestType::PARKIN)
      control.changeTo<ParkIn>();
    else if (context.world_model->request() == RequestType::OFF)
      control.changeTo<Off>();
  }
};

////////////////////////////////////////////////////////////////////////////////

void palyWlcBehavior() {
  std::shared_ptr<WorldModel> world_model = std::make_shared<WorldModel>();
  Context context(world_model);
  std::shared_ptr<WlcWorldModelAdaptor> wlc_wm_adaptor = std::make_shared<WlcWorldModelAdaptor>();
  wlc_wm_adaptor->setWorldModel(world_model);
  std::shared_ptr<WlcBehavior> wlc_behavior = std::make_shared<WlcBehavior>(wlc_wm_adaptor);
}

void playUtils() {
  // implement the promt above with a class
  Prompter<int>::prompt("Enter int");
  std::cout << Prompter<int>::value() << std::endl;

  std::cout << prompt<YN>("Enter a y or n") << std::endl;
}

int test(int arc, char** argv) {
  std::cout << "test" << std::endl;
  palyWlcBehavior();
  return 0;
}

#define TEST 0
int main(int arc, char** argv) {
	#if TEST == 0

  std::shared_ptr<WorldModel> world_model = std::make_shared<WorldModel>();
	// shared data storage instance
	Context context(world_model);

	// state machine structure
	M::PeerRoot<
		Wait,
		ParkIn,
    Off
	> machine(context, HfsmCoutLogger::Instance());

	while (machine.isActive<Off>() == false) {
    world_model->setRequest(prompt<int>("please input request, 0 for none, 1 for ParkIn, 2 for Off, 3 for Wait")); // stateless module
    machine.update(); // stateful module are updated in the state machine
  }

	return 0;

	#else
	return test(arc, argv);
	#endif
}