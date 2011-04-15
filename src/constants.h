#ifndef CONSTANTS_H_
#define CONSTANTS_H_

/*
 * Define: CFG_SKIP_WINDOW_MGR_CHECK
 *
 * <XY_CONFIG> entry to skip the check for a running window manager.
 * (code)
 * skip_window_manager_check = ( true | false )
 * (end)
 * Defaults to <DEFAULT_SKIP_WINDOW_MGR_CHECK>.
 */
#define CFG_SKIP_WINDOW_MGR_CHECK "skip_window_manager_check"
#define SKIP_WINDOW_MGR_CHECK_MSG "skipping window manager check"

/*
 * Define: DEFAULT_SKIP_WINDOW_MGR_CHECK
 *
 * <XY_CONFIG> entry default value for <CFG_SKIP_WINDOW_MGR_CHECK>.
 *
 * Default:
 * (code)
 * false
 * (end)
 */
#define DEFAULT_SKIP_WINDOW_MGR_CHECK "false"

/*
 * Define: CFG_WINDOW_MGR_NAME
 *
 * <XY_CONFIG> entry to specify the window manager's name.
 * (code)
 * window_manager_name = <text>
 * (end)
 * Defaults to <DEFAULT_WINDOW_MGR_NAME>.
 */
#define CFG_WINDOW_MGR_NAME "window_manager_name"
#define CHANGE_WINDOW_MGR_NAME_MSG "changing window manager name"

/*
 * Define: DEFAULT_WINDOW_MGR_NAME
 *
 * <XY_CONFIG> entry default value for <CFG_WINDOW_MGR_NAME>.
 *
 * Default:
 * (code)
 * xy
 * (end)
 */
#define DEFAULT_WINDOW_MGR_NAME "xy"

/*
 * Define: CFG_BROADCAST_GROUP
 *
 * <XY_CONFIG> entry to control the multicast group address.
 * (code)
 * broadcast_group = <dotted_decimal_address>
 * (end)
 * Defaults to <DEFAULT_BROADCAST_GROUP>.
 */
#define CFG_BROADCAST_GROUP "broadcast_group"

/*
 * Define: DEFAULT_BROADCAST_GROUP
 *
 * <XY_CONFIG> entry default value of <CFG_BROADCAST_GROUP>.
 *
 * Default:
 * (code)
 * 224.0.0.3
 * (end)
 */
#define DEFAULT_BROADCAST_GROUP "224.0.0.3"

/*
 * Define: CFG_BROADCAST_PORT
 *
 * <XY_CONFIG> entry to control the multicast port.
 * (code)
 * broadcast_port = <port>
 * (end)
 * Defaults to <DEFAULT_BROADCAST_PORT>.
 */
#define CFG_BROADCAST_PORT "broadcast_port"

/*
 * Define: DEFAULT_BROADCAST_PORT
 *
 * <XY_CONFIG> entry default value of <CFG_BROADCAST_PORT>.
 *
 * Default:
 * (code)
 * 47002
 * (end)
 */
#define DEFAULT_BROADCAST_PORT 47002

#define STARTUP_MSG "starting up"

#define STARTED_MSG "started"

#define SHUTTING_DOWN_MSG "shutting down"

#define SHUTDOWN_MSG "shutdown"

#define INIT_LOGGING_FAILURE "failed to start logging"

#define INIT_BROADCAST_FAILURE "failed to start broadcast"

#define GET_CLIENTS_FAILURE "failed to get clients"

#define WINDOW_MGR_RUNNING "a window manager is already running"

#define READING_CONFIGURATION_MSG "reading configuration"

#define USING_DEFAULT_CONFIGURATION_MSG "using default configuration"

#define FAILED_TO_GET_CLIENTS "failed to get clients"

#define NO_CONFIGURATION "no configuration"

#define CREATING_XY_DIR "creating xy directory"

#define DEFAULT_CFG_FILE_HDR "# xy configuration file"

#endif

