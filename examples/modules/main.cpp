#include <map>
#include <string>

#define HFSM_ENABLE_LOG_INTERFACE
#include "machine_logger.h"
#include "context.h"
#include "utils.h"
#include "wlc_behavior.h"
#include "wlc_world_model_adaptor.h"

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
    STDLOG(create ParkingSlot);
    STDLOG(openspace_state_machine_-> createMachineParkIn());
    STDLOG(get init and target of APA);
    STDLOG(set planning_core);
    STDLOG(set turn signal by judging the target side);
    STDLOG(openspace_state_machine_->changeToStandby());
    STDLOG(reset advanced_abandon);
  }

  void transition(Control& control, Context& context) {
    STDLOG(update_parking_slot_info and wheelstop collision_check);
    if (context.is_finish || context.isWaitREquested()) {
      STDLOG(finish_procedure include report failure reason);
      control.changeTo<Wait>();
    }
  }
};

struct Wait : M::Base {
  void enter(Context&) {
  }

  void transition(Control& control, Context& context) {
    if (context.isParkinRequested())
      control.changeTo<ParkIn>();
    else if (context.isOffRequested())
      control.changeTo<Off>();
  }
};

////////////////////////////////////////////////////////////////////////////////

void palyWlcBehavior() {
  Context context;
  std::shared_ptr<WorldModel> world_model = std::make_shared<WorldModel>();
  std::shared_ptr<WlcWorldModelAdaptor> wlc_wm_adaptor = std::make_shared<WlcWorldModelAdaptor>();
  wlc_wm_adaptor->setWorldModel(world_model);
  std::shared_ptr<WlcBehavior> wlc_behavior = std::make_shared<WlcBehavior>(wlc_wm_adaptor);
  wlc_behavior->disableFinishFlag(context);
}

int test(int arc, char** argv) {
  std::cout << "test" << std::endl;

  // // implement the promt above with a class
  // Prompter<int>::prompt("Enter int");
  // std::cout << Prompter<int>::value() << std::endl;

  // std::cout << prompt<YN>("Enter a y or n") << std::endl;
  palyWlcBehavior();
  return 0;
}

#define TEST 0
int main(int arc, char** argv) {
	#if TEST == 0
	// shared data storage instance
	Context context;

	// state machine structure
	M::PeerRoot<
		Wait,
		ParkIn,
    Off
	> machine(context, HfsmCoutLogger::Instance());

  std::shared_ptr<WorldModel> world_model = std::make_shared<WorldModel>();
  std::shared_ptr<WlcWorldModelAdaptor> wlc_wm_adaptor = std::make_shared<WlcWorldModelAdaptor>();
  wlc_wm_adaptor->setWorldModel(world_model);
  std::shared_ptr<WlcBehavior> wlc_behavior = std::make_shared<WlcBehavior>(wlc_wm_adaptor);

	while (machine.isActive<Off>() == false) {
    world_model->setRequest(context);
    
    if (machine.isActive<ParkIn>()) {
      context.is_finish = prompt<YN>("is_finish?");
      wlc_behavior->disableFinishFlag(context);
    }
    machine.update();
  }

	return 0;

	#else
	return test(arc, argv);
	#endif
}