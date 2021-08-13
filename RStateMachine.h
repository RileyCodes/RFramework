#pragma once
#include <variant>
//Credits to: https://sii.pl/blog/implementing-a-state-machine-in-c17/
using std::variant;


template <typename... States>
class RStateMachine
{
public:
    template <typename State>
    void transitionTo()
    {
        currentState = &std::get<State>(states);
    }

    template <typename Event>
    void handle(const Event& event)
    {
        auto passEventToState = [this, &event](auto statePtr) {
            statePtr->handle(event).execute(*this);
        };
        std::visit(passEventToState, currentState);
    }

private:
    std::tuple<States...> states;
    std::variant<States*...> currentState{ &std::get<0>(states) };
};

template <typename State>
struct TransitionTo
{
    template <typename Machine>
    void execute(Machine& machine)
    {
        machine.template transitionTo<State>();
    }
};

struct Nothing
{
    template <typename Machine>
    void execute(Machine&)
    {
    }
};