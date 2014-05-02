#include "headers/bignum.h"
#include "headers/bignumparseexception.h"
#include <cstring>

namespace grzes
{
using std::vector;
using std::string;

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
    do
    {
        Digit d = number % base;
        digits.push_back(d);
        number /= base;
    } while (number > 0);
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
    int i = 0;
    Sign sign = Sign::PLUS;
    try {
        sign = parseSign(str, n, i);
        i++;
    } catch (const BigNumException&) {}
    const vector<Digit> digits = parseDigits(str, n, i);
    if (digits.empty())
        throw BigNumParseException("BigNum::parse -> Parse error. No digits in string.");
    return BigNum(sign, digits);
}

BigNum::Sign BigNum::parseSign(const char * str, int strLen, int i)
{
    if (i >= strLen)
        throw BigNumParseException("BigNum::parseSign -> Parse error.");
    if (str[i] == '+' || str[i] == '-')
        return (str[i] == '+' ? Sign::PLUS : Sign::MINUS);
    throw BigNumParseException("BigNum::parseSign -> Parse error.");
}

int char2int(char c)
{
    return c - '0';
}

vector<BigNum::Digit> BigNum::parseDigits(const char * str, size_t strLen, int digitsBeg)
{
    vector<Digit> digits;
    int j = strLen-1;
    while (j >= digitsBeg)
    {
        Digit d = parseDigit(str, digitsBeg, j);
        digits.push_back(d);
        j -= 9;
    }
    return digits;
}


BigNum::Digit BigNum::parseDigit(const char * str, int digitsBeg, int nextDigitEnd)
{
    Digit d = 0;
    int nextDigitBeg = nextDigitEnd - 8;
    if (nextDigitBeg < digitsBeg)
        nextDigitBeg = digitsBeg;
    int iterations = nextDigitEnd - nextDigitBeg + 1;
    for (int i = 0; i < iterations; ++i)
    {
        int c = char2int(str[nextDigitBeg + i]);
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

string BigNum::getDisplay() const
{
    vector<char> vec = getDisplayAsVector();
    return string(vec.begin(), vec.end());
}

BigNum::BigNum(BigNum::Sign s, vector<BigNum::Digit> ds)
    : digits(ds)
    , sign(s)
{
    ensureFormIsCorrect();
}

void BigNum::removeLeadingZeros()
{
    Digit d = digits[digits.size()-1];
    while (digits.size() > 1 && d == 0)
    {
        digits.pop_back();
        d = digits[digits.size()-1];
    }
}

void BigNum::ensureFormIsCorrect()
{
    ensureZeroHasProperForm();
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

void BigNum::ensureZeroHasProperForm()
{
    if (sign == Sign::ZERO)
    {
        digits.clear();
        digits.push_back(0);
    }
    else if (digits.size() == 1 && digits[0] == 0)
    {
        sign = Sign::ZERO;
    }
}

std::vector<char> BigNum::getDisplayAsVector() const
{
    vector<char> res;
    if (sign == Sign::MINUS)
        res.push_back('-');
    for (auto it = digits.rbegin(); it != digits.rend(); it++)
    {
        bool withLeadingZeros = it != digits.rbegin();
        vector<char> d = getDigitReversed(*it, withLeadingZeros);
        res.insert(res.end(), d.rbegin(), d.rend());
    }
    return res;
}

char int2char(int c)
{
    return c + '0';
}

std::vector<char> BigNum::getDigitReversed(BigNum::Digit d, bool withLeandingZeros) const
{
    vector<char> res;
    for (int i = 0; i < 9; ++i)
    {
        int c = d % 10;
        d /= 10;
        res.push_back(int2char(c));
        if (d == 0 && !withLeandingZeros)
            break;
    }
    return res;
}


} //namespace grzes
