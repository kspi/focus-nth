#ifndef atoms_h
#define atoms_h

#include <xcb/xcb.h>

#define DEFATOM(atom) xcb_atom_t atom
#include "atoms.inc"
#undef DEFATOM

void atoms_intern(xcb_connection_t *);

#endif
