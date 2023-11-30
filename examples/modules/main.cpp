#include <map>
#include <string>

#define HFSM_ENABLE_LOG_INTERFACE
#include "machine_logger.h"
#include "modules.h"

//------------------------------------------------------------------------------

// data shared between FSM states and outside code
struct Context {
	unsigned cycleCount;
  Car car;
};

// convenience typedef
using M = hfsm::Machine<Context>;

////////////////////////////////////////////////////////////////////////////////

// forward declared for Red::transition()
struct Off;

// top-level region in the hierarchy
struct On
	: M::Base // necessary boilerplate!
{
	// called on state entry
	void enter(Context& context) {
		context.cycleCount = 0;
	}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	// forward declared for Red::transition()
	struct YellowDownwards;

	// sub-states
	struct Red
		: M::Base
	{
		void enter(Context& context) {
			++context.cycleCount;
		}

    void update(Context& context) {
      context.car.stop();
    }

		// state can initiate transitions to _any_ other state
		void transition(Control& control, Context& context) {
			// multiple transitions can be initiated, can be useful in a hierarchy
			if (context.cycleCount > 3)
				control.changeTo<Off>();
			else
				control.changeTo<YellowDownwards>();
		}
	};

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	// forward declared for transition()
	struct Green;

	struct YellowDownwards
		: M::Base
	{
		void enter(Context&) {
		}

		void transition(Control& control, Context&) {
			control.changeTo<Green>();
		}
	};

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	struct YellowUpwards
		: M::Base
	{
		void enter(Context&) {
		}

		void transition(Control& control, Context&) {
			control.changeTo<Red>();
		}
	};

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	struct Green
		: M::Base
	{
		void enter(Context& context) {
		}

    void update(Context& context) {
      context.car.drive();
    }

		void transition(Control& control, Context&) {
			control.changeTo<YellowUpwards>();
		}
	};

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
};

//------------------------------------------------------------------------------

// another top-level state
struct Off
	: M::Base
{
	void enter(Context&) {
	}
};

////////////////////////////////////////////////////////////////////////////////

int
main() {
	// shared data storage instance
	Context context;

	// state machine structure
	M::PeerRoot<
		// sub-machine ..
		M::Composite<On,
			// .. with 4 sub-states
			On::Red,
			On::YellowDownwards,
			On::YellowUpwards,
			On::Green
		>,
		Off
	> machine(context, HfsmCoutLogger::Instance());

	while (machine.isActive<Off>() == false)
		machine.update();

	return 0;
}