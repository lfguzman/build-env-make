#include "RWLock.h"


RWLock::RWLock()
    : mutex_{}
    , read_{}
    , write_{}
    , isWriterActive_{false}
    , activeReaders_{0u}
    , readersWaiting_{0u}
    , writersWaiting_{0u}
{
}

void RWLock::lock_to_read(void)
{
    std::unique_lock lock(mutex_);

    if (isWriterActive_) {
        ++readersWaiting_;
        read_.wait(lock, [this]{return !isWriterActive_;});
        --readersWaiting_;
    }

    ++activeReaders_;
}

bool RWLock::try_lock_to_read(void)
{
    std::lock_guard lock(mutex_);

    if (isWriterActive_) {
        return false;
    }

    ++activeReaders_;
    return true;
}

void RWLock::unlock_from_read(void)
{
    std::lock_guard lock(mutex_);

    --activeReaders_;
    if ((activeReaders_ == 0u) && (writersWaiting_ > 0u)) {
        write_.notify_one();
    }
}

void RWLock::lock_to_write(void)
{
    std::unique_lock lock(mutex_);

    if (isWriterActive_ || (activeReaders_ > 0)) {
        ++writersWaiting_;
        write_.wait(lock, [this]{
            return(!isWriterActive_ && (activeReaders_ == 0));
        } );
        --writersWaiting_;
    }

    isWriterActive_ = true;
}

bool RWLock::try_lock_to_write(void)
{
    std::lock_guard lock(mutex_);

    if (isWriterActive_ || (activeReaders_ > 0)) {
        return false;
    }

    isWriterActive_ = true;
    return true;
}

void RWLock::unlock_from_write(void)
{
    std::lock_guard lock(mutex_);

    isWriterActive_ = false;
    if (readersWaiting_) {
        read_.notify_all();
    } else if (writersWaiting_) {
        write_.notify_one();
    }
}
