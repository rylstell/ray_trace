#ifndef VEC3_H
#define VEC3_H

class Vec3 {
public:
    float x, y, z;
    Vec3(float, float, float);
    Vec3(float=0);
    Vec3(const Vec3&);
    float dot(const Vec3&) const;
    Vec3 cross(const Vec3&) const;
    float mag_squared() const;
    float mag() const;
    Vec3 abs() const;
    Vec3 normalize() const;
    void normalize_ip();
    Vec3 rotate_x(float);
    Vec3 rotate_y(float);
    Vec3 rotate_z(float);
    Vec3 operator + (const Vec3&) const;
    Vec3 operator - (const Vec3&) const;
    Vec3 operator * (float) const;
    Vec3 operator / (float) const;
    Vec3 operator += (const Vec3&);
    Vec3 operator -= (const Vec3&);
    Vec3 operator *= (float);
    Vec3 operator /= (float);
    static Vec3 vmin(const Vec3&, const Vec3&);
    static Vec3 vmax(const Vec3&, const Vec3&);
    friend std::ostream& operator << (std::ostream&, const Vec3&);
};

#endif