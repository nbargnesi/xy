.. _apiref:

*************
API Reference
*************

.. highlight:: c

configuration.h
===============

The configuration module reponsible for managing the configuration of xy.
Any change to xy's runtime configuration (RC) file will cause the configuration
to be reloaded (see :func:`config_reinit`).

.. type:: CONFIG

   The type made available after a call to :func:`config_init`. It contains all
   the necessary configuration options xy uses while running.

.. function:: CONFIG * config_init()

   Initializes xy's configuration. Returns the configuration or NULL on failure.
   This function makes :type:`CONFIG` available for use.

.. function:: void config_terminate()

   The configuration module's shutdown hook.

.. function:: void write_default_config()

   Writes the default xy configuration to the xy runtime configuration path.

.. function:: void config_reinit()   

   Reinitializes the configuration from the xy runtime configuration path.

util.h
======

A collection of utility functions meant to stand alone from other xy modules.

.. function:: char * rc_path()

   Returns the path of xy's runtime configuration. The returned string should be
   freed by the caller.

.. function:: bool streq(const char *, const char *)

    Returns ``true`` if the strings are equal or null, ``false`` otherwise.

.. function:: char * trim(char *)

    Removes leading and trailing whitespace from the string and returns a
    pointer starting from the first non-whitespace character. The returned
    string is contained in the same memory as the argument.

    Example::

        char *conststr = " test string ";
        char *dup = strdup(conststr);
        char *trimmed = trim(dup); // trimmed: "test string"
        free(dup);

.. function:: void dump_stack(uint num_frames)

    Dumps *num_frames* stack frames to stderr.

.. function:: void parse_command(char *cmd, char **argv)

    Tokenizes *cmd* by spaces into tokens placed in *argv*.

    Example::

        char *command = "some string here";
        char *dup = strdup(command);
        char *argv[3];
        parse_command(dup, argv);
        // argv[0]: "some"
        // argv[1]: "string"
        // argv[2]: "here"
        free(dup);

.. function:: void exec(const char * cmd)

    Executes the supplied command by calling *execvp*. The command will be
    parsed before the call is made (see `parse_command`).

inotify.h
=========

The inotify module. XY uses the inotify module to react to changes occurring on
the filesystem.

types
-----

.. type:: in_fd

   The inotify file descriptor made available after a call to
   :func:`xy_inotify_init`. This file descriptor is suitable for system calls
   like *select* and *epoll*.

functions
---------

.. function:: void xy_inotify_init()

   Initializes xy's inotify module. This function makes :type:`in_fd`
   available for use, which may be -1 on failure.

.. function:: void xy_inotify_reinit()

   Reinitializes the inotify module. This function makes a new :type:`in_fd`
   available for use.

.. function:: void xy_inotify_terminate()

   The inotify module's shutdown hook.

.. function:: void xy_inotify_read()

   Drains the inotify event queue of all events.
