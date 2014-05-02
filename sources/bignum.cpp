#include "headers/bignum.h"
#include "headers/bignumparseexception.h"
#include <cstring>

namespace grzes
{
using std::vector;
using std::string;

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

BigNum operator+(const BigNum &a, const BigNum &b)
{
    if (a == 0)
        return b;
    else if (b == 0)
        return a;
    else if (a.sign == b.sign)
    {
        vector<BigNum::Digit>&& digits = BigNum::addDigits(a.digits, b.digits);
        return BigNum(a.sign, std::move(digits));
    }
    else
    {
        if (a.absGreater(b, true /* allow equality*/))
        {
            vector<BigNum::Digit>&& digits = BigNum::subtractDigits(a.digits, b.digits);
            return BigNum(a.sign, std::move(digits));
        }
        else
        {
            vector<BigNum::Digit>&& digits = BigNum::subtractDigits(b.digits, a.digits);
            return BigNum(b.sign, std::move(digits));
        }
    }
}

bool operator==(const BigNum &a, const BigNum &b)
{
    return a.sign == b.sign && a.digits == b.digits;
}

bool operator<(const BigNum &a, const BigNum &b)
{
    switch (a.sign)
    {
    case BigNum::Sign::MINUS:
        if (b.sign == BigNum::Sign::MINUS)
            return a.absGreater(b, false);
        else return true;
    case BigNum::Sign::ZERO:
        return b.sign == BigNum::Sign::PLUS;
    case BigNum::Sign::PLUS:
        if (b.sign == BigNum::Sign::PLUS)
            return b.absGreater(a, false);
        else return false;
    default:
        throw BigNumException("operator< -> Internal error. Program flow should not reach this statement.");
    }
}

bool operator<=(const BigNum &a, const BigNum &b)
{
    switch (a.sign)
    {
    case BigNum::Sign::MINUS:
        if (b.sign == BigNum::Sign::MINUS)
            return a.absGreater(b, true);
        else return true;
    case BigNum::Sign::ZERO:
        return b.sign != BigNum::Sign::MINUS;
    case BigNum::Sign::PLUS:
        if (b.sign == BigNum::Sign::PLUS)
            return b.absGreater(a, true);
        else return false;
    default:
        throw BigNumException("operator< -> Internal error. Program flow should not reach this statement.");
    }
}

bool operator>(const BigNum &a, const BigNum &b)
{
    switch (a.sign)
    {
    case BigNum::Sign::MINUS:
        if (b.sign == BigNum::Sign::MINUS)
            return b.absGreater(a, false);
        else return false;
    case BigNum::Sign::ZERO:
        return b.sign == BigNum::Sign::MINUS;
    case BigNum::Sign::PLUS:
        if (b.sign == BigNum::Sign::PLUS)
            return a.absGreater(b, false);
        else return true;
    default:
        throw BigNumException("operator< -> Internal error. Program flow should not reach this statement.");
    }
}

bool operator>=(const BigNum &a, const BigNum &b)
{
    switch (a.sign)
    {
    case BigNum::Sign::MINUS:
        if (b.sign == BigNum::Sign::MINUS)
            return b.absGreater(a, true);
        else return false;
    case BigNum::Sign::ZERO:
        return b.sign != BigNum::Sign::PLUS;
    case BigNum::Sign::PLUS:
        if (b.sign == BigNum::Sign::PLUS)
            return a.absGreater(b, true);
        else return true;
    default:
        throw BigNumException("operator< -> Internal error. Program flow should not reach this statement.");
    }
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

void BigNum::negate()
{
    switch (sign)
    {
    case Sign::MINUS:
        sign = Sign::PLUS;
        break;
    case Sign::PLUS:
        sign = Sign::MINUS;
        break;
    default:
        break;
    }
}

BigNum BigNum::getNegated() const
{
    BigNum res = *this;
    res.negate();
    return res;
}

string BigNum::getDisplay() const
{
    vector<char> vec = getDisplayAsVector();
    return string(vec.begin(), vec.end());
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
    vector<Digit>&& digits = parseDigits(str, n, i);
    if (digits.empty())
        throw BigNumParseException("BigNum::parse -> Parse error. No digits in string.");
    return BigNum(sign, std::move(digits));
}

/******************************************************************************/
/*************************** PRIVATE METHODS **********************************/
/******************************************************************************/

BigNum::BigNum(BigNum::Sign s, vector<BigNum::Digit>&& ds)
    : digits(ds)
    , sign(s)
{
    ensureFormIsCorrect();
}

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
    removeLeadingZeros();
    ensureDigitsNotEmpty();
    ensureZeroHasProperForm();
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

bool BigNum::absGreater(const BigNum &other, bool allowEq) const
{
    if (digits.size() != other.digits.size())
        return digits.size() > other.digits.size();
    else
    {
        for (int i = digits.size()-1; i >= 0; --i)
        {
            if (digits[i] != other.digits[i])
                return digits[i] > other.digits[i];
        }
        return allowEq; // numbers are equal
    }
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

template<typename inttype>
inttype max(inttype a, inttype b)
{
    return (a > b ? a : b);
}

inline BigNum::Digit safeAt(const vector<BigNum::Digit> &a, unsigned int i)
{
    if (i >= a.size())
        return 0;
    else
        return a[i];
}

vector<BigNum::Digit> BigNum::addDigits(const vector<BigNum::Digit> &a, const vector<BigNum::Digit> &b)
{
    unsigned int l = max(a.size(), b.size());
    int carry = 0;
    vector<BigNum::Digit> res;
    for (unsigned int i = 0; i < l; ++i)
    {
        Digit d = safeAt(a, i) + safeAt(b, i) + carry;
        if (d > base)
        {
            carry = 1;
            d -= base;
        }
        else
        {
            carry = 0;
        }
        res.push_back(d);
    }
    if (carry > 0)
        res.push_back(carry);
    return res;
}

std::vector<BigNum::Digit> BigNum::subtractDigits(const std::vector<BigNum::Digit> &gr, const std::vector<BigNum::Digit> &sm)
{
    int borrow = 0;
    vector<BigNum::Digit> res;
    for (unsigned int i = 0; i < gr.size(); ++i)
    {
        Digit d;
        if (gr[i] < borrow + safeAt(sm, i))
        {
            borrow = 1;
            d = gr[i] + base - borrow - safeAt(sm, i);
        }
        else
        {
            borrow = 0;
            d = gr[i] - borrow - safeAt(sm, i);
        }
        res.push_back(d);
    }
    return res;
}

} //namespace grzes
