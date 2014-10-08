#include <xcb/xcb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

xcb_atom_t _NET_ACTIVE_WINDOW;
xcb_atom_t _NET_CLIENT_LIST;
xcb_atom_t _NET_CURRENT_DESKTOP;
xcb_atom_t _NET_DESKTOP;
xcb_atom_t _NET_WM_ICON_GEOMETRY;
xcb_atom_t _NET_WM_ICON_POSITION;
xcb_atom_t _NET_WM_STATE;
xcb_atom_t _NET_WM_STATE_SKIP_TASKBAR;
xcb_atom_t _NET_WM_WINDOW_TYPE;
xcb_atom_t _NET_WM_WINDOW_TYPE_DESKTOP;
xcb_atom_t _NET_WM_WINDOW_TYPE_DOCK;
xcb_atom_t CARDINAL;

const xcb_window_t ROOT_WINDOW = { 0 };

xcb_atom_t get_atom(xcb_connection_t *conn, const char *name) {
    xcb_intern_atom_cookie_t cookie = xcb_intern_atom(conn, 0, strlen(name), name);
    xcb_intern_atom_reply_t *reply = xcb_intern_atom_reply(conn, cookie, NULL);
    return reply->atom;
}

void intern_atoms(xcb_connection_t *conn) {
    _NET_ACTIVE_WINDOW = get_atom(conn, "_NET_ACTIVE_WINDOW");
    _NET_CLIENT_LIST = get_atom(conn, "_NET_CLIENT_LIST");
    _NET_CURRENT_DESKTOP = get_atom(conn, "_NET_CURRENT_DESKTOP");
    _NET_DESKTOP = get_atom(conn, "_NET_DESKTOP");
    _NET_WM_ICON_GEOMETRY = get_atom(conn, "_NET_WM_ICON_GEOMETRY");
    _NET_WM_ICON_POSITION = get_atom(conn, "_NET_WM_ICON_POSITION");
    _NET_WM_STATE = get_atom(conn, "_NET_WM_STATE");
    _NET_WM_STATE_SKIP_TASKBAR = get_atom(conn, "_NET_WM_STATE_SKIP_TASKBAR");
    _NET_WM_WINDOW_TYPE = get_atom(conn, "_NET_WM_WINDOW_TYPE");
    _NET_WM_WINDOW_TYPE_DESKTOP = get_atom(conn, "_NET_WM_WINDOW_TYPE_DESKTOP");
    _NET_WM_WINDOW_TYPE_DOCK = get_atom(conn, "_NET_WM_WINDOW_TYPE_DOCK");
    CARDINAL = get_atom(conn, "CARDINAL");
}

int get_current_desktop(xcb_connection_t *conn) {
    xcb_get_property_cookie_t cookie = xcb_get_property(
        conn,
        0,
        ROOT_WINDOW,
        _NET_CURRENT_DESKTOP,
        CARDINAL,
        0,
        1);

    xcb_get_property_reply_t *reply = xcb_get_property_reply(conn, cookie, NULL);
    
    void *value = xcb_get_property_value(reply);

    int current_desktop = *(int *)value;
    return current_desktop;
}

void help(void) {
    fprintf(stderr, "Usage: irssi-focus <window number>\n");
}

int main(int argc, char **argv) {
    if (argc < 2) {
        help();
        return 1;
    }

    errno = 0;
    int window_num = strtol(argv[1], NULL, 10);
    if (errno) {
        perror("strtol");
        help();
        return 1;
    }

    xcb_connection_t *connection = xcb_connect(NULL, NULL);
    printf("%d\n", get_current_desktop(connection));
    xcb_disconnect(connection);
}
