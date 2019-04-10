#pragma once

#include <romz/container/ByteArray.h>

namespace romz {

struct LocalCursor;
struct BtreeIndex;
struct Page;

struct ups_key_t
{

};

struct BtreeCursorState
{
    // the parent cursor
    LocalCursor *parent;

    // The BtreeIndex instance
    BtreeIndex *btree;

    // "coupled" or "uncoupled" states; coupled means that the
    // cursor points into a Page object, which is in
    // memory. "uncoupled" means that the cursor has a copy
    // of the key on which it points (i.e. because the coupled page was
    // flushed to disk and removed from the cache)
    int state;

    // the id of the duplicate key to which this cursor is coupled
    int duplicate_index;

    // for coupled cursors: the page we're pointing to
    Page *coupled_page;

    // ... and the index of the key in that page
    int coupled_index;

    // for uncoupled cursors: a copy of the key at which we're pointing
    ups_key_t uncoupled_key;

    // a ByteArray which backs |uncoupled_key.data|
    ByteArray uncoupled_arena;
};


}
