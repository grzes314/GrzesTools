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
    BigNum& operator=(const BigNum&) = default;
    static BigNum parse(const char *str);
    static Sign parseSign(const char *str, int strLen, int i);
    static std::vector<Digit> parseDigits(const char *str, size_t strLen, int digitsBeg);
    static Digit parseDigit(const char * str, int digitsBeg, int nextDigitEnd);
    int toInt() const;
    int signum() const;
    std::string getDisplay() const;
private:

    BigNum(Sign s, std::vector<Digit> ds);

    template <typename inttype>
    void setSign(inttype number);

    template <typename uinttype>
    void setFromNumber(uinttype number);

    void removeLeadingZeros();
    void ensureFormIsCorrect();
    void ensureDigitsNotEmpty();
    void ensureZeroHasProperForm();

    std::vector<char> getDisplayAsVector() const;
    std::vector<char> getDigitReversed(Digit d, bool withLeandingZeros) const;

    std::vector<Digit> digits;
    Sign sign;
    static const Digit base = 1000000000;
};

} // namespace grzes

#endif // BIGNUM_H
