#pragma once

#include <cstdio>
#include <cstdint>
#include <string>

namespace romz {

///
/// A simple wrapper around a file handle. Throws exceptions in
/// case of errors. Moves the file handle when copied.
///
class File
{
public:
    File( const std::string& path );
    File( const std::string& path, bool read_only );
    ~File();

    File( const File &other ) = delete;
    File( File &&other ) = delete;

    File &operator=( const File &other ) = delete;
    File &operator=( File &&other ) = delete;

    void pread( void *buf, std::uint64_t count, std::uint64_t offset );
    void pwrite( const void *buf, std::uint64_t count, std::uint64_t offset );

    void read( void *buf, uint64_t count );
    void write( const void *buffer, uint64_t len );


    static std::size_t page_size();


    void lseek( std::uint64_t offset ) const;
    std::uint64_t tell() const;


    void fsync() const;
    void fdatasync() const;

    std::uint64_t file_size() const;
    void truncate( std::uint64_t length ) const;

private:
    void lock_exclusive() const;
    void set_posix_advice() const;

    [[ noreturn ]] static void exception_with_errno( const std::string& txt );


private:
    // The file handle
    int m_fd;

    // Parameter for posix_fadvise()
    int m_posix_advice;
};

}
