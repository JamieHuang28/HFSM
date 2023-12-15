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
    if (context.basic_parkin_behavior->isFinish() && context.wlc_behavior->isFinish()) {
      STDLOG(finish_procedure include report failure reason);
      control.changeTo<Wait>();
    }
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