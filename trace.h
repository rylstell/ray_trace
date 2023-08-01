#ifndef TRACE_H
#define TRACE_H

#include <vector>
#include <string>
#include <limits>
#include "vec3.h"




#define PI 3.14159265358979323846
const float infinity = std::numeric_limits<float>::infinity();




struct Color {
    float r;
    float g;
    float b;
    Color operator + (const Color&) const;
    Color operator * (float) const;
    friend std::ostream& operator << (std::ostream&, const Color&);
};



struct Material {
    Color color = {0.5, 0.5, 0.5};
    float specular = 0.3;
    float diffuse = 0.3;
    float ambient = 0.4;
    float shininess = 32;
    float reflectance = 1;
    float phong_factor = 1.0;
};



struct Intersection {
    Vec3 point;
    Vec3 normal;
    Vec3 to_light;
    Vec3 to_camera;
    Vec3 light_reflect;
    Vec3 ray_reflect;
    Vec3 ray_transmit;
};



struct Camera {
    Vec3 pos;
    Vec3 dir;
    Vec3 up;
    Vec3 right;
    Camera(const Vec3&, const Vec3&, const Vec3&);
};



struct Light {
    Vec3 pos;
    float specular = 1;
    float diffuse = 1;
};




class Object {

protected:
    Vec3 pos = {0, 0, 0};
    
public:
    Material material;
    bool has_interior = true;
    float interior_ior = 1;
    float exterior_ior = 1;

    Material get_material() const;
    void set_material(Material&);

    virtual Vec3 get_pos();
    virtual Vec3 get_normal(Vec3);
    virtual bool ray_intersect(const Vec3&, const Vec3&, float&);

};

class Sphere: public Object {
public:
    float radius;
    Sphere(float);
    Sphere(float, Material);
    Vec3 get_normal(Vec3);
    bool ray_intersect(const Vec3&, const Vec3&, float&);
    void translate(Vec3);
    void rotate_x(float);
    void rotate_y(float);
    void rotate_z(float);
    void rotate_x(float, Vec3);
    void rotate_y(float, Vec3);
    void rotate_z(float, Vec3);
};

class Plane: public Object {
private:
    Vec3 normal;
public:
    Plane(Vec3);
    Plane(Vec3, Material);
    Vec3 get_normal(Vec3);
    bool ray_intersect(const Vec3&, const Vec3&, float&);
    void translate(Vec3);
    void rotate_x(float);
    void rotate_y(float);
    void rotate_z(float);
    void rotate_x(float, Vec3);
    void rotate_y(float, Vec3);
    void rotate_z(float, Vec3);
};

class Rectangle: public Object {
private:
    Plane plane;
    Vec3 corner;
    Vec3 edge1;
    Vec3 edge2;
public:
    Rectangle(float, float);
    Rectangle(float, float, Material);
    Vec3 get_pos();
    Vec3 get_normal(Vec3);
    bool ray_intersect(const Vec3&, const Vec3&, float&);
    void translate(Vec3);
    void rotate_x(float);
    void rotate_y(float);
    void rotate_z(float);
    void rotate_x(float, Vec3);
    void rotate_y(float, Vec3);
    void rotate_z(float, Vec3);
};

class Box: public Object {
private:
    Rectangle rectangles[6];
    int last_intersected;  // I don't like this variable lol
public:
    Box(float, float, float);
    Box(float, float, float, Material);
    Vec3 get_normal(Vec3);
    bool ray_intersect(const Vec3&, const Vec3&, float&);
    void translate(Vec3);
    void rotate_x(float);
    void rotate_y(float);
    void rotate_z(float);
    void rotate_x(float, Vec3);
    void rotate_y(float, Vec3);
    void rotate_z(float, Vec3);
};






class Scene {

private:
    int width;
    int height;
    const Camera& cam;
    const Light& light;
    std::vector<Object*> objects;
    int bounces = 0;
    float fov = PI / 2;
    Color background_color = {0, 0, 0};
    float ambient = 1;
    Object* get_closest_object(const Vec3&, const Vec3&, float&);
    Color trace(const Vec3&, const Vec3&, int, float=1.0);
    Intersection process_intersection(const Vec3&, const Vec3&, const Vec3&, float, float);

public:
    Scene(const Camera&, const Light&, int, int);
    void add_object(Object*);
    void capture(const std::string&);
    void set_bounces(int);
    void set_fov(float);
    void set_background_color(Color);
    void set_ambient(float);
};




void write_image_ppm_ascii(int*, int, int, const std::string&);

float fresnel(float, float, float, const Vec3&, const Vec3&);


#endif


