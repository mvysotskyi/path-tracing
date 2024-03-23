#include "vec.h"

template<typename _T, unsigned short _Ds>
vec<_T, _Ds>::vec() {
	value_type* dptr = &(this->x);
	for (size_type i = 0; i < vec_size; i++) {
		*dptr = 0;
		++dptr;
	}
}

template<typename _T, unsigned short _Ds>
vec<_T, _Ds>::vec(value_type x, value_type y, value_type z, value_type w) {
	memcpy(&(this->x), &x, sizeof(value_type) * vec_size);
}

template<typename _T, unsigned short _Ds>
vec<_T, _Ds>::vec(value_type _value) {
	value_type _x = _value * q_rsqrt((float)vec_size);
	
	value_type* dptr = &(this->x);
	for (size_type i = 0; i < vec_size; i++) {
		*dptr = _x;
		++dptr;
	}
}

template<typename _T, unsigned short _Ds>
vec<_T, _Ds>::vec(const vec<_T, _Ds>& _vec) {
	memcpy(&x, &(_vec.x), sizeof(value_type) * vec_size);
}

template<typename _T, unsigned short _Ds>
void vec<_T, _Ds>::operator*=(value_type _x) {
	value_type* begin = &x;
	for (size_type i = 0; i < vec_size; ++i) begin[i] *= _x;
}

template<typename _T, unsigned short _Ds>
void vec<_T, _Ds>::operator+=(const vec<value_type, _Ds>& _vec) {
	value_type* begin1 = &x, *begin2 = const_cast<float*>(&_vec.x);

	for (size_type i = 0; i < vec_size; i++) begin1[i] += begin2[i];
} 

template<typename _T, unsigned short _Ds>
void vec<_T, _Ds>::operator+=(value_type _x) {
	value_type* begin = &x;
	for (size_type i = 0; i < vec_size; ++i) begin[i] += _x;
}

template<typename _T, unsigned short _Ds>
vec<_T, _Ds> operator+(const vec<_T, _Ds>& _vec1, const vec<_T, _Ds>& _vec2) {
	vec<_T, _Ds> ret(0, 0, 0);

	_T* beg = &ret.x;
	const _T* beg1 = &_vec1.x;
	const _T* beg2 = &_vec2.x;

	for (int i = 0; i < _Ds; ++i) {
		beg[i] = beg1[i] + beg2[i];
	}
	return ret;
}

template<typename _T, unsigned short _Ds>
_T vec<_T, _Ds>::length() {
	value_type* begin = &x;
	value_type sq_sum = 0;

	for (size_type i = 0; i < vec_size; ++i) sq_sum += (begin[i] * begin[i]);
	return sqrt(sq_sum);
}

template<typename _T, unsigned short _Ds>
void vec<_T, _Ds>::normalize() {
	value_type* begin = &x;
	value_type sq_sum = 0;

	for (size_type i = 0; i < vec_size; ++i) sq_sum += (begin[i] * begin[i]);
	float n_value = q_rsqrt(sq_sum);

	for (size_type i = 0; i < vec_size; ++i) begin[i] *= n_value;
}

template<typename _T, unsigned short _Ds> // FIXME!
_T* vec<_T, _Ds>::max_abs_coord() {
	value_type _max = 0, *_max_ptr = &x;

	for (value_type* cptr = &x; (size_type)(cptr - &x) < vec_size; cptr++) {
		value_type _val = fabs(*cptr);

		if (_val > _max) _max = _val;
		_max_ptr = cptr;
	}

	return _max_ptr;
}

template vec<float, 3> operator+(const vec<float, 3>&, const vec<float, 3>&);
template vec<float, 2> operator+(const vec<float, 2>&, const vec<float, 2>&);

template class vec<float, 3>;
template class vec<float, 2>;