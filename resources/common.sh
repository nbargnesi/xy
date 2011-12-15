#!/usr/bin/env bash
function start_display() {
    if [ -z $(which Xvfb) ]; then
        echo "no Xvfb in PATH" >&2
        return
    fi
    Xvfb :1 >/dev/null 2>&1 &
    export XVFB_PID=$(jobs -p | head -n 1)
    if [ ! -z "${DISPLAY}" ]; then
        export OLD_DISPLAY="${DISPLAY}"
    fi
    export DISPLAY=":1"
}

function stop_display() {
    if [ -z "$XVFB_PID" ]; then
        echo "XVFB_PID not set" >&2
        return
    fi
    kill $XVFB_PID
    unset XVFB_PID
    unset DISPLAY
    if [ ! -z "${OLD_DISPLAY}" ]; then
        export DISPLAY="${OLD_DISPLAY}"
        unset OLD_DISPLAY
    fi
}

