#ifndef INCLUDED_RWLOCK_H
#define INCLUDED_RWLOCK_H


// First iteration of a rwlock with Read Precedence


#include <mutex>


/**
 * Enables multiple readers to get the lock at one time but only one writer
 */
class RWLock
{
public:
    // TYPES
    using size_type = size_t;

    // CREATORS
    RWLock();
    RWLock(RWLock const &) = delete;
    RWLock(RWLock&&) = delete;
    RWLock&  operator=(RWLock const &) = delete;
    RWLock&  operator=(RWLock&&) = delete;
    
    ~RWLock() = default;

    // MUTATORS
    void lock_to_read(void);
    bool try_lock_to_read(void);

    void lock_to_write(void);
    bool try_lock_to_write(void);

    void unlock_from_read(void);
    void unlock_from_write(void);

    // ACCESSORS (for AUTs, mainly)
    bool isWriterActive(void) const;
    size_type getActiveReaders(void) const;
    size_type getReadersWaiting(void) const;
    size_type getWritersWaiting(void) const;

private:
    mutable std::mutex mutex_;
    std::condition_variable read_;   // wait for read
    std::condition_variable write_;   // wait for write
    bool isWriterActive_;
    size_type activeReaders_;
    size_type readersWaiting_;
    size_type writersWaiting_;
};

// adapter to be able to use std guards
class read_lock
{
public:
    explicit read_lock(RWLock& lock)
        : rw_lock_{lock}
    { }

    read_lock(read_lock const &) = delete;
    read_lock& operator=(read_lock const &) = delete;
    read_lock(read_lock&&) = delete;
    read_lock& operator=(read_lock&&) = delete;

    ~read_lock() = default;

    void lock()
    { return rw_lock_.lock_to_read(); }

    bool try_lock()
    { return rw_lock_.try_lock_to_read(); }

    void unlock()
    { return rw_lock_.unlock_from_read(); }

private:
    RWLock& rw_lock_;
};


// adapter to be able to use std guards
class write_lock
{
public:
    explicit write_lock(RWLock& lock)
        : rw_lock_{lock}
    { }

    write_lock(write_lock const &) = delete;
    write_lock& operator=(write_lock const &) = delete;
    write_lock(write_lock&&) = delete;
    write_lock& operator=(write_lock&&) = delete;

    ~write_lock() = default;

    void lock()
    { return rw_lock_.lock_to_write(); }

    bool try_lock()
    { return rw_lock_.try_lock_to_write(); }

    void unlock()
    { return rw_lock_.unlock_from_write(); }

private:
    RWLock& rw_lock_;
};


#endif   // INCLUDED_RWLOCK_H
