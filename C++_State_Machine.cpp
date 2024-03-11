#include <iostream>
#include <unordered_map>
#include <chrono>
#include <thread>
#include <functional>
#include <string>
#include <vector>

// Define an action function type and a structure for state definitions without explicit transition messages.
using ActionFunction = std::function<void()>;
struct StateDefinition {
	std::string baseMessage; // Base action message without transition info.
	int nextStateId;
};

// Global maps to hold state actions and transitions.
std::unordered_map<int, ActionFunction> stateActions;
std::unordered_map<int, int> transitions;

// Utility function to register a state, its base action message, and subsequent transition.
static void registerState(int stateId, const StateDefinition& definition) {
	stateActions[stateId] = [stateId, definition]() {
		std::cout << "Current State: State " << stateId << "\n";
		std::string completeMessage = definition.baseMessage;
		if (definition.nextStateId != -1) { // Check if it's not the termination state.
			completeMessage += " Transitioning to state " + std::to_string(definition.nextStateId) + ".";
		}
		else {
			completeMessage += " Preparing to exit.";
		}
		std::cout << completeMessage << "\n";
		};
	transitions[stateId] = definition.nextStateId;
}

// Initialize the state machine with states, actions, and dynamic transition messages.
void initializeStateMachine(const std::vector<StateDefinition>& stateDefinitions) {
	for (size_t i = 0; i < stateDefinitions.size(); ++i) {
		registerState(i, stateDefinitions[i]);
	}
}

int main() {
	// Define base state messages without explicit transition info.
	std::vector<StateDefinition> stateDefinitions = {
		{"Starting application", 1},
		{"Switching to editing mode", 2},
		{"Saving work", 3},
		{"Continuing editing", 2}, // Loop back to a previous state as an example.
		{"Exiting application", -1} // Transition to termination state.
	};

	initializeStateMachine(stateDefinitions);

	int currentState = 0;

	while (currentState != -1) {
		std::this_thread::sleep_for(std::chrono::seconds(1));

		auto actionIt = stateActions.find(currentState);
		if (actionIt != stateActions.end()) {
			actionIt->second();
		}
		else {
			std::cerr << "Invalid state encountered: " << currentState << ". Exiting loop.\n";
			break;
		}

		currentState = transitions[currentState];
	}

	return 0;
}
