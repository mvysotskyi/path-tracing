#pragma once

#include "../fmath/fmath.h"

#include <cstring>

//template<typename _T, typename _Tp, unsigned short _Sz>
//class IList {
//public:
//	_T data[_Sz];
//
//	IList(_T first, ...) {
//		va_list parg;
//		va_start(parg, first);
//
//		for (int i = 0; i < _Sz; i++) {
//			data[i] = va_arg(first, _Tp);
//		}
//
//		va_end(parg);
//	}
//};


template<typename _T, unsigned short _Ds>
class vec;

template<typename _T, unsigned short _Ds>
vec<_T, _Ds> operator+(const vec<_T, _Ds>& _vec1, const vec<_T, _Ds>& _vec2);

template<typename _T, unsigned short _Ds>
class vec {
private:
	using value_type = _T;
	using size_type = unsigned long;

	size_type vec_size = _Ds;

public:
	value_type x{};
	value_type y{};
	value_type z{};
	value_type w{};

public:
	vec();
	vec(value_type x, value_type y, value_type z = 0, value_type w = 0);
	vec(value_type _value);

	vec(const vec<value_type, _Ds>& _vec);

public:
	void operator*=(value_type _x);
	void operator+=(const vec& _vec);
	void operator+=(value_type _x);

	friend vec operator+<>(const vec<value_type, _Ds>& _vec1, const vec<value_type, _Ds>& _vec2);
	
	value_type length();
	void normalize();

	value_type* max_abs_coord();
};