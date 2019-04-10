#pragma once

#include <romz/btree/BtreeCursorState.h>
#include <romz/container/IntrusiveListNode.h>
#include <cassert>

namespace romz {

struct Context;
struct LocalCursor;
struct Record;


//
// The Cursor structure for a b+tree cursor
//
struct BtreeCursor {
    enum {
        // Cursor does not point to any key
        kStateNil       = 0,
        // Cursor flag: the cursor is coupled
        kStateCoupled   = 1,
        // Cursor flag: the cursor is uncoupled
        kStateUncoupled = 2
    };

    // Constructor
    BtreeCursor(LocalCursor *parent = nullptr);

    // Destructor; asserts that the cursor is nil
    ~BtreeCursor() {
        close();
    }

    // Clones another BtreeCursor
    void clone(BtreeCursor *other);

    // Closes the cursor
    void close() {
        set_to_nil();
    }

    // Compares the current key against |key|
    int compare(Context *context, ups_key_t *key);

    // Returns true if the cursor is nil
    bool is_nil() const {
        return st_.state == kStateNil;
    }

    // Reset's the cursor's state and uninitializes it. After this call
    // the cursor no longer points to any key.
    void set_to_nil();

    // Couples the cursor to a key directly in a page. Also sets the
    // duplicate index.
    void couple_to(Page *page, uint32_t index, int duplicate_index = 0);

    // Returns true if this cursor is coupled to a btree key
    bool is_coupled() const {
        return st_.state == kStateCoupled;
    }

    // Returns true if this cursor is uncoupled
    bool is_uncoupled() const {
        return st_.state == kStateUncoupled;
    }

    // Returns the duplicate index that this cursor points to.
    int duplicate_index() const {
        return st_.duplicate_index;
    }

    // Sets the duplicate key we're pointing to
    void set_duplicate_index(int duplicate_index) {
        st_.duplicate_index = duplicate_index;
    }

    Page *coupled_page() const {
        assert(st_.state == kStateCoupled);
        return st_.coupled_page;
    }

    int coupled_slot() const {
        assert(st_.state == kStateCoupled);
        return st_.coupled_index;
    }

    // Returns the current key of the cursor. Can be a shallow copy!
    ups_key_t *uncoupled_key() {
        assert(st_.state == kStateUncoupled);
        return &st_.uncoupled_key;
    }

    // Uncouples the cursor
    void uncouple_from_page(Context *context);

    // Returns true if a cursor points to this btree key
    bool points_to(Context *context, Page *page, int slot);

    // Returns true if a cursor points to this external key
    bool points_to(Context *context, ups_key_t *key);

    // Moves the btree cursor to the next page
    void move_to_next_page(Context *context);

    // Positions the cursor on a key and retrieves the record (if |record|
    // is a valid pointer)
    void find(Context *context, ups_key_t *key, ByteArray *key_arena,
              Record *record, ByteArray *record_arena,
              uint32_t flags);

    // Moves the cursor to the first, last, next or previous element
    void move(Context *context, ups_key_t *key, ByteArray *key_arena,
              Record *record, ByteArray *record_arena,
              uint32_t flags);

    // Overwrite the record of this cursor
    void overwrite(Context *context, Record *record, uint32_t flags);

    // Returns the number of records of the referenced key
    int record_count(Context *context, uint32_t flags);

    // retrieves the record size of the current record
    uint32_t record_size(Context *context);

    // Uncouples all cursors from a page
    // This method is called whenever the page is deleted or becomes invalid
    static void uncouple_all_cursors(Context *context, Page *page,
                                     int start = 0);

    // The cursor's current state
    BtreeCursorState st_;

    // Linked list of cursors which point to the same page
    IntrusiveListNode<BtreeCursor> list_node;
};
}
