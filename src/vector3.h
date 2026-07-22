#pragma once

#ifndef VECTOR3_H
#define VECTOR3_H

#include <cmath>
#include <iostream>
#include <string>

template <typename ntype = double>
class Vector3 {
public:
	//typename n = ntype;
	ntype vec[3];

	Vector3() : vec{ 0,0,0 } {};
	Vector3(ntype e0, ntype e1, ntype e2) : vec{e0, e1, e2} {}

	ntype x() const { return vec[0]; }
	ntype y() const { return vec[1]; }
	ntype z() const { return vec[2]; }

	Vector3 operator-() const { return Vector3(-vec[0], -vec[1], -vec[2]); }
	ntype operator[](int i) const { return vec[i]; }
	ntype& operator[](int i) { return vec[i]; }

	Vector3& operator+=(const Vector3& v) {
		vec[0] += v.vec[0];
		vec[1] += v.vec[1];
		vec[2] += v.vec[2];
		return *this;
	}

	Vector3& operator*=(double t) {
		vec[0] *= t;
		vec[1] *= t;
		vec[2] *= t;
		return *this;
	}

	Vector3& operator/=(double t) { return *this *= 1 / t; }
	
	double lengthSquared() const {
		return vec[0] * vec[0] + vec[1] * vec[1] + vec[2] * vec[2];
	}

	double length() const {
		return std::sqrt(lengthSquared());
	}
};

// using point3 = Vector3<double>;


// inline std::ostream& operator <<(std::ostream& out, const Vector3<int>& v) {
// 	out << " %d %d %d ", v.vec[0], v.vec[1], v.vec[2];
// 	// out << " %d %d %d ", v.vec[0], v.vec[1], v.vec[2];
// 	//out << static_cast<int>(v.e[0]) << ' ' << static_cast<int>(v.e[1]) << ' ' << static_cast<int>(v.e[2]);
// 	return out;
// 	//return out << " %d %d %d ",  v.e[0], v.e[1], v.e[2];
// 	//return out << v.e[0] << ' ' << v.e[1] << ' ' << v.e[2];
// };

// inline std::ostream& OutputColor(std::ostream& out, const Vector3<int>& v) {
//     return out << "(" << v.x << ", " << v.y << ", " << v.z << ")";
// }

template<typename T>
std::ostream& operator<<(std::ostream& out, const Vector3<T>& v) {
    // return out << "(" << v.vec[0] << ", " << v.vec[1] << ", " << v.vec[2] << ")";
    return out << "(" << v.x() << ", " << v.y() << ", " << v.z() << ")";
}


//inline std::ostream& operator <<(std::ostream& out, const vec3<float>& v) {
//	return out << v.e[0] << ' ' << v.e[1] << ' ' << v.e[2];
//}
//inline std::ostream& operator <<(std::ostream& out, const vec3<double>& v) {
//	return out << v.e[0] << ' ' << v.e[1] << ' ' << v.e[2];
//}

inline Vector3<> operator+(const Vector3<>& u, const Vector3<>& v) {
	return Vector3<>(u.vec[0] + v.vec[0], u.vec[1] + v.vec[1], u.vec[2] + v.vec[2]);
};

inline Vector3<> operator-(const Vector3<>& u, const Vector3<>& v) {
	return Vector3<>(u.vec[0] - v.vec[0], u.vec[1] - v.vec[1], u.vec[2] - v.vec[2]);
};

inline Vector3<> operator*(const Vector3<>& u, const Vector3<>& v) {
	return Vector3<>(u.vec[0] * v.vec[0], u.vec[1] * v.vec[1], u.vec[2] * v.vec[2]);
};

template<typename A, typename B>
inline Vector3<A> operator*(const Vector3<A>& u, const Vector3<B>& v) {
	return Vector3<A>(u.vec[0] * v.vec[0], u.vec[1] * v.vec[1], u.vec[2] * v.vec[2]);
};

inline Vector3<> operator*(double t, const Vector3<>& v) {
	return Vector3<>(t * v.vec[0], t * v.vec[1], t * v.vec[2]);
};

inline Vector3<> operator*(const Vector3<>& v, double t) {
	return t * v;
};

inline Vector3<int> operator*(double t, const Vector3<int>& v) {
	return Vector3<int>(t * v.vec[0], t * v.vec[1], t * v.vec[2]);
};

inline Vector3<int> operator*(const Vector3<int>& v, double t) {
	return t * v;
};

template<typename A, typename B>
inline Vector3<A> operator*(B t, const Vector3<A>& v) {
	return Vector3<A>(t * v.vec[0], t * v.vec[1], t * v.vec[2]);
};

template<typename A, typename B>
inline Vector3<A> operator*(const Vector3<A>& v, B t) {
	return t * v;
};

inline Vector3<> operator/(const Vector3<>& v, double t) {
	return (1 / t) * v;
};

inline double dot(const Vector3<>& u, const Vector3<>& v) {
	return u.vec[0] * v.vec[0]
		+ u.vec[1] * v.vec[1]
		+ u.vec[2] * v.vec[2];
};

inline Vector3<> cross(const Vector3<>& u, const Vector3<>& v) {
	return Vector3<>(u.vec[1] * v.vec[2] - u.vec[2] * v.vec[1],
		u.vec[2] * v.vec[0] - u.vec[0] * v.vec[2],
		u.vec[0] * v.vec[1] - u.vec[1] * v.vec[0]);
};

inline Vector3<> unitVec(const Vector3<>& v) {
	return v / v.length();
};

#endif