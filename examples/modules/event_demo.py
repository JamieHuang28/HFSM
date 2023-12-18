import threading

class ParkingStateMachine:
    def __init__(self, event_bus):
        self.state = 'INITIAL'
        self.event_bus = event_bus
        self.obstacle_timer = None
        self.subscribe_to_events()

    def subscribe_to_events(self):
        self.event_bus.subscribe('TrajectoryTrackingStarted', self.on_trajectory_tracking_started)
        self.event_bus.subscribe('TrajectoryTrackingCompleted', self.on_trajectory_tracking_completed)
        self.event_bus.subscribe('ObstacleDetected', self.on_obstacle_detected)
        self.event_bus.subscribe('ObstacleCleared', self.on_obstacle_cleared)

    def on_trajectory_tracking_started(self, event):
        print("Trajectory tracking started.")
        self.state = 'TRAJECTORY_TRACKING'

    def on_trajectory_tracking_completed(self, event):
        if self.state == 'TRAJECTORY_TRACKING':
            print("Trajectory tracking completed.")
            self.transition_to_parked_successfully()

    def on_obstacle_detected(self, event):
        if self.state == 'TRAJECTORY_TRACKING':
            print("Obstacle detected. Starting timer...")
            # Cancel existing timer if any
            if self.obstacle_timer is not None:
                self.obstacle_timer.cancel()
            # Start a 3-second timer
            self.obstacle_timer = threading.Timer(3.0, self.transition_to_parked_due_to_obstacle)
            self.obstacle_timer.start()

    def on_obstacle_cleared(self, event):
        if self.state == 'TRAJECTORY_TRACKING':
            print("Obstacle cleared before timer elapsed.")
            # Stop the 3-second timer if running
            if self.obstacle_timer is not None:
                self.obstacle_timer.cancel()
                self.obstacle_timer = None

    def transition_to_parked_due_to_obstacle(self):
        print("Timer expired, vehicle has been stopped for 3 seconds due to an obstacle.")
        self.transition_to_parked_successfully()

    def transition_to_parked_successfully(self):
        self.state = 'PARKED_SUCCESSFULLY'
        print("Transitioned to PARKED_SUCCESSFULLY state.")
        # Perform actions necessary for completing parking...
        self.event_bus.publish('ParkedSuccessfully', {})


# Event Bus (Simplified version for example purpose)
class EventBus:
    def __init__(self):
        self.subscribers = {}

    def subscribe(self, event_type, handler):
        if event_type not in self.subscribers:
            self.subscribers[event_type] = []
        self.subscribers[event_type].append(handler)

    def publish(self, event_type, event):
        if event_type in self.subscribers:
            for handler in self.subscribers[event_type]:
                # In a real-world scenario, you may want to handle exceptions and possibly run these in separate threads.
                handler(event)


# Example usage of the state machine with an event bus
event_bus = EventBus()
parking_state_machine = ParkingStateMachine(event_bus)

# Simulate events being triggered by different parts of the system
event_bus.publish('TrajectoryTrackingStarted', {})
# ... some time later ...
event_bus.publish('ObstacleDetected', {})
# ... within 3 seconds, if obstacle is cleared ...
event_bus.publish('ObstacleCleared', {})
# ... or after 3 seconds if the obstacle was not cleared ...
# The `transition_to_parked_due_to_obstacle` function will be called automatically by the timer.
# ... once trajectory tracking completes successfull