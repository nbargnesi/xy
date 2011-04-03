#ifndef STATE_H_
#define STATE_H_

enum STATE_ENUM {
    STARTING_UP,
    STARTED,
    SHUTTING_DOWN
};
typedef enum STATE_ENUM State;

/*
 * Transitions to a state.
 */
void transition(const State);

#endif
