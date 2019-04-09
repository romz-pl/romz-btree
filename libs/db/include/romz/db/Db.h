#pragma once

#include <romz/config/DbConfig.h>
#include <romz/container/ByteArray.h>
#include <list>

namespace romz {

struct Cursor;
struct ScanVisitor;
struct Env;
struct ups_env_metrics_t;
struct Txn;
struct ups_key_t;
struct ups_record_t;
struct ups_operation_t;
struct ups_parameter_t;

///
/// An abstract base class for a Database; is overwritten for local and
/// remote implementations
///


struct Db
{
    Db(Env *env_, DbConfig &config_);

    virtual ~Db() = default;

    uint32_t flags() const;

    uint16_t name() const;

    void set_name(uint16_t name);

    // Fills in the current metrics
    virtual void fill_metrics(ups_env_metrics_t *metrics) = 0;

    // Returns the database parameters (ups_db_get_parameters)
    virtual void get_parameters(ups_parameter_t *param) = 0;

    // Checks the database integrity (ups_db_check_integrity)
    virtual void check_integrity(uint32_t flags) = 0;

    // Returns the number of keys (ups_db_count)
    virtual uint64_t count(Txn *txn, bool distinct) = 0;

    // Inserts a key/value pair (ups_db_insert, ups_cursor_insert)
    virtual void insert(Cursor *cursor, Txn *txn, ups_key_t *key, ups_record_t *record, uint32_t flags) = 0;

    // Erase a key/value pair (ups_db_erase, ups_cursor_erase)
    virtual void erase(Cursor *cursor, Txn *txn, ups_key_t *key, uint32_t flags) = 0;

    // Lookup of a key/value pair (ups_db_find, ups_cursor_find)
    virtual void find(Cursor *cursor, Txn *txn, ups_key_t *key, ups_record_t *record, uint32_t flags) = 0;

    // Creates a cursor (ups_cursor_create)
    virtual Cursor *cursor_create(Txn *txn, uint32_t flags) = 0;

    // Clones a cursor (ups_cursor_clone)
    virtual Cursor *cursor_clone(Cursor *src) = 0;

    // Moves a cursor, returns key and/or record (ups_cursor_move)
    virtual void cursor_move(Cursor *cursor, ups_key_t *key, ups_record_t *record, uint32_t flags) = 0;

    // Performs bulk operations
    virtual void bulk_operations(Txn *txn, ups_operation_t *operations, size_t operations_length, uint32_t flags) = 0;

    // Closes the database (ups_db_close)
    virtual void close(uint32_t flags) = 0;

    // Adds a cursor to the linked list of cursors
    void add_cursor(Cursor *cursor);

    // Removes a cursor from the linked list of cursors
    void remove_cursor(Cursor *cursor);

    ByteArray &key_arena(Txn *txn);
    ByteArray &record_arena(Txn *txn);

    // the current Environment
    Env *env;

    // the user-provided context data
    void *context;

    // linked list of all cursors
    std::list< Cursor* > cursor_list;

    // the configuration settings
    DbConfig config;

    // This is where key->data points to when returning a
    // key to the user; used if Txns are disabled
    ByteArray _key_arena;

    // This is where record->data points to when returning a
    // record to the user; used if Txns are disabled
    ByteArray _record_arena;
};

}
