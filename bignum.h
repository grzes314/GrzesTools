#ifndef BIGNUM_H
#define BIGNUM_H

namespace grzes
{

class BigNum
{
public:
    BigNum();
    explicit BigNum(int);
    int toInt() const;
};

} // namespace grzes

#endif // BIGNUM_H
