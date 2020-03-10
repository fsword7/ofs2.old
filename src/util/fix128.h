/*
 * int128.h - 128-bit fixed point/integer package
 *
 * 128-bit fixed point (96.32) provides large celestial space
 * for universal coordinate system within billions of light year coverage
 * and keeping nanometer accuracy. Double-precision floating is
 * inadequate for accuracy.
 *
 *  Created on: Feb 23, 2020
 *      Author: Tim Stark
 */

#pragma once

class fix128_t
{
public:
	fix128_t() = default;
	fix128_t(double val);
	fix128_t(uint64_t val);
	~fix128_t() = default;

	operator double() const;
	operator uint64_t() const;

	inline fix128_t operator + (const fix128_t &val);
	inline fix128_t operator - (const fix128_t &val);
	inline fix128_t operator += (const fix128_t &val);
	inline fix128_t operator -= (const fix128_t &val);
	inline fix128_t operator - () const;

	inline bool operator == (const fix128_t &val) const;
	inline bool operator != (const fix128_t &val) const;
	inline bool operator < (const fix128_t &val) const;
	inline bool operator <= (const fix128_t &val) const;
	inline bool operator > (const fix128_t &val) const;
	inline bool operator >= (const fix128_t &val) const;

	inline bool isNegative() const;
	inline int sign() const;
	inline void negate();

//	fix128_t operator * (const fix128_t &val);
//	fix128_t operator / (const fix128_t &val);
//	fix128_t operator *= (const fix128_t &val);
//	fix128_t operator /= (const fix128_t &val);

public:
	uint64_t hi = 0, lo = 0;
};

inline int fix128_t::sign() const
{
	if (hi == 0 && lo == 0)
		return 0;
	else if (hi > LONG_MAX)
		return -1;
	else
		return 1;
}

inline void fix128_t::negate()
{
	hi = ~hi;
	lo = ~lo;
	if (hi == 0)
		lo++;
}

inline fix128_t fix128_t::operator + (const fix128_t &val)
{
	fix128_t res;

	res.hi = hi + val.hi;
	res.lo = lo + val.lo;
	if (res.lo < lo)
		res.hi++;
	return res;
}

inline fix128_t fix128_t::operator - (const fix128_t &val)
{
	fix128_t res;

	res.hi = hi - val.hi;
	res.lo = lo - val.lo;
	if (res.lo > lo)
		res.lo--;
	return res;
}

inline fix128_t fix128_t::operator += (const fix128_t &val)
{
	hi += val.hi;
	lo += val.lo;
	if (lo < val.lo)
		hi++;
	return *this;
}

inline fix128_t fix128_t::operator -= (const fix128_t &val)
{
	hi -= val.hi;
	lo -= val.lo;
	if (lo > val.lo)
		hi--;
	return *this;
}

inline fix128_t fix128_t::operator - () const
{
	fix128_t res = *this;

	res.negate();
	return res;
}


inline bool fix128_t::isNegative() const
{
	return (hi > LONG_MAX);
}

inline bool fix128_t::operator == (const fix128_t &val) const
{
	return (hi == val.hi) && (lo == val.lo);
}

inline bool fix128_t::operator != (const fix128_t &val) const
{
	return (hi != val.hi) || (lo != val.lo);
}

inline bool fix128_t::operator < (const fix128_t &val) const
{
	if (isNegative() == val.isNegative())
		return hi == val.hi ? lo < val.lo : hi < val.hi;
	return isNegative();
}

inline bool fix128_t::operator <= (const fix128_t &val) const
{
	if (isNegative() == val.isNegative())
		return hi == val.hi ? lo <= val.lo : hi < val.hi;
	return isNegative();
}

inline bool fix128_t::operator > (const fix128_t &val) const
{
	if (isNegative() == val.isNegative())
		return hi == val.hi ? lo > val.lo : hi > val.hi;
	return isNegative();
}

inline bool fix128_t::operator >= (const fix128_t &val) const
{
	if (isNegative() == val.isNegative())
		return hi == val.hi ? lo >= val.lo : hi > val.hi;
	return isNegative();
}
