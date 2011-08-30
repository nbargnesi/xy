.. _apiref:

*************
API Reference
*************

.. highlight:: c

util.h
======

A collection of utility functions meant to stand alone from other xy modules.

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
