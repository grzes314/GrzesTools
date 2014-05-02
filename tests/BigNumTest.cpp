#include <gtest/gtest.h>

#include "headers/bignum.h"

namespace {

using namespace grzes;
using namespace std;

class BigNumTest : public ::testing::Test
{
 protected:
  BigNumTest() {}

  virtual ~BigNumTest() {}

  virtual void SetUp() {}

  virtual void TearDown() {}

  BigNum zero = 0;
  BigNum millionOne = 1000000001;
  BigNum negMillionOne = -1000000001;
  BigNum millionTwo = 1000000002;
  BigNum millionsSum = 2000000003;
  BigNum millionsMult = 1000000003000000002LL;
};

TEST_F(BigNumTest, ToIntWorksWithSmallInts)
{
    BigNum a(5);
    ASSERT_EQ(5, a.toInt());
}

TEST_F(BigNumTest, SignumWorks)
{
    EXPECT_EQ(0, BigNum(0).signum());
    EXPECT_EQ(-1, BigNum(-20).signum());
    EXPECT_EQ(1, BigNum(230).signum());
    EXPECT_EQ(0, BigNum("0").signum());
    EXPECT_EQ(-1, BigNum("-20").signum());
    EXPECT_EQ(1, BigNum("230").signum());
}

TEST_F(BigNumTest, DisplayWorks)
{
    EXPECT_EQ(string("1000000001"), millionOne.getDisplay());
    EXPECT_EQ(string("-1000000001"), negMillionOne.getDisplay());
    EXPECT_EQ(string("0"), zero.getDisplay());
    EXPECT_EQ(string("1000000003000000002"), millionsMult.getDisplay());
}

TEST_F(BigNumTest, ParsingOfValidStringsWorks)
{
    BigNum b = BigNum::parse("+1000000001");
    EXPECT_EQ(string("1000000001"), b.getDisplay());
    b = BigNum::parse("-1000000001");
    EXPECT_EQ(string("-1000000001"), b.getDisplay());
    b = BigNum::parse("0");
    EXPECT_EQ(string("0"), b.getDisplay());
    b = BigNum::parse("-0");
    EXPECT_EQ(string("0"), b.getDisplay());
    b = BigNum::parse("1000000003000000002");
    EXPECT_EQ(string("1000000003000000002"), b.getDisplay());
}

TEST_F(BigNumTest, ParsingThrowsOnInvalidStrings)
{
    EXPECT_THROW(BigNum::parse(""), BigNumException);
    EXPECT_THROW(BigNum::parse("-"), BigNumException);
    EXPECT_THROW(BigNum::parse("+"), BigNumException);
    EXPECT_THROW(BigNum::parse("12.31"), BigNumException);
    EXPECT_THROW(BigNum::parse("--2312"), BigNumException);
    EXPECT_THROW(BigNum::parse("84957-4259"), BigNumException);
}

}  // namespace
