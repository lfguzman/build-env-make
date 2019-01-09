#include "RWLock.h"
#include <iostream>
#include <gtest/gtest.h>
#include <gmock/gmock.h>


using namespace std;


class RWLockTest : public ::testing::Test
{
protected:
    RWLockTest()
        : rwlock_{}
        , rlock_{rwlock_}
        , wlock_{rwlock_}
    { }

    ~RWLockTest() = default;

    bool isLockedForRead(void) const
    {
        return(rwlock_.getActiveReaders() > 0);
    }

    bool isLockedForWrite(void) const
    {
        return rwlock_.isWriterActive();
    }

    RWLock rwlock_;
    read_lock<> rlock_;  // only difference with adaptor being a template
    write_lock wlock_;
};

TEST_F(RWLockTest, lockToRead_isNotLocked_shouldLockForRead)
{
    // excercise SUT
    auto lock = lock_guard{rlock_};

    // verify output
    ASSERT_TRUE(isLockedForRead());
    ASSERT_FALSE(isLockedForWrite());
}

TEST_F(RWLockTest, lockToWrite_isNotLocked_shouldLockForWrite)
{
    // excercise SUT
    auto lock = lock_guard{wlock_};

    // verify output
    ASSERT_TRUE(isLockedForWrite());
    ASSERT_FALSE(isLockedForRead());
}

TEST_F(RWLockTest, tryLockToRead_isLockedForRead_shouldLock)
{
    // setup fixture
    auto lock = lock_guard{rlock_};

    // exercise SUT
    // and
    // verify output
    ASSERT_TRUE(rwlock_.try_lock_to_read());
}

TEST_F(RWLockTest, tryLockToRead_isLockedForWrite_shouldNotLock)
{
    // setup fixture
    auto lock = lock_guard{wlock_};

    // exercise SUT
    // and
    // verify output
    ASSERT_FALSE(rwlock_.try_lock_to_read());
}

TEST_F(RWLockTest, tryLockToWrite_isLockedForRead_shouldNotLock)
{
    // setup fixture
    auto lock = lock_guard{rlock_};

    // exercise SUT
    // and
    // verify output
    ASSERT_FALSE(rwlock_.try_lock_to_write());
}

TEST_F(RWLockTest, tryLockToWrite_isLockedForWrite_shouldNotLock)
{
    // setup fixture
    auto lock = lock_guard{wlock_};

    // exercise SUT
    // and
    // verify output
    ASSERT_FALSE(rwlock_.try_lock_to_write());
}

TEST_F(RWLockTest, unlockFromWrite_isLockedForWrite_shouldUnlock)
{
    // setup fixture
    {
        auto lock = lock_guard{wlock_};

    // exercise SUT
    // leaving scope calls unlock_from_write
    }

    // verify output
    ASSERT_FALSE(isLockedForWrite());
    ASSERT_FALSE(isLockedForRead());
}

TEST_F(RWLockTest, unlockFromRead_isLockedForRead_shouldUnlock)
{
    // setup fixture
    {
        auto lock = lock_guard{rlock_};

    // exercise SUT
    // leaving scope calls unlock_from_read
    }

    // verify output
    ASSERT_FALSE(isLockedForWrite());
    ASSERT_FALSE(isLockedForRead());
}


using ::testing::Return;

class MockRWLock
{
public:
    MockRWLock() = default;
    ~MockRWLock() = default;

    MOCK_CONST_METHOD0(lock_to_read, void());
    MOCK_CONST_METHOD0(try_lock_to_read, bool());
    MOCK_CONST_METHOD0(unlock_from_read, void());
};

TEST(read_lockTest, lockUnlock_usingStdLockGuard_shouldCallLockAndUnlock)
{
    // setup fixture
    auto mockRWLock = MockRWLock{};
    EXPECT_CALL(mockRWLock, lock_to_read()).Times(1);
    EXPECT_CALL(mockRWLock, try_lock_to_read()).Times(0);
    EXPECT_CALL(mockRWLock, unlock_from_read()).Times(1);

    // exercise SUT
    auto rlock = read_lock{mockRWLock};
    lock_guard{rlock};

    // verify output done by gmock and gtest
}

TEST(read_lockTest, tryLock_FailsTry_shouldNotAquireLock)
{
    // setup fixture
    auto mockRWLock = MockRWLock{};
    EXPECT_CALL(mockRWLock, lock_to_read()).Times(0);
    EXPECT_CALL(mockRWLock, try_lock_to_read()).WillOnce(Return(false));
    EXPECT_CALL(mockRWLock, unlock_from_read()).Times(0);

    // exercise SUT
    auto rlock = read_lock{mockRWLock};
    auto lock = unique_lock{rlock, std::try_to_lock};

    // verify output
    ASSERT_FALSE(lock) << "It shouldn't have acquired lock";
}

TEST(read_lockTest, tryLock_SucceedsTry_shouldAquireLock)
{
    // setup fixture
    auto mockRWLock = MockRWLock{};
    EXPECT_CALL(mockRWLock, lock_to_read()).Times(0);
    EXPECT_CALL(mockRWLock, try_lock_to_read()).WillOnce(Return(true));
    EXPECT_CALL(mockRWLock, unlock_from_read()).Times(1);

    // exercise SUT
    auto rlock = read_lock{mockRWLock};
    auto lock = unique_lock{rlock, std::try_to_lock};

    // verify output
    ASSERT_TRUE(lock) << "It should have acquired lock";
}
