#ifndef STATE_H_
#define STATE_H_

enum STATE_ENUM {
    INITIALIZING_MODULES,
    TERMINATING_MODULES
};
typedef enum STATE_ENUM State;

/*
 * Transitions to a state.
 */
void transition(const State);

#endif
