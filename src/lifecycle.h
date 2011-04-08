#ifndef STARTUP_H_
#define STARTUP_H_

/*
 * Function: module_init
 *
 * Calls <module_name>_init for each module.
 */
void module_init();

/*
 * Function: directory_init
 */
void directory_init();

/*
 * Function: setup
 */
void setup();

/*
 * Function: shutdown
 */
void shutdown();

/*
 * Function: module_terminate
 *
 * Calls <module_name>_terminate for each module.
 */
void module_terminate();

#endif

