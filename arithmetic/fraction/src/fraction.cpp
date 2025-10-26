#include <cmath>

#include "../include/fraction.h"

fraction::fraction(
    big_integer &&numerator,
    big_integer &&denominator):
        _numerator(std::move(numerator)),
        _denominator(std::move(denominator))
{
    simplify();
}

fraction::fraction(
    std::string const &numerator,
    std::string const &denominator):
        _numerator(numerator),
        _denominator(denominator)
{
    simplify();
}

fraction fraction::operator-() const
{
    return fraction(-big_integer(_numerator), big_integer(_denominator));
}


fraction &fraction::operator+=(
    fraction const &other)
{
    fraction tmp(other);
    
    if (_denominator != other._denominator)
    {
        equalize_denominators(*this, tmp);
    }
    
    _numerator += tmp._numerator;
    simplify();
    
    return *this;
}

fraction fraction::operator+(
    fraction const &other) const
{
    return fraction(*this) += other;
}

fraction &fraction::operator-=(
    fraction const &other)
{
    fraction tmp(other);
    
    if (_denominator != other._denominator)
    {
        equalize_denominators(*this, tmp);
    }
    
    _numerator -= tmp._numerator;
    simplify();
    
    return *this;
}

fraction fraction::operator-(
    fraction const &other) const
{
    return fraction(*this) -= other;
}

fraction &fraction::operator*=(
    fraction const &other)
{
    _numerator *= other._numerator;
    _denominator *= other._denominator;
    
    simplify();
    
    return *this;
}

fraction fraction::operator*(
    fraction const &other) const
{
    return fraction(*this) *= other;
}

fraction &fraction::operator/=(
    fraction const &other)
{
    _numerator *= other._denominator;
    _denominator *= other._numerator;
    
    simplify();
    
    return *this;
}

fraction fraction::operator/(
    fraction const &other) const
{
    return fraction(*this) /= other;
}

bool fraction::operator==(
    fraction const &other) const
{
    if (_denominator != other._denominator)
    {
        return false;
    }
    
    return _numerator == other._numerator;
}

bool fraction::operator!=(
    fraction const &other) const
{
    return !(*this == other);
}

bool fraction::operator>=(
    fraction const &other) const
{
    fraction tmp_a(*this);
    fraction tmp_b(other);
    
    equalize_denominators(tmp_a, tmp_b);
    
    return tmp_a._numerator >= tmp_b._numerator;
}

bool fraction::operator>(
    fraction const &other) const
{
    return !(*this <= other);
}

bool fraction::operator<=(
    fraction const &other) const
{
    fraction tmp_a(*this);
    fraction tmp_b(other);
    
    equalize_denominators(tmp_a, tmp_b);
    
    return tmp_a._numerator <= tmp_b._numerator;
}

bool fraction::operator<(
    fraction const &other) const
{
    return !(*this >= other);
}

std::ostream &operator<<(
    std::ostream &stream,
    fraction const &obj)
{
    return stream << obj._numerator << '/' << obj._denominator;
}

std::istream &operator>>(
    std::istream &stream,
    fraction &obj)
{
    std::string str, numerator, denominator;
    stream >> str;
    
    size_t pos = str.find('/');
    
    numerator = str.substr(0, pos);
    denominator = str.substr(pos + 1);
    
    obj._numerator = big_integer(numerator);
    obj._denominator = big_integer(numerator);
    
    return stream;
}

fraction fraction::sin(
    fraction const &epsilon) const
{
    fraction zero("0", "1");
    fraction one("1", "1");
    fraction two("2", "1");
    
    if (epsilon <= zero)
    {
        throw std::logic_error("Epsilon must be greater than 0");
    }

    fraction const &x = *this;
    fraction current = x;
    fraction sum = current;
    fraction n = one;
    
    do
    {
        fraction tmp = two * n;
        current *= -(x * x) / (tmp * (tmp + one));;
        sum += current;
        n += one;
    }
    while(current.abs() >= epsilon);
    
    return sum;
}

fraction fraction::cos(
    fraction const &epsilon) const
{
    fraction zero("0", "1");
    fraction one("1", "1");
    fraction two("2", "1");
    
    if (epsilon <= zero)
    {
        throw std::logic_error("Epsilon must be greater than 0");
    }

    fraction const &x = *this;
    fraction current = one;
    fraction sum = current;
    fraction n = one;
    do
    {
        fraction tmp = two * n;
        current *=  -(x * x) / (tmp * (tmp - one));
        sum += current;
        n += one;
    }
    while(current.abs() >= epsilon);
    
    return sum;
}

fraction fraction::tg(
    fraction const &epsilon) const
{
    return this->sin(epsilon) / this->cos(epsilon);
}

fraction fraction::ctg(
    fraction const &epsilon) const
{
    return this->cos(epsilon) / this->sin(epsilon);
}

fraction fraction::sec(
    fraction const &epsilon) const
{
    return cos(epsilon).inverse();
}

fraction fraction::cosec(
    fraction const &epsilon) const
{
    return sin(epsilon).inverse();
}

fraction fraction::arcsin(
    fraction const &epsilon) const
{
    fraction zero("0", "1");
    fraction one("1", "1");
    fraction two("2", "1");
    
    if (epsilon <= zero)
    {
        throw std::logic_error("Epsilon must greater than 0");
    }
    
    if (abs() > one)
    {
        throw std::logic_error("Module of number must be not greater than 1");
    }
    
    if (abs() > fraction("4", "5"))
    {
        return pi() / two - (one - pow(2)).root(2,epsilon).arcsin(epsilon);
    }
    
    fraction const &x = *this;
    fraction current = x;
    fraction sum = current;
    fraction n = one;
    
    do
    {
        fraction tmp = two * n;
        current *= (tmp - one).pow(2) * x * x / (tmp * (tmp + one));
        sum += current;
        n += one;
    }
    while(current.abs() >= epsilon);
    
    return sum;
}

fraction fraction::arccos(
    fraction const &epsilon) const
{
    return pi() / fraction("2", "1") - arcsin(epsilon);
}

fraction fraction::arctg(
    fraction const &epsilon) const
{
    return (*this / ((fraction("1", "1")) + pow(2)).root(2, epsilon)).arcsin(epsilon);
}

fraction fraction::arcctg(
    fraction const &epsilon) const
{
    return (*this / ((fraction("1", "1")) + pow(2)).root(2, epsilon)).arccos(epsilon);
}

fraction fraction::arcsec(
    fraction const &epsilon) const
{
    if (abs() < fraction("1", "1"))
    {
        throw std::logic_error("Module of number must be not lesser than 1");
    }
    
    return inverse().arccos(epsilon);
}

fraction fraction::arccosec(
    fraction const &epsilon) const
{
    if (abs() < fraction("1", "1"))
    {
        throw std::logic_error("Module of number must be not lesser than 1");
    }
    
    return inverse().arcsin(epsilon);
}

fraction fraction::pow(
    size_t degree) const
{
    fraction zero("0", "1");
    fraction one("1", "1");
    
    if (*this == zero && degree == 0)
    {
        throw std::logic_error("Attempt to raise zero in power zero");
    }
    
    if (degree == 0)
    {
        return one;
    }
    
    fraction value = *this;
    fraction tmp_res = one;
    
    while (degree > 0)
    {
        if (degree & 1)
        {
            tmp_res *= value;
        }
        
        value *= value;
        degree /= 2;
    }
    
    return tmp_res;
}

fraction fraction::root(
    size_t degree,
    fraction const &epsilon) const
{
    if (degree == 0)
    {
        throw std::logic_error("Root degree cannot be zero");
    }
    if (degree == 1)
    {
        return *this;
    }

    fraction zero("0", "1");
    fraction one("1", "1");
    fraction two("2", "1");
    fraction fraction_degree(big_integer(std::to_string(degree)), big_integer("1"));
    fraction tmp_fraction = *this;


    int sign = 1;
    int l = 0;
    
    if (tmp_fraction < zero)
    {
        if (degree & 1)
        {
            sign = -1;
            tmp_fraction *= -one;
        }
        else
        {
            throw std::logic_error("Cannot take odd degree root of negative number");
        }
    }

    fraction current = (one + tmp_fraction) / two;
    fraction next = current;

    fraction one_by_n(big_integer("1"), big_integer(std::to_string(degree)));

    fraction prev_degree = fraction_degree - one;

    do
    {
        current = next;
        next = one_by_n * (prev_degree * current + tmp_fraction / current.pow(degree - 1));
    }
    while ((next - current).abs() >= epsilon);

    if (sign == -1)
    {
        return -next;
    }
    return next;
}

fraction fraction::log2(
    fraction const &epsilon) const
{
    return ln(epsilon) / fraction("2", "1").ln(epsilon);
}

fraction fraction::ln(
    fraction const &epsilon) const
{
    fraction zero("0", "1");
    fraction one("1", "1");
    fraction two("2", "1");
    fraction ten("10", "1");
    
    if (*this <= zero)
    {
        throw std::logic_error("Cannot take logarithm of non-positive number");
    }
    
    if (*this > two)
    {
        fraction m = one;
        fraction dividend = *this;
        
        while ((dividend /= two) > one)
        {
            m += one;
        }
        
        return dividend.ln(epsilon) + m * two.ln(epsilon);
    }
    
    if (epsilon <= zero)
    {
        throw std::logic_error("Epsilon must be greater than 0");
    }

    fraction x = (*this - one) / (*this + one);

    fraction current = one;
    fraction next = current;
    fraction sum = x;
    fraction n = one;
    
    do
    {
        current *= x * x;
        n += two;
        next = current / n;
        sum += next;
    }
    while((current - next).abs() >= epsilon);

    sum *= two;
    
    return sum;
}

fraction fraction::lg(
    fraction const &epsilon) const
{
    return ln(epsilon) / fraction("10", "1").ln(epsilon);
}

fraction fraction::abs() const
{
    return fraction(_numerator.abs(), _denominator.abs());
}

fraction fraction::inverse() const
{
    return fraction(big_integer(_denominator), big_integer(_numerator));
}

void fraction::equalize_denominators(fraction &a, fraction &b)
{
    big_integer tmp = a._denominator;
    
    a._numerator *= b._denominator;
    a._denominator *= b._denominator;
    
    b._numerator *= tmp;
    b._denominator *= tmp;
}
    
void fraction::simplify()
{
    big_integer gcd = big_integer::gcd(_numerator, _denominator);
    
    
    _numerator /= gcd;
    _denominator /= gcd;
    
    if (_denominator < 0)
    {
        _denominator = -_denominator;
        _numerator = -_numerator;
    }
}

fraction fraction::pi()
{
    double pi = M_PI;
    pi *= 100000000;
    int p_i = floor(pi);
    return fraction(big_integer(std::vector<int>{p_i}), big_integer(std::vector<int>{100000000}));
}