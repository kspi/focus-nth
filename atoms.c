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
#define DEFATOM(atom) {0},
#include "atoms.inc"
#undef DEFATOM
    };

    int i = 0;
#define DEFATOM(atom) cookies[i] = xcb_intern_atom(conn, 1, sizeof #atom - 1, #atom); ++i;
#include "atoms.inc"
#undef DEFATOM

    i = 0;
#define DEFATOM(atom) atom = get_atom_reply(conn, cookies[i]); ++i;
#include "atoms.inc"
#undef DEFATOM
}

