#include "RWLock.h"
#include <iostream>
#include <gtest/gtest.h>
#include <gmock/gmock.h>


using namespace std;


// Examples of Google Mock using templates

class IParent
{
public:
    virtual ~IParent() {};

    virtual void parent_method() = 0;
};

class Member
{
public:
    void member_method()
    {
        cout << "member_method\n";
    }
};

template <class T = Member>
class Parent2 : public IParent
{
public:
    using member_type = T;

    Parent2(member_type* m) : member{m} {}
    void parent_method() override
    {
        member->member_method();
    }
private:
    member_type* member; // member is now of type T* (i.e. member_type*)
};

template <class T = Member>
class Parent : public IParent
{
public:
    Parent() : member{} {}
    void parent_method() override
    {
        member.member_method();
    }
private:
    using member_type = T;
    member_type member; // member is now of type T (i.e. member_type)

    template <class U>
    friend U& GetMemberForTestMock(Parent<U>& parent);
};


// initialize in the test/mock files as this member function is only for
// testing with mockMembers
template <class U>
U& GetMemberForTestMock(Parent<U>& parent)
{
    return parent.member;
}

class MockMember
{
public:
    MOCK_CONST_METHOD0(member_method, void());
};

TEST(ParentTest, callsMemberMethodOnce)
{
    auto parent = Parent<MockMember>{};
    EXPECT_CALL(GetMemberForTestMock(parent), member_method()).Times(1);
    parent.parent_method();
}

TEST(ParentTest, callsMemberMethodOnceAgain)
{
    auto member = MockMember{};
    EXPECT_CALL(member, member_method()).Times(1);

    auto parent = Parent2<MockMember>{&member};
    parent.parent_method();
}
