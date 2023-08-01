#include <iostream>
#include "vec3.h"


Vec3::Vec3(float _x, float _y, float _z) {
    x = _x;
    y = _y;
    z = _z;
}

Vec3::Vec3(float val) :
    Vec3(val, val, val)
{}

Vec3::Vec3(const Vec3& vec) :
    Vec3(vec.x, vec.y, vec.z)
{}

float Vec3::dot(const Vec3& vec) const {
    return x * vec.x + y * vec.y + z * vec.z;
}

Vec3 Vec3::cross(const Vec3& vec) const {
    Vec3 ret;
    ret.x = y * vec.z - z * vec.y;
    ret.y = z * vec.x - x * vec.z;
    ret.z = x * vec.y - y * vec.x;
    return ret;
}

float Vec3::mag_squared() const {
    return x * x + y * y + z * z;
}

float Vec3::mag() const {
    return sqrt(mag_squared());
}

Vec3 Vec3::abs() const {
    Vec3 ret;
    ret.x = std::abs(x);
    ret.y = std::abs(y);
    ret.z = std::abs(z);
    return ret;
}

Vec3 Vec3::normalize() const {
    Vec3 ret;
    float m = mag();
    ret.x = x / m;
    ret.y = y / m;
    ret.z = z / m;
    return ret;
}

void Vec3::normalize_ip() {
    float m = mag();
    x /= m;
    y /= m;
    z /= m;
}

Vec3 Vec3::rotate_x(float theta) {
    float cos_theta = cos(theta);
    float sin_theta = sin(theta);
    Vec3 ret;
    ret.x = x;
    ret.y = y * cos_theta - z * sin_theta;
    ret.z = y * sin_theta + z * cos_theta;
    return ret;
}

Vec3 Vec3::rotate_y(float theta) {
    float cos_theta = cos(theta);
    float sin_theta = sin(theta);
    Vec3 ret;
    ret.x = x * cos_theta + z * sin_theta;
    ret.y = y;
    ret.z = z * cos_theta - x * sin_theta;
    return ret;
}

Vec3 Vec3::rotate_z(float theta) {
    float cos_theta = cos(theta);
    float sin_theta = sin(theta);
    Vec3 ret;
    ret.x = x * cos_theta - y * sin_theta;
    ret.y = x * sin_theta + y * cos_theta;
    ret.z = z;
    return ret;
}

Vec3 Vec3::operator + (const Vec3& vec) const {
    Vec3 ret;
    ret.x = x + vec.x;
    ret.y = y + vec.y;
    ret.z = z + vec.z;
    return ret;
}

Vec3 Vec3::operator - (const Vec3& vec) const {
    Vec3 ret;
    ret.x = x - vec.x;
    ret.y = y - vec.y;
    ret.z = z - vec.z;
    return ret;
}

Vec3 Vec3::operator * (float scaler) const {
    Vec3 ret;
    ret.x = x * scaler;
    ret.y = y * scaler;
    ret.z = z * scaler;
    return ret;
}

Vec3 Vec3::operator / (float scaler) const {
    Vec3 ret;
    ret.x = x / scaler;
    ret.y = y / scaler;
    ret.z = z / scaler;
    return ret;
}

Vec3 Vec3::operator += (const Vec3& vec) {
    x += vec.x;
    y += vec.y;
    z += vec.z;
    return *this;
}

Vec3 Vec3::operator -= (const Vec3& vec) {
    x -= vec.x;
    y -= vec.y;
    z -= vec.z;
    return *this;
}

Vec3 Vec3::operator *= (float scaler) {
    x *= scaler;
    y *= scaler;
    z *= scaler;
    return *this;
}

Vec3 Vec3::operator /= (float scaler) {
    x /= scaler;
    y /= scaler;
    z /= scaler;
    return *this;
}

Vec3 Vec3::vmin(const Vec3& v1, const Vec3& v2) {
    Vec3 ret;
    ret.x = std::min(v1.x, v2.x);
    ret.y = std::min(v1.y, v2.y);
    ret.z = std::min(v1.z, v2.z);
    return ret;
}

Vec3 Vec3::vmax(const Vec3& v1, const Vec3& v2) {
    Vec3 ret;
    ret.x = std::max(v1.x, v2.x);
    ret.y = std::max(v1.y, v2.y);
    ret.z = std::max(v1.z, v2.z);
    return ret;
}

std::ostream& operator << (std::ostream &output, const Vec3& vec) {
    output << "Vec3(" << vec.x << ", " << vec.y << ", " << vec.z << ")";
    return output;
}
