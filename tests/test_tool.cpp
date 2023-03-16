#include <iostream>
#include <string>
#include "magic_enum.hpp"
#include "bcd.h"

template <typename TestResult>
class TestCodeBase
{
private:
    virtual std::string _toString(TestResult x) = 0;
    std::string _methodName;
    bool _allPass;
    auto createTag(std::string testName) const
    {
        return "[function:" + _methodName + "] of[" + testName + "]";
    }
    auto printState(bool isPass, std::string testTag) const
    {
        if (isPass)
        {
            std::cout << "[OK"
                      << "\033[1;32m ●\033[0m ]:" + testTag + " is"
                      << "\033[1;32m PASS\033[0m" << std::endl;
        }
        else
        {
            std::cout << "[Faild"
                      << "\033[1;31m ●\033[0m ]:" + testTag + " is\033[1;31m Faild\033[0m" << std::endl;
        }
    }

    auto assertFaild(std::string testName, TestResult expected, TestResult actual)
    {
        std::cout << " "
                  << "\033[1;31m ● " + testName + "\033[0m" << std::endl;
        std::cout << "expected, "
                  << "\033[1;97;42m-  " + this->_toString(expected) + "  -\033[0m" << std::endl;
        std::cout << "but, got  "
                  << "\033[1;31;100m+  " + this->_toString(actual) + "  +\033[0m" << std::endl;
    }

protected:
    const void expectEq(std::string testName, TestResult expected, TestResult actual)
    {
        auto testTag = createTag(testName);

        std::cout << "[TEST]:" + testTag << std::endl;

        if (expected == actual)
        {
            printState(true, testTag);
        }
        else
        {
            printState(false, testTag);
            assertFaild(testName, expected, actual);
            _allPass = false;
        }
        std::cout << std::endl;
    }

public:
    TestCodeBase(std::string testTargetMethodName)
        : _methodName(testTargetMethodName), _allPass(true)
    {
    }

    virtual void runTest() = 0;
};

class TEST_absCompear : TestCodeBase<COMPEAR_RESULT>
{
private:
    std::string _toString(COMPEAR_RESULT x) override
    {
        return std::string(magic_enum::enum_name(x));
    }

public:
    TEST_absCompear() : TestCodeBase<COMPEAR_RESULT>("absCompear")
    {
    }

    void runTest() override
    {
        std::cout << "[absCompear_TEST]" << std::endl;

        BCD *a;
        BCD *b;
        COMPEAR_RESULT result;

        a = convertDECtoBCD(0, false);
        b = convertDECtoBCD(4294967295, false);
        result = absCompear(a, b);
        this->expectEq("a:0 <  b:max", COMPEAR_RESULT::RIGHT, result);

        a = convertDECtoBCD(4294967295, false);
        b = convertDECtoBCD(0, false);
        result = absCompear(a, b);
        this->expectEq("a:max >  b:0", COMPEAR_RESULT::LEFT, result);

        a = convertDECtoBCD(0, false);
        b = convertDECtoBCD(4294967295, true);
        result = absCompear(a, b);
        this->expectEq("a:0 <  b:-max", COMPEAR_RESULT::RIGHT, result);

        a = convertDECtoBCD(4294967295, true);
        b = convertDECtoBCD(0, false);
        result = absCompear(a, b);
        this->expectEq("a:-max >  b:0", COMPEAR_RESULT::LEFT, result);

        a = convertDECtoBCD(0, true);
        b = convertDECtoBCD(4294967295, false);
        result = absCompear(a, b);
        this->expectEq("a:-0 <  b:max", COMPEAR_RESULT::RIGHT, result);

        a = convertDECtoBCD(4294967295, false);
        b = convertDECtoBCD(0, true);
        result = absCompear(a, b);
        this->expectEq("a:max >  b:-0", COMPEAR_RESULT::LEFT, result);

        a = convertDECtoBCD(1, false);
        b = convertDECtoBCD(2, false);
        result = absCompear(a, b);
        this->expectEq("a:1 <  b:2", COMPEAR_RESULT::RIGHT, result);

        a = convertDECtoBCD(2, false);
        b = convertDECtoBCD(1, true);
        result = absCompear(a, b);
        this->expectEq("a:2 >  b:-1", COMPEAR_RESULT::LEFT, result);

        a = convertDECtoBCD(0, false);
        b = convertDECtoBCD(0, false);
        result = absCompear(a, b);
        this->expectEq("a:zero =  b:zero", COMPEAR_RESULT::EQUAL, result);

        a = convertDECtoBCD(1, false);
        b = convertDECtoBCD(1, false);
        result = absCompear(a, b);
        this->expectEq("a:1 =  b:1", COMPEAR_RESULT::EQUAL, result);

        a = convertDECtoBCD(4294967295, false);
        b = convertDECtoBCD(4294967295, false);
        result = absCompear(a, b);
        this->expectEq("a:max =  b:max", COMPEAR_RESULT::EQUAL, result);
    }
};

// void unsignedAdd_TEST()
// {
//     std::cout << "[unsignedAdd_TEST]" << std::endl;

//     BCD *a;
//     BCD *b;
//     BCD *result;

//     a = convertDECtoBCD(0, false);
//     b = convertDECtoBCD(4294967295, false);
//     result = unsignedAdd(a, b);
//     _tool->expectEq("a:0 +  b:max = max", COMPEAR_RESULT::RIGHT, result);
// }

int main(int argc, char **argv)
{
    std::cout << "[Start unit testing]" << std::endl
              << std::endl;
    TEST_absCompear _absCompear_TEST;
    _absCompear_TEST.runTest();

    return 0;
}