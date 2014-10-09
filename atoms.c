#include <stdlib.h>
#include <assert.h>
#include "atoms.h"

static xcb_atom_t get_atom_reply(xcb_connection_t *conn, xcb_intern_atom_cookie_t cookie) {
    xcb_intern_atom_reply_t *reply = xcb_intern_atom_reply(conn, cookie, NULL);
    assert(reply);
    xcb_atom_t atom = reply->atom;
    free(reply);
    assert(atom);
    return atom;
}


void atoms_intern(xcb_connection_t *conn) {
    xcb_intern_atom_cookie_t cookies[] = {
#define DEFATOM(atom) xcb_intern_atom(conn, 1, sizeof #atom - 1, #atom),
#include "atoms.inc"
#undef DEFATOM
    };

    xcb_intern_atom_cookie_t *cookie = cookies;
#define DEFATOM(atom) atom = get_atom_reply(conn, *cookie++);
#include "atoms.inc"
#undef DEFATOM
}

