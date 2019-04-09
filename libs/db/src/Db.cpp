#include <romz/db/Db.h>


namespace romz {

// Constructor
Db::Db(Env *env_, DbConfig &config_)
    : env(env_)
    , context( nullptr )
    , config(config_)
{
}

// Returns the runtime-flags - the flags are "mixed" with the flags from
// the Environment
uint32_t Db::flags() const
{
    // return env->flags() | config.flags;
    return 0;
}

// Returns the database name
uint16_t Db::name() const
{
    return config.db_name;
}

// Sets the database name; required when renaming the local proxy of
// a remote database
void Db::set_name(uint16_t name)
{
    config.db_name = name;
}

// Returns the memory buffer for the key data: the per-database buffer
// if |txn| is null or temporary, otherwise the buffer from the |txn|
ByteArray &Db::key_arena(Txn */*txn*/)
{
//    return (txn == 0 || ISSET(txn->flags, UPS_TXN_TEMPORARY))
//            ? _key_arena
//            : txn->key_arena;

    throw "Db::key_arena not implemented";
}

// Returns the memory buffer for the record data: the per-database buffer
// if |txn| is null or temporary, otherwise the buffer from the |txn|
ByteArray &Db::record_arena(Txn */*txn*/)
{
//    return (txn == 0 || ISSET(txn->flags, UPS_TXN_TEMPORARY))
//            ? _record_arena
//            : txn->record_arena;

    throw "Db::record_arena not implemented";
}

void Db::add_cursor(Cursor *cursor)
{
    cursor_list.push_back( cursor );
}

void Db::remove_cursor(Cursor *cursor)
{
    cursor_list.remove( cursor );
    // fix the linked list of cursors
}

}
