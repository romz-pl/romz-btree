#pragma once

#include <cstdio>
#include <cstdint>

namespace romz {

///
/// A simple wrapper around a file handle. Throws exceptions in
/// case of errors. Moves the file handle when copied.
///
class File
{
private:
    constexpr static int UPS_INVALID_FD = -1;

public:
    enum {
        kSeekSet = SEEK_SET,
        kSeekEnd = SEEK_END,
        kSeekCur = SEEK_CUR,
        // kMaxPath = PATH_MAX
    };


    File();
    File(File &&other);
    ~File();
    File &operator=(File &&other);


    void create(const char *filename, std::uint32_t mode);
    void open(const char *filename, bool read_only);
    void close();
    bool is_open() const;

    void flush();

    void set_posix_advice(int parameter);

    void mmap(uint64_t offset, std::size_t size, bool readonly, std::uint8_t **buffer);
    void munmap(void *buffer, std::size_t size);

    void pread(std::uint64_t addr, void *buffer, std::size_t len);
    void pwrite(std::uint64_t addr, const void *buffer, std::size_t len);

    void write(const void *buffer, size_t len);


    static std::size_t granularity();


    void seek(std::uint64_t offset, int whence) const;
    std::uint64_t tell() const;

    std::uint64_t file_size() const;
    void truncate(std::uint64_t newsize);

private:

    // The file handle
    int m_fd;

    // Parameter for posix_fadvise()
    int m_posix_advice;
};

}
