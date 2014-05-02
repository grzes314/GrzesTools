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

TEST_F(BigNumTest, NegateWorks)
{
    millionOne.negate();
    zero.negate();
    negMillionOne.negate();
    EXPECT_EQ(-1, millionOne.signum());
    EXPECT_EQ(0, zero.signum());
    EXPECT_EQ(1, negMillionOne.signum());

}

TEST_F(BigNumTest, getNegatedWorks)
{
    EXPECT_EQ(negMillionOne, millionOne.getNegated());
    EXPECT_EQ(0, zero.getNegated().signum());
    EXPECT_EQ(1, negMillionOne.getNegated().signum());

}

TEST_F(BigNumTest, EqOperatorWorks)
{
    EXPECT_TRUE(2 == BigNum(2));
    EXPECT_TRUE(-2 == BigNum(-2));
    EXPECT_FALSE(2 == BigNum(-2));
    EXPECT_FALSE(-2 == BigNum(2));
    EXPECT_TRUE(BigNum("2") == BigNum(2));
    EXPECT_TRUE(millionsMult == "1000000003000000002");
    EXPECT_FALSE(negMillionOne == "1000000001");
    EXPECT_TRUE(BigNum() == 0);
}

TEST_F(BigNumTest, PlusOperatorWorks)
{
    EXPECT_EQ(0, negMillionOne + millionOne);
    EXPECT_EQ(millionsSum, millionOne + millionTwo);
    EXPECT_EQ(-5, BigNum(-10) + 5);
    EXPECT_EQ(-5, BigNum(5) + (-10));
    EXPECT_EQ(5, BigNum(-5) + 10);
    EXPECT_EQ(millionsMult, millionsMult + 0);
}

TEST_F(BigNumTest, OperatorLessWorks)
{
    EXPECT_FALSE(BigNum(-5) < -5);
    EXPECT_TRUE(BigNum(-5) < 5);
    EXPECT_FALSE(BigNum(5) < -5);
    EXPECT_FALSE(BigNum(5) < 5);

    EXPECT_FALSE(BigNum(0) < -5);
    EXPECT_TRUE(BigNum(0) < 5);
    EXPECT_TRUE(BigNum(-5) < 0);
    EXPECT_FALSE(BigNum(5) < 0);
    EXPECT_FALSE(BigNum(0) < 0);
}

TEST_F(BigNumTest, OperatorLessEqWorks)
{
    EXPECT_TRUE(BigNum(-5) <= -5);
    EXPECT_TRUE(BigNum(-5) <= 5);
    EXPECT_FALSE(BigNum(5) <= -5);
    EXPECT_TRUE(BigNum(5) <= 5);

    EXPECT_FALSE(BigNum(0) <= -5);
    EXPECT_TRUE(BigNum(0) <= 5);
    EXPECT_TRUE(BigNum(-5) <= 0);
    EXPECT_FALSE(BigNum(5) <= 0);
    EXPECT_TRUE(BigNum(0) <= 0);
}

TEST_F(BigNumTest, OperatorGreaterWorks)
{
    EXPECT_FALSE(BigNum(-5) > -5);
    EXPECT_FALSE(BigNum(-5) > 5);
    EXPECT_TRUE(BigNum(5) > -5);
    EXPECT_FALSE(BigNum(5) > 5);

    EXPECT_TRUE(BigNum(0) > -5);
    EXPECT_FALSE(BigNum(0) > 5);
    EXPECT_FALSE(BigNum(-5) > 0);
    EXPECT_TRUE(BigNum(5) > 0);
    EXPECT_FALSE(BigNum(0) > 0);
}

TEST_F(BigNumTest, OperatorGreaterEqWorks)
{
    EXPECT_TRUE(BigNum(-5) >= -5);
    EXPECT_FALSE(BigNum(-5) >= 5);
    EXPECT_TRUE(BigNum(5) >= -5);
    EXPECT_TRUE(BigNum(5) >= 5);

    EXPECT_TRUE(BigNum(0) >= -5);
    EXPECT_FALSE(BigNum(0) >= 5);
    EXPECT_FALSE(BigNum(-5) >= 0);
    EXPECT_TRUE(BigNum(5) >= 0);
    EXPECT_TRUE(BigNum(0) >= 0);
}


}  // namespace
