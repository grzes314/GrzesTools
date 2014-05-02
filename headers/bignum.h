#ifndef BIGNUM_H
#define BIGNUM_H

#include <vector>
#include <cstdint>
#include "headers/bignumexception.h"

namespace grzes
{

class BigNum
{
public:
    typedef uint32_t Digit;
    enum class Sign {
        PLUS, ZERO, MINUS
    };
    BigNum();
    BigNum(int number);
    BigNum(long number);
    BigNum(long long number);
    BigNum(BigNum&& original);
    BigNum(const char *str);
    BigNum(const BigNum&) = default;

    BigNum& operator=(const BigNum&) = default;
    friend BigNum operator+(const BigNum& a, const BigNum& b);
    friend bool operator==(const BigNum& a, const BigNum& b);
    friend bool operator<(const BigNum& a, const BigNum& b);
    friend bool operator<=(const BigNum& a, const BigNum& b);
    friend bool operator>(const BigNum& a, const BigNum& b);
    friend bool operator>=(const BigNum& a, const BigNum& b);

    int toInt() const;
    int signum() const;
    void negate();
    BigNum getNegated() const;
    std::string getDisplay() const;

    static BigNum parse(const char *str);
private:

    BigNum(Sign s, std::vector<Digit>&& ds);

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

    bool absGreater(const BigNum& other, bool allowEq) const;

    static Sign parseSign(const char *str, int strLen, int i);
    static std::vector<Digit> parseDigits(const char *str, size_t strLen, int digitsBeg);
    static Digit parseDigit(const char * str, int digitsBeg, int nextDigitEnd);
    static std::vector<Digit> addDigits(const std::vector<Digit>& a, const std::vector<Digit>& b);
    static std::vector<Digit> subtractDigits(const std::vector<Digit>& gr, const std::vector<Digit>& sm);

    std::vector<Digit> digits;
    Sign sign;
    static const Digit base = 1000000000;
};

} // namespace grzes

#endif // BIGNUM_H
