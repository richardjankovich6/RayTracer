#include <iostream>
#include <cmath>
#include <stdexcept>
#include <type_traits>

namespace Engine {

// Templated class declaration
template<typename T = float>
class Vector3 {
public:
    // --- Members ---
    T x;
    T y;
    T z;

    // --- Constructors ---
    Vector3() : x(static_cast<T>(0.0)), y(static_cast<T>(0.0)), z(static_cast<T>(0.0)) {}

    template<typename Args>
    explicit Vector3(Args x, Args y, Args z) 
        : x(static_cast<T>(x)), y(static_cast<T>(y)), z(static_cast<T>(z)) {}
    
    Vector3(T v) : x(v), y(v), z(v) {}

    template<typename X, typename Y, typename Z>
    explicit Vector3(X vx, Y vy, Z vz) 
        : x(static_cast<T>(vx)), y(static_cast<T>(vy)), z(static_cast<T>(vz)) {}
    
    
    // --- Math Operators ---

    // + (Vector addition)
    Vector3 operator+(const Vector3& other) const {
        return Vector3(x + other.x, y + other.y, z + other.z);
    }

    // - (Vector subtraction)
    Vector3 operator-(const Vector3& other) const {
        return Vector3(x - other.x, y - other.y, z - other.z);
    }

    // Unary Negation
    Vector3 operator-() const {
        return Vector3(-x, -y, -z);
    }

    // * (Scalar multiplication)
    template<typename U>
    Vector3 operator*(U scalar) const {
        return Vector3(x * static_cast<T>(scalar), y * static_cast<T>(scalar), z * static_cast<T>(scalar));
    }

    // / (Division by scalar)
    template<typename U>
    Vector3 operator/(U scalar) const {
        if constexpr (std::is_arithmetic<U>::value && scalar == T{0}) {
            throw std::runtime_error("Division by zero");
        }
        return Vector3(x / static_cast<T>(scalar), y / static_cast<T>(scalar), z / static_cast<T>(scalar));
    }

    // Equality Comparison (for float/double types)
    template<typename U = T>
    bool operator==(const Vector3& other) const {
        constexpr float EPSILON = 0.0001f;
        return std::abs(static_cast<float>(x - other.x)) < EPSILON &&
               std::abs(static_cast<float>(y - other.y)) < EPSILON &&
               std::abs(static_cast<float>(z - other.z)) < EPSILON;
    }

    // --- Math Functions ---

    T Length() const {
        return std::sqrt(x * x + y * y + z * z);
    }

    Vector3 Normalize() const {
        T len = Length();
        if (len > static_cast<T>(0)) {
            return Vector3(x / len, y / len, z / len);
        }
        return Vector3(0, 0, 0);
    }

    T Dot(const Vector3& other) const {
        return x * other.x + y * other.y + z * other.z;
    }

    Vector3 Cross(const Vector3& other) const {
        return Vector3(
            y * other.z - z * other.y,
            z * other.x - x * other.z,
            x * other.y - y * other.x
        );
    }

    friend std::ostream& operator<<(std::ostream& os, const Vector3<T>& v) {
        return os << "(" << v.x << ", " << v.y << ", " << v.z << ")";
    }

    // --- Component Accessors ---
    // template<typename U = T>
    static T zero() { return T{0}; }

public:
    using value_type = T;
};

template<typename T>
std::ostream& operator<<(std::ostream& os, const Engine::Vector3<T>& v) {
    return os << "(" << v.x << ", " << v.y << ", " << v.z << ")";
}

} // namespace Engine
