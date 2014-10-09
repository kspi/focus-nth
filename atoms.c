#include <stdlib.h>
#include <assert.h>
#include "atoms.h"

static xcb_atom_t get_atom(xcb_connection_t *conn, xcb_intern_atom_cookie_t cookie) {
    xcb_intern_atom_reply_t *reply = xcb_intern_atom_reply(conn, cookie, NULL);
    assert(reply);
    xcb_atom_t atom = reply->atom;
    free(reply);
    assert(atom);
    return atom;
}


void atoms_intern(xcb_connection_t *conn) {
#define DEFATOM(atom) xcb_intern_atom_cookie_t atom##_cookie = xcb_intern_atom(conn, 1, sizeof #atom - 1, #atom);
#include "atoms.inc"
#undef DEFATOM

#define DEFATOM(atom) atom = get_atom(conn, atom##_cookie);
#include "atoms.inc"
#undef DEFATOM
}

