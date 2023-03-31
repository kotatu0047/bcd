#include <iostream>
#include <string>
#include <vector>
#include <tuple>
#include "magic_enum.hpp"
#include "bcd.h"

// test tool code
/***********************************/

/**
 *
 * @  kotatu0047 test
 *
 */
template <typename TestResult>
class TestCodeBase
{
private:
    virtual std::string _toString(TestResult x) = 0;
    virtual bool _isEqual(TestResult a, TestResult b)
    {
        return a == b;
    };

    std::string _methodName = "";
    int _passCase = 0;
    int _faildCase = 0;
    auto createTag(std::string testName) const
    {
        return "[function:" + _methodName + "] of [" + testName + "]";
    }
    auto printState(bool isPass, std::string testTag)
    {
        if (isPass)
        {
            this->_passCase++;

            std::cout << "[OK"
                      << "\033[1;32m ●\033[0m ]:"
                      << "\033[1;32m PASS\033[0m" << std::endl;
        }
        else
        {
            this->_faildCase++;

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

        std::cout << "[TEST]:" + testTag;

        if (this->_isEqual(expected, actual))
        {
            printState(true, testTag);
        }
        else
        {
            printState(false, testTag);
            assertFaild(testName, expected, actual);
        }
        std::cout << std::endl;
    }

    void printTestStart() const
    {
        std::cout << "========================[" << this->_methodName << "]========================" << std::endl;
    }

    void printTestEnd() const
    {
        std::cout << "========================[" << this->_methodName << "] test finished===========" << std::endl;
    }

public:
    TestCodeBase(std::string testTargetMethodName)
        : _methodName(testTargetMethodName)
    {
    }
    virtual void runTest() = 0;

    /**
        std::tuple<passCase, faildCase, methodName>;
     */
    std::tuple<int, int, std::string> getTestResult() const
    {

        if ((this->_passCase + this->_faildCase) == 0)
        {
            throw std::logic_error("You must execute the runTest method before executing the getTestResult method.");
        }

        return std::make_tuple(this->_passCase, this->_faildCase, this->_methodName);
    }
};

/***********************************/

class TEST_absCompear : public TestCodeBase<COMPEAR_RESULT>
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
        this->printTestStart();

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

        a = convertDECtoBCD(2147483648, false);
        b = convertDECtoBCD(2147483647, false);
        result = absCompear(a, b);
        this->expectEq("a:2147483648 =  b:2147483647", COMPEAR_RESULT::LEFT, result);
    }
};

class TEST_bcdCompear : public TestCodeBase<COMPEAR_RESULT>
{
private:
    std::string _toString(COMPEAR_RESULT x) override
    {
        return std::string(magic_enum::enum_name(x));
    }

public:
    TEST_bcdCompear() : TestCodeBase<COMPEAR_RESULT>("bcdCompear")
    {
    }

    void runTest() override
    {
        this->printTestStart();

        BCD *a;
        BCD *b;
        COMPEAR_RESULT result;

        a = convertDECtoBCD(4294967295, false);
        b = convertDECtoBCD(0, false);
        result = bcdCompear(a, b);
        this->expectEq("a:max >  b:0", COMPEAR_RESULT::LEFT, result);

        a = convertDECtoBCD(2, false);
        b = convertDECtoBCD(1, true);
        result = bcdCompear(a, b);
        this->expectEq("a:2 >  b:-1", COMPEAR_RESULT::LEFT, result);

        a = convertDECtoBCD(2, true);
        b = convertDECtoBCD(1, false);
        result = bcdCompear(a, b);
        this->expectEq("a:-2 <  b:1", COMPEAR_RESULT::RIGHT, result);

        a = convertDECtoBCD(2, true);
        b = convertDECtoBCD(1, true);
        result = bcdCompear(a, b);
        this->expectEq("a:-2 <  b:-1", COMPEAR_RESULT::RIGHT, result);

        a = convertDECtoBCD(0, false);
        b = convertDECtoBCD(4294967295, false);
        result = bcdCompear(a, b);
        this->expectEq("a:0 <  b:max", COMPEAR_RESULT::RIGHT, result);

        a = convertDECtoBCD(0, false);
        b = convertDECtoBCD(4294967295, true);
        result = bcdCompear(a, b);
        this->expectEq("a:0 > b:-max", COMPEAR_RESULT::LEFT, result);

        a = convertDECtoBCD(4294967295, true);
        b = convertDECtoBCD(0, false);
        result = bcdCompear(a, b);
        this->expectEq("a:-max < b:0", COMPEAR_RESULT::RIGHT, result);

        a = convertDECtoBCD(999, true);
        b = convertDECtoBCD(1000, true);
        result = bcdCompear(a, b);
        this->expectEq("a:-999 >  b:-1000", COMPEAR_RESULT::LEFT, result);

        a = convertDECtoBCD(4294967295, false);
        b = convertDECtoBCD(4294967295, false);
        result = bcdCompear(a, b);
        this->expectEq("a:max =  b:max", COMPEAR_RESULT::EQUAL, result);

        a = convertDECtoBCD(4294967295, false);
        b = convertDECtoBCD(4294967295, true);
        result = bcdCompear(a, b);

        this->expectEq("a:max >  b:-max", COMPEAR_RESULT::LEFT, result);

        a = convertDECtoBCD(1, true);
        b = convertDECtoBCD(1, false);
        result = bcdCompear(a, b);
        this->expectEq("a:-1 <  b:1", COMPEAR_RESULT::RIGHT, result);

        a = convertDECtoBCD(1, true);
        b = convertDECtoBCD(1, true);
        result = bcdCompear(a, b);
        this->expectEq("a:-1 = b:-1", COMPEAR_RESULT::EQUAL, result);

        a = convertDECtoBCD(0, false);
        b = convertDECtoBCD(0, false);
        result = bcdCompear(a, b);
        this->expectEq("a:0 = b:0", COMPEAR_RESULT::EQUAL, result);

        a = convertDECtoBCD(0, true);
        b = convertDECtoBCD(0, false);
        result = bcdCompear(a, b);
        this->expectEq("a:-0 = b:0", COMPEAR_RESULT::EQUAL, result);

        a = convertDECtoBCD(0, false);
        b = convertDECtoBCD(0, true);
        result = bcdCompear(a, b);
        this->expectEq("a:0 = b:-0", COMPEAR_RESULT::EQUAL, result);

        a = convertDECtoBCD(0, true);
        b = convertDECtoBCD(0, true);
        result = bcdCompear(a, b);
        this->expectEq("a:-0 = b:-0", COMPEAR_RESULT::EQUAL, result);

        a = convertDECtoBCD(1, false);
        b = convertDECtoBCD(2, false);
        result = bcdCompear(a, b);
        this->expectEq("a:1 <  b:2", COMPEAR_RESULT::RIGHT, result);

        a = convertDECtoBCD(2, false);
        b = convertDECtoBCD(1, true);
        result = bcdCompear(a, b);
        this->expectEq("a:2 >  b:-1", COMPEAR_RESULT::LEFT, result);

        a = convertDECtoBCD(1, false);
        b = convertDECtoBCD(1, false);
        result = bcdCompear(a, b);
        this->expectEq("a:1 =  b:1", COMPEAR_RESULT::EQUAL, result);

        a = convertDECtoBCD(4294967295, false);
        b = convertDECtoBCD(4294967295, false);
        result = bcdCompear(a, b);
        this->expectEq("a:max =  b:max", COMPEAR_RESULT::EQUAL, result);

        a = convertDECtoBCD(2147483648, false);
        b = convertDECtoBCD(2147483647, false);
        result = bcdCompear(a, b);
        this->expectEq("a:2147483648 =  b:2147483647", COMPEAR_RESULT::LEFT, result);
    }
};

class TEST_unsignedAdd : public TestCodeBase<BCD *>
{
private:
    std::string _toString(BCD *x) override
    {
        char str[BCD_MAX_DIGITS + 1];
        convertBCDtoSTR(x, str, true, true);
        return std::string(str, BCD_MAX_DIGITS + 1);
    }
    bool _isEqual(BCD *a, BCD *b) override
    {
        if (bcdCompear(a, b) == COMPEAR_RESULT::EQUAL)
        {
            return true;
        }
        else
        {
            return false;
        }
    }

public:
    TEST_unsignedAdd() : TestCodeBase<BCD *>("unsignedAdd")
    {
    }

    void runTest() override
    {
        this->printTestStart();

        BCD *a;
        BCD *b;
        BCD *result;
        BCD *expected;

        a = convertDECtoBCD(0, false);
        b = convertDECtoBCD(4294967295, false);
        result = unsignedAdd(a, b);
        expected = convertDECtoBCD(4294967295, false);
        this->expectEq("a:|0| + b:|max| = max", expected, result);

        a = convertDECtoBCD(4294967295, false);
        b = convertDECtoBCD(0, false);
        result = unsignedAdd(a, b);
        expected = convertDECtoBCD(4294967295, false);
        this->expectEq("a:|max| + b:|0| = max", expected, result);

        a = convertDECtoBCD(2147483648, false);
        b = convertDECtoBCD(2147483647, false);
        result = unsignedAdd(a, b);
        expected = convertDECtoBCD(4294967295, false);
        this->expectEq("a:|2147483648| + b:|2147483647| = max", expected, result);

        a = convertDECtoBCD(0, false);
        b = convertDECtoBCD(0, false);
        result = unsignedAdd(a, b);
        expected = convertDECtoBCD(0, false);
        this->expectEq("a:|0| + b:|0| = 0", expected, result);

        a = convertDECtoBCD(0, false);
        b = convertDECtoBCD(0, true);
        result = unsignedAdd(a, b);
        expected = convertDECtoBCD(0, false);
        this->expectEq("a:0 + b:|-0| = 0", expected, result);

        a = convertDECtoBCD(1, false);
        b = convertDECtoBCD(1, false);
        result = unsignedAdd(a, b);
        expected = convertDECtoBCD(2, false);
        this->expectEq("a:|1| + b:|1| = 2", expected, result);

        a = convertDECtoBCD(6, false);
        b = convertDECtoBCD(5, false);
        result = unsignedAdd(a, b);
        expected = convertDECtoBCD(11, false);
        this->expectEq("a:|6| + b:|5| = 11", expected, result);

        a = convertDECtoBCD(99009, false);
        b = convertDECtoBCD(999, false);
        result = unsignedAdd(a, b);
        expected = convertDECtoBCD(100008, false);
        this->expectEq("a:|99009| + b:|999| = 100008", expected, result);

        a = convertDECtoBCD(99009, true);
        b = convertDECtoBCD(999, true);
        result = unsignedAdd(a, b);
        expected = convertDECtoBCD(100008, false);
        this->expectEq("a:|-99009| + b:|-999| = 100008", expected, result);
    }
};

class TEST_unsignedSub : public TestCodeBase<BCD *>
{
private:
    std::string _toString(BCD *x) override
    {
        char str[BCD_MAX_DIGITS + 1];
        convertBCDtoSTR(x, str, true, true);
        return std::string(str, BCD_MAX_DIGITS + 1);
    }
    bool _isEqual(BCD *a, BCD *b) override
    {
        if (bcdCompear(a, b) == COMPEAR_RESULT::EQUAL)
        {
            return true;
        }
        else
        {
            return false;
        }
    }

public:
    TEST_unsignedSub() : TestCodeBase<BCD *>("unsignedSub")
    {
    }

    void runTest() override
    {
        this->printTestStart();

        BCD *a;
        BCD *b;
        BCD *result;
        BCD *expected;

        a = convertDECtoBCD(0, false);
        b = convertDECtoBCD(4294967295, false);
        result = unsignedSub(a, b);
        expected = convertDECtoBCD(4294967295, true);
        this->expectEq("a:0 - b:max = -max", expected, result);

        a = convertDECtoBCD(4294967295, false);
        b = convertDECtoBCD(0, false);
        result = unsignedSub(a, b);
        expected = convertDECtoBCD(4294967295, false);
        this->expectEq("a:max - b:0 = max", expected, result);

        a = convertDECtoBCD(2147483648, false);
        b = convertDECtoBCD(2147483648, false);
        result = unsignedSub(a, b);
        expected = convertDECtoBCD(0, false);
        this->expectEq("a:2147483648 - b:2147483648 = 0", expected, result);

        a = convertDECtoBCD(2147483648, false);
        b = convertDECtoBCD(2147483647, false);
        result = unsignedSub(a, b);
        expected = convertDECtoBCD(1, false);
        this->expectEq("a:2147483648 - b:2147483647 = 1", expected, result);

        a = convertDECtoBCD(0, false);
        b = convertDECtoBCD(0, false);
        result = unsignedSub(a, b);
        expected = convertDECtoBCD(0, false);
        this->expectEq("a:0 - b:0 = 0", expected, result);

        a = convertDECtoBCD(0, false);
        b = convertDECtoBCD(0, true);
        result = unsignedSub(a, b);
        expected = convertDECtoBCD(0, false);
        this->expectEq("a:0 - b:|-0| = 0", expected, result);

        a = convertDECtoBCD(1, false);
        b = convertDECtoBCD(1, false);
        result = unsignedSub(a, b);
        expected = convertDECtoBCD(0, false);
        this->expectEq("a:1 - b:1 = 0", expected, result);

        a = convertDECtoBCD(6, false);
        b = convertDECtoBCD(5, false);
        result = unsignedSub(a, b);
        expected = convertDECtoBCD(1, false);
        this->expectEq("a:6 - b:5 = 1", expected, result);

        a = convertDECtoBCD(99009, false);
        b = convertDECtoBCD(999, false);
        result = unsignedSub(a, b);
        expected = convertDECtoBCD(98010, false);
        this->expectEq("a:99009 - b:999 = 98010", expected, result);

        a = convertDECtoBCD(99009, true);
        b = convertDECtoBCD(999, false);
        result = unsignedSub(a, b);
        expected = convertDECtoBCD(98010, false);
        this->expectEq("a:|-99009| - b:999 = 98010", expected, result);

        a = convertDECtoBCD(99009, false);
        b = convertDECtoBCD(999, true);
        result = unsignedSub(a, b);
        expected = convertDECtoBCD(98010, false);
        this->expectEq("a:|99009| - b:|-999| = 98010", expected, result);

        a = convertDECtoBCD(99009, true);
        b = convertDECtoBCD(999, true);
        result = unsignedSub(a, b);
        expected = convertDECtoBCD(98010, false);
        this->expectEq("a:|-99009| - b:|-999| = 98010", expected, result);

        a = convertDECtoBCD(999, false);
        b = convertDECtoBCD(99009, false);
        result = unsignedSub(a, b);
        expected = convertDECtoBCD(98010, true);
        this->expectEq("a:999 - b:99009 = -98010", expected, result);

        a = convertDECtoBCD(999, true);
        b = convertDECtoBCD(99009, false);
        result = unsignedSub(a, b);
        expected = convertDECtoBCD(98010, true);
        this->expectEq("a:|-999| - b:99009 = -98010", expected, result);

        a = convertDECtoBCD(999, false);
        b = convertDECtoBCD(99009, true);
        result = unsignedSub(a, b);
        expected = convertDECtoBCD(98010, true);
        this->expectEq("a:|999| - b:|-99009| = -98010", expected, result);

        a = convertDECtoBCD(999, true);
        b = convertDECtoBCD(99009, true);
        result = unsignedSub(a, b);
        expected = convertDECtoBCD(98010, true);
        this->expectEq("a:|-999| - b:|-99009| = -98010", expected, result);
    }
};

class TEST_addOrSubForSignedBCD : public TestCodeBase<BCD *>
{
private:
    std::string _toString(BCD *x) override
    {
        char str[BCD_MAX_DIGITS + 1];
        convertBCDtoSTR(x, str, true, true);
        return std::string(str, BCD_MAX_DIGITS + 1);
    }
    bool _isEqual(BCD *a, BCD *b) override
    {
        if (bcdCompear(a, b) == COMPEAR_RESULT::EQUAL)
        {
            return true;
        }
        else
        {
            return false;
        }
    }

public:
    TEST_addOrSubForSignedBCD() : TestCodeBase<BCD *>("addOrSubForSignedBCD")
    {
    }

    void runTest() override
    {
        this->printTestStart();

        BCD *a;
        BCD *b;
        BCD *result;
        BCD *expected;

        a = convertDECtoBCD(100, false);
        b = convertDECtoBCD(100, false);
        result = addOrSubForSignedBCD(a, b, OperatorKind::OP_ADD);
        expected = convertDECtoBCD(200, false);
        this->expectEq("a:+N + b:+N = +2N", expected, result);

        a = convertDECtoBCD(100, false);
        b = convertDECtoBCD(100, true);
        result = addOrSubForSignedBCD(a, b, OperatorKind::OP_ADD);
        expected = convertDECtoBCD(0, false);
        this->expectEq("a:+N + b:-N = 0", expected, result);

        a = convertDECtoBCD(100, true);
        b = convertDECtoBCD(100, false);
        result = addOrSubForSignedBCD(a, b, OperatorKind::OP_ADD);
        expected = convertDECtoBCD(0, false);
        this->expectEq("a:-N + b:+N = 0", expected, result);

        a = convertDECtoBCD(100, true);
        b = convertDECtoBCD(100, true);
        result = addOrSubForSignedBCD(a, b, OperatorKind::OP_ADD);
        expected = convertDECtoBCD(200, true);
        this->expectEq("a:-N + b:-N = -2N", expected, result);

        a = convertDECtoBCD(100, false);
        b = convertDECtoBCD(100, false);
        result = addOrSubForSignedBCD(a, b, OperatorKind::OP_SUB);
        expected = convertDECtoBCD(0, false);
        this->expectEq("a:+N - b:+N = 0", expected, result);

        a = convertDECtoBCD(100, false);
        b = convertDECtoBCD(100, true);
        result = addOrSubForSignedBCD(a, b, OperatorKind::OP_SUB);
        expected = convertDECtoBCD(200, false);
        this->expectEq("a:+N - b:-N = +2N", expected, result);

        a = convertDECtoBCD(100, true);
        b = convertDECtoBCD(100, false);
        result = addOrSubForSignedBCD(a, b, OperatorKind::OP_SUB);
        expected = convertDECtoBCD(200, true);
        this->expectEq("a:-N - b:+N = -2N", expected, result);

        a = convertDECtoBCD(100, true);
        b = convertDECtoBCD(100, true);
        result = addOrSubForSignedBCD(a, b, OperatorKind::OP_SUB);
        expected = convertDECtoBCD(0, false);
        this->expectEq("a:-N - b:-N = 0", expected, result);
    }
};

void asertAllResult(std::vector<std::tuple<int, int, std::string>> results)
{
    // bool isAllPassed = true;

    for (auto result : results)
    {
        int passCase = std::get<0>(result);
        int faildCase = std::get<1>(result);
        int allCase = passCase + faildCase;
        std::string methodName = std::get<2>(result);

        if (faildCase == 0)
        {
            std::cout << "[\033[1;32m●\033[0m " << methodName << "] ok passed " << passCase << "/" << allCase << std::endl;
        }
        else
        {
            // isAllPassed = false;
            std::cout << "[\033[1;31m●\033[0m " << methodName << "] " << faildCase << "/" << allCase << " test is faild" << std::endl;
        }
    }
}

int main(int argc, char **argv)
{
    std::cout << "[Start unit testing]" << std::endl
              << std::endl;

    std::vector<std::tuple<int, int, std::string>> results;

    TEST_absCompear test_absCompear;
    test_absCompear.runTest();
    results.push_back(test_absCompear.getTestResult());

    TEST_bcdCompear test_bcdCompear;
    test_bcdCompear.runTest();
    results.push_back(test_bcdCompear.getTestResult());

    TEST_unsignedAdd test_unsignedAdd;
    test_unsignedAdd.runTest();
    results.push_back(test_unsignedAdd.getTestResult());

    TEST_unsignedSub test_unsignedSub;
    test_unsignedSub.runTest();
    results.push_back(test_unsignedSub.getTestResult());

    TEST_addOrSubForSignedBCD test_addOrSubForSignedBCD;
    test_addOrSubForSignedBCD.runTest();
    results.push_back(test_addOrSubForSignedBCD.getTestResult());

    asertAllResult(results);
    std::cout << std::endl;

    return 0;
}