#include <gtest/gtest.h>

#include "headers/bignum.h"

namespace {

using namespace grzes;

class BigNumTest : public ::testing::Test
{
 protected:
  BigNumTest() {}

  virtual ~BigNumTest() {}

  virtual void SetUp() {}

  virtual void TearDown() {}
};

// Tests that the Foo::Bar() method does Abc.
TEST_F(BigNumTest, ToIntWorksWithSmallInts)
{
    BigNum a(5);
    ASSERT_EQ(5, a.toInt());
}
//1 000 000 003 000 000 002 = 1 000 000 001 * 1 000 000 002
}  // namespace
