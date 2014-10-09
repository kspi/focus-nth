#include <assert.h>
#include <errno.h>
#include <limits.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <xcb/xcb.h>

#include "atoms.h"

int get_current_desktop(xcb_connection_t *conn, xcb_window_t root_window) {
    xcb_get_property_cookie_t cookie = xcb_get_property(
        conn, 0,
        root_window,
        _NET_CURRENT_DESKTOP,
        XCB_ATOM_CARDINAL,
        0, 1);

    xcb_get_property_reply_t *reply = xcb_get_property_reply(conn, cookie, NULL);
    assert(reply);
    assert(xcb_get_property_value_length(reply) > 0);

    uint32_t *value = (uint32_t *)xcb_get_property_value(reply);
    int current_desktop = *value;
    free(reply);

    return current_desktop;
}

xcb_screen_t *screen_of_display(xcb_connection_t *c, int screen) {
    xcb_screen_iterator_t iter;
    iter = xcb_setup_roots_iterator (xcb_get_setup (c));
    for (; iter.rem; --screen, xcb_screen_next (&iter)) {
        if (screen == 0) {
            return iter.data;
        }
    }
    return NULL;
}

int window_get_desktop(xcb_connection_t *c, xcb_window_t window) {
    xcb_get_property_cookie_t cookie = xcb_get_property(
        c, 0,
        window, _NET_WM_DESKTOP, XCB_ATOM_CARDINAL,
        0, 1);

    xcb_get_property_reply_t *reply = xcb_get_property_reply(c, cookie, NULL);
    if (!reply) return -1;
    assert(xcb_get_property_value_length(reply) > 0);

    uint32_t *value = (uint32_t *)xcb_get_property_value(reply);
    int desktop = *value;
    free(reply);

    return desktop;
}

int window_is_normal(xcb_connection_t *c, xcb_window_t window) {
    int is_normal = 1;
    {
        xcb_get_property_cookie_t cookie = xcb_get_property(
            c, 0,
            window, _NET_WM_STATE, XCB_ATOM_ATOM,
            0, -1);

        xcb_get_property_reply_t *reply = xcb_get_property_reply(c, cookie, NULL);
        assert(reply);

        int len = xcb_get_property_value_length(reply) / sizeof(xcb_atom_t);
        xcb_atom_t *state = (xcb_atom_t *)xcb_get_property_value(reply);
        for (int i = 0; is_normal && i < len; ++i) {
            if (state[i] == _NET_WM_STATE_SKIP_TASKBAR) is_normal = 0;
        }
        free(reply);
        if (!is_normal) return 0;
    }

    {
        xcb_get_property_cookie_t cookie = xcb_get_property(
            c, 0,
            window, _NET_WM_STATE, XCB_ATOM_ATOM,
            0, -1);

        xcb_get_property_reply_t *reply = xcb_get_property_reply(c, cookie, NULL);
        assert(reply);

        int len = xcb_get_property_value_length(reply) / sizeof(xcb_atom_t);
        xcb_atom_t *type = (xcb_atom_t *)xcb_get_property_value(reply);
        for (int i = 0; is_normal && i < len; ++i) {
            if (type[i] == _NET_WM_WINDOW_TYPE_DESKTOP
                || type[i] == _NET_WM_WINDOW_TYPE_DOCK)
            {
                is_normal = 0;
            }
        }
        free(reply);
        if (!is_normal) return 0;
    }

    return 1;
}

int window_icon_geometry(xcb_connection_t *c, xcb_window_t w, int geometry[4]) {
    xcb_get_property_cookie_t cookie = xcb_get_property(
        c, 0,
        w, _NET_WM_ICON_GEOMETRY, XCB_ATOM_CARDINAL,
        0, 4);

    xcb_get_property_reply_t *reply = xcb_get_property_reply(c, cookie, NULL);
    if (!reply) return 0;
    assert(xcb_get_property_value_length(reply) == 4 * 4);

    int32_t *value = (int32_t *)xcb_get_property_value(reply);
    for (int i = 0; i < 4; ++i) {
        geometry[i] = value[i];
    }
    free(reply);

    return 1;
}

typedef struct {
    xcb_window_t window;
    int icon_geometry[4];
} window_info;

typedef struct {
    size_t length;
    window_info values[];
} window_list;

window_list *get_windows(xcb_connection_t *c, xcb_window_t root_window, int desktop) {
    xcb_get_property_cookie_t cookie = xcb_get_property(
        c, 0,
        root_window,
        _NET_CLIENT_LIST,
        XCB_ATOM_WINDOW,
        0, -1);

    xcb_get_property_reply_t *reply = xcb_get_property_reply(c, cookie, NULL);
    assert(reply);

    long len = xcb_get_property_value_length(reply) / sizeof(xcb_window_t);
    xcb_window_t *x_windows = (xcb_window_t *)xcb_get_property_value(reply);

    window_list *list = (window_list *)malloc(sizeof *list + len * sizeof(window_info));
    int list_i = 0;
    for (int i = 0; i < len; ++i) {
        xcb_window_t w = x_windows[i];

        if (window_get_desktop(c, w) != desktop) continue;
        if (!window_is_normal(c, w)) continue;

        list->values[list_i].window = w;
        if (!window_icon_geometry(c, w, list->values[list_i].icon_geometry)) continue;

        ++list_i;
    }
    list->length = list_i;
    list = (window_list *)realloc(list, sizeof *list + list->length * sizeof(window_info));

    free(reply);

    return list;
}

int window_info_compare(const void *va, const void *vb) {
    const window_info *a = (const window_info *)va;
    const window_info *b = (const window_info *)vb;
    int result;
    if ((result = a->icon_geometry[1] - b->icon_geometry[1])) return result;
    return a->icon_geometry[0] - b->icon_geometry[0];
}

void window_activate(xcb_connection_t *c, xcb_window_t root, xcb_window_t w) {
    xcb_client_message_event_t event;
    event.response_type = XCB_CLIENT_MESSAGE;
    event.sequence = 0;
    event.window = w;
    event.type = _NET_ACTIVE_WINDOW;
    event.format = 32;
    event.data.data32[0] = 2;
    event.data.data32[1] = XCB_CURRENT_TIME;
    event.data.data32[2] = 0;
    event.data.data32[3] = 0;
    event.data.data32[3] = 0;

    uint32_t event_mask = XCB_EVENT_MASK_SUBSTRUCTURE_REDIRECT | XCB_EVENT_MASK_SUBSTRUCTURE_NOTIFY;

    xcb_send_event(c, 0, root, event_mask, (const char*) &event);
    xcb_flush(c);
}


void help(char *app) {
    fprintf(stderr, "Usage: %s <window number>\n", app);
}

int main(int argc, char **argv) {
    char *app = argv[0];
    char **args = argv + 1;
    char args_len = argc - 1;

    if (args_len == 0 || (args_len == 1 && 0 == strcmp(args[0], "-h"))) {
        help(app);
        return 1;
    }

    errno = 0;
    char *endptr;
    char *window_num_str = args[0];
    long window_num = strtol(window_num_str, &endptr, 10);
    if (errno) {
        perror("Bad window number");
        help(app);
        return 1;
    }
    if (endptr == window_num_str) {
        fprintf(stderr, "No digits in window number\n");
        help(app);
        return 1;
    }
    if (window_num < 0 || window_num > INT_MAX) {
        fprintf(stderr, "Window number should be non-negative and not greater than %d\n", INT_MAX);
        help(app);
        return 1;
    }

    int screen_number;
    xcb_connection_t *connection = xcb_connect(NULL, &screen_number);
    assert(connection);
    atoms_intern(connection);

    xcb_screen_t *screen = screen_of_display(connection, screen_number);
    assert(screen);

    xcb_window_t root_window = screen->root;

    int desktop = get_current_desktop(connection, root_window);

    window_list *windows = get_windows(connection, root_window, desktop);
    qsort(windows->values, windows->length, sizeof *windows->values, window_info_compare);

    if ((unsigned long) window_num < windows->length) {
        window_activate(connection, root_window, windows->values[window_num].window);
    }

    free(windows);

    xcb_disconnect(connection);
}
