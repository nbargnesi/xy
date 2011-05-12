/*
 * This file is part of xy.
 *
 * XY is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * XY is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with xy.  If not, see <http://www.gnu.org/licenses/>.
 */

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
#define DEFAULT_BROADCAST_PORT "47002"

/*
 * Define: KS_SEPARATOR
 *
 * Defines how key shortcuts are to specified and parsed.
 */
#define KS_SEPARATOR " "

/*
 * Define: MOD1_KS
 */
#define MOD1_KS "Mod1"

/*
 * Define: MOD2_KS
 */
#define MOD2_KS "Mod2"

/*
 * Define: MOD3_KS
 */
#define MOD3_KS "Mod3"

/*
 * Define: MOD4_KS
 */
#define MOD4_KS "Mod4"

/*
 * Define: SHIFT_KS
 */
#define SHIFT_KS "Shift"

/*
 * Define: CTRL_KS
 */
#define CTRL_KS "Control"

/*
 * Define: CFG_KS_MENU
 *
 * <XY_CONFIG> entry to control the key shortcut used to launch the menu.
 * (code)
 * key_shortcut_menu = <mask> <keyname>
 * (end)
 *
 * Examples masks: Mod4 Shift Control
 *
 * Defaults to <DEFAULT_KS_MENU>.
 */
#define CFG_KS_MENU "key_shortcut_menu"

/*
 * Define: DEFAULT_KS_MENU
 *
 * <XY_CONFIG> entry default value of <CFG_KS_MENU>.
 *
 * Default:
 * (code)
 * Mod4 p
 * (end)
 */
#define DEFAULT_KS_MENU "Mod4"KS_SEPARATOR"p"

/*
 * Define: CFG_KS_TERMINAL
 *
 * <XY_CONFIG> entry to control the key shortcut used to launch a terminal.
 * (code)
 * key_shortcut_terminal = <mask> <keyname>
 * (end)
 *
 * Examples masks: Mod4 Shift Control
 *
 * Defaults to <DEFAULT_KS_TERMINAL>.
 */
#define CFG_KS_TERMINAL "key_shortcut_terminal"

/*
 * Define: DEFAULT_KS_TERMINAL
 *
 * <XY_CONFIG> entry default value of <CFG_KS_TERMINAL>.
 *
 * Default:
 * (code)
 * Mod4 Return
 * (end)
 */
#define DEFAULT_KS_TERMINAL "Mod4"KS_SEPARATOR"Return"

/*
 * Define: CFG_KS_QUIT
 *
 * <XY_CONFIG> entry to control the key shortcut used to quit.
 * (code)
 * key_shortcut_terminal = <mask> <keyname>
 * (end)
 *
 * Examples masks: Mod4 Shift Control
 *
 * Defaults to <DEFAULT_KS_QUIT>.
 */
#define CFG_KS_QUIT "key_shortcut_quit"

/*
r* Define: DEFAULT_KS_QUIT
 *
 * <XY_CONFIG> entry default value of <CFG_KS_QUIT>.
 *
 * Default:
 * (code)
 * Mod4 q
 * (end)
 */
#define DEFAULT_KS_QUIT "Mod4"KS_SEPARATOR"q"

#define STARTUP_MSG "starting up"

#define IPC_STARTUP_MSG "IPC started"

#define BROADCAST_STARTUP_MSG "broadcast started"

#define STARTED_MSG "started"

#define SHUTTING_DOWN_MSG "shutting down"

#define SHUTDOWN_MSG "shutdown"

#define PONG_MSG "PONG"

#define INIT_LOGGING_FAILURE "failed to start logging"

#define INIT_IPC_FAILURE "failed to start IPC"

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

