#include <stdlib.h>
#include <assert.h>
#include "atoms.h"

static xcb_atom_t get_atom(xcb_connection_t *conn, const char *name, size_t name_len) {
    xcb_intern_atom_cookie_t cookie = xcb_intern_atom(conn, 0, name_len, name);
    xcb_intern_atom_reply_t *reply = xcb_intern_atom_reply(conn, cookie, NULL);
    xcb_atom_t atom = reply->atom;
    free(reply);
    assert(atom);
    return atom;
}


void atoms_intern(xcb_connection_t *conn) {
#define DEFATOM(atom) atom = get_atom(conn, #atom, sizeof #atom - 1)
#include "atoms.inc"
#undef DEFATOM
}

