#ifndef BIGNUM_H
#define BIGNUM_H

#include <vector>
#include <cstdint>
#include "headers/BigNumException.h"

namespace grzes
{

class BigNum
{
    typedef uint32_t Digit;
public:
    enum class Sign {
        PLUS, ZERO, MINUS
    };
    BigNum();
    BigNum(int number);
    BigNum(long number);
    BigNum(long long number);
    BigNum(BigNum&& original);
    BigNum(const char *str);
    static BigNum parse(const char *str);
    static Sign parseSign(const char *str, size_t strLen, unsigned int i);
    static std::vector<Digit> parseDigits(const char *str, size_t strLen, unsigned int digitsBeg);
    static Digit parseDigit(const char * str, unsigned int digitsBeg, unsigned int nextDigitEnd);
    int toInt() const;
    int signum() const;
private:

    BigNum(Sign s, std::vector<Digit> ds);

    void removeLeadingZeros();
    void ensureFormIsCorrect();
    void ensureDigitsNotEmpty();
    void ensureZeroIsOneDigit();

    template <typename inttype>
    void setSign(inttype number);

    template <typename uinttype>
    void setFromNumber(uinttype number);

    std::vector<Digit> digits;
    Sign sign;
    static const Digit base = 1000000000;
};

} // namespace grzes

#endif // BIGNUM_H
