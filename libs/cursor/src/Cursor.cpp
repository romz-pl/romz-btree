#include <romz/cursor/Cursor.h>

namespace romz {

// Constructor
Cursor::Cursor(Db *db_, Txn *txn_)
    : db(db_)
    , txn(txn_)
    , next( nullptr )
    , previous( nullptr )
{
}

// Copy constructor; used for cloning a Cursor
Cursor::Cursor(Cursor &other)
    : db(other.db)
    , txn(other.txn)
    , next( nullptr )
    , previous( nullptr )
{
}

}
