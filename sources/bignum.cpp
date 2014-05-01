#include "headers/bignum.h"
#include <cstring>

namespace grzes
{
using std::vector;

template <typename inttype>
void BigNum::setSign(inttype number)
{
    if (number <= 0)
    {
        if (number < 0)
            sign = Sign::MINUS;
        else
            sign = Sign::ZERO;
    }
    else
    {
        sign = Sign::PLUS;
    }
}

template <typename inttype>
void BigNum::setFromNumber(inttype number)
{
    if (number < 0)
        throw "BigNum::setFromNumber -> internal error. Argument should be greater or equal zero.";
    Digit fst = number % base;
    Digit snd = number / base;
    Digit thd = snd / base;
    digits.push_back(fst);
    if (snd > 0)
        digits.push_back(snd);
    if (thd > 0)
        digits.push_back(thd);
}

template <typename inttype>
inttype abs(inttype n)
{
    return (n >= 0 ? n : -n);
}

BigNum::BigNum()
    : BigNum(0)
{
}

BigNum::BigNum(int number)
{
    setSign(number);
    setFromNumber( abs(number) );
}

BigNum::BigNum(long number)
{
    setSign(number);
    setFromNumber( abs(number) );
}

BigNum::BigNum(long long number)
{
    setSign(number);
    setFromNumber( abs(number) );
}

BigNum::BigNum(BigNum&& original)
    : digits(std::move(original.digits))
    , sign(original.sign)
{
}

BigNum::BigNum(const char * str)
    : BigNum(parse(str))
{
}

BigNum BigNum::parse(const char * str)
{
    size_t n = strlen(str);
    unsigned int i = 0;
    Sign sign = Sign::PLUS;
    try {
        sign = parseSign(str, n, i);
        i++;
    } catch (BigNumException& ex) {}
    const vector<Digit> digits = parseDigits(str, n, i);
    return BigNum(sign, digits);
}

BigNum::Sign BigNum::parseSign(const char * str, size_t strLen, unsigned int i)
{
    if (i >= strLen)
        throw BigNumException("BigNum::parseSign -> Parse error.");
    if (str[i] == '+' || str[i] == '-')
        return (str[i] == '+' ? Sign::PLUS : Sign::MINUS);
    throw BigNumException("BigNum::parseSign -> Parse error.");
}

int char2int(char c)
{
    return c - '0';
}

vector<BigNum::Digit> BigNum::parseDigits(const char * str, size_t strLen, unsigned int digitsBeg)
{
    vector<Digit> digits;
    unsigned int j = strLen-1;
    while (j >= digitsBeg)
    {
        Digit d = parseDigit(str, digitsBeg, j);
        digits.push_back(d);
        j -= 9;
    }
    return digits;
}


BigNum::Digit BigNum::parseDigit(const char * str, unsigned int digitsBeg, unsigned int nextDigitEnd)
{
    Digit d = 0;
    for (int i = 0; i < 9 && digitsBeg <= nextDigitEnd-i; ++i)
    {
        int c = char2int(str[i]);
        if (c < 0 || c > 9)
            throw BigNumException("BigNum::parseDigit -> parse error. Not a digit.");
        d *= 10;
        d += c;
    }
    return d;
}

int BigNum::toInt() const
{
    if (digits.size() > 1)
        throw BigNumException("BigNum::toInt() -> Absolute value of number exceeds billion");
    return signum() * digits[0];
}

int BigNum::signum() const
{
    switch (sign)
    {
        case Sign::MINUS:
            return -1;
        case Sign::PLUS:
            return 1;
        default:
            return 0;
    }
}

BigNum::BigNum(BigNum::Sign s, std::vector<BigNum::Digit> ds)
    : digits(ds)
    , sign(s)
{
    ensureFormIsCorrect();
}

void BigNum::removeLeadingZeros()
{
    Digit d = digits[digits.size()-1];
    while (d == 0)
    {
        digits.pop_back();
        d = digits[digits.size()-1];
    }
    ensureDigitsNotEmpty();
}

void BigNum::ensureFormIsCorrect()
{
    ensureZeroIsOneDigit();
    ensureDigitsNotEmpty();
    removeLeadingZeros();
}

void BigNum::ensureDigitsNotEmpty()
{
    if (digits.empty())
    {
        digits.push_back(0);
        sign = Sign::ZERO;
    }
}

void BigNum::ensureZeroIsOneDigit()
{
    if (sign == Sign::ZERO)
    {
        digits.clear();
        digits.push_back(0);
    }
}


} //namespace grzes
