#include <iostream>
#include <math.h>
#include <vector>
#include <string>
#include <fstream>
#include "trace.h"



void write_image_ppm_ascii(int* pixels, int w, int h, const std::string& filename) {
    std::ofstream image_stream;
    image_stream.open(filename + ".ppm");
    image_stream
        << "P3\n"
        << w << " " << h << "\n"
        << "255\n";
    int n = w * h * 3;
    for (int i = 0; i < n; i++) {
        image_stream << pixels[i] << " ";
        if ((i+1) % 7 == 0) { image_stream << "\n"; }
    }
}

// I didn't write this function...
float fresnel(float n1, float n2, float reflectance, const Vec3& normal, const Vec3& ray) {
    float r0 = (n1 - n2) / (n1 + n2);
    r0 = r0 * r0;
    float cosX = -1 * normal.dot(ray);
    if (n1 > n2) {
        float n = n1 / n2;
        float sinT2 = n * n * (1.0 - cosX * cosX);
        if (sinT2 > 1.0) {
            return 1.0;
        }
        cosX = sqrt(1.0 - sinT2);
    }
    float x = 1.0 - cosX;
    float ret = r0 + (1.0 - r0) * x * x * x * x * x;
    ret = (reflectance + (1.0 - reflectance) * ret);
    return ret;
}

int min(int a, int b) {
    return a < b ? a : b;
}

int max(int a, int b) {
    return a > b ? a : b;
}







Color Color::operator + (const Color& color) const {
    Color ret;
    ret.r = r + color.r;
    ret.g = g + color.g;
    ret.b = b + color.b;
    return ret;
}

Color Color::operator * (float scaler) const {
    Color ret;
    ret.r = r * scaler;
    ret.g = g * scaler;
    ret.b = b * scaler;
    return ret;
}

std::ostream& operator << (std::ostream &output, const Color& color) {
    output << "Color(" << color.r << ", " << color.g << ", " << color.b << ")";
    return output;
}





Camera::Camera(const Vec3& _pos, const Vec3& _dir, const Vec3& _up) :
    pos(_pos),
    dir(_dir),
    up(_up)
{
    right = up.cross(dir);
    right.normalize_ip();
}






Material Object::get_material() const{
    return material;
}

void Object::set_material(Material& _material) {
    material = _material;
}

Vec3 Object::get_pos() {
    return pos;
}

Vec3 Object::get_normal(Vec3 point) {
    return {0, 0, 0};
}

bool Object::ray_intersect(const Vec3& ray, const Vec3& ray_origin, float& t) {
    return false;
}






Sphere::Sphere(float _radius) {
    radius = _radius;
}

Sphere::Sphere(float _radius, Material _material) :
    Sphere(_radius)
{
    material = _material;
}

Vec3 Sphere::get_normal(Vec3 point) {
    Vec3 normal = point - pos;
    normal.normalize_ip();
    return normal;
}

bool Sphere::ray_intersect(const Vec3& ray, const Vec3& ray_origin, float& t) {
    Vec3 origin_to_pos = pos - ray_origin;
    float side1 = origin_to_pos.dot(ray);
    float side2 = sqrt(origin_to_pos.mag_squared() - side1 * side1);
    if (side2 > radius) {
        return false;
    }
    float half = sqrt(radius * radius - side2 * side2);
    float t1 = side1 - half;
    float t2 = side1 + half;
    if (t2 < 0) {
        return false;
    }
    t = t1 < 0 ? t2 : t1;
    return true;
}

void Sphere::translate(Vec3 vec) {
    pos += vec;
}

void Sphere::rotate_x(float theta) {
    rotate_x(theta, pos);
}

void Sphere::rotate_y(float theta) {
    rotate_y(theta, pos);
}

void Sphere::rotate_z(float theta) {
    rotate_z(theta, pos);
}

void Sphere::rotate_x(float theta, Vec3 rotation_origin) {
    pos -= rotation_origin;
    pos = pos.rotate_x(theta);
    pos += rotation_origin;
}

void Sphere::rotate_y(float theta, Vec3 rotation_origin) {
    pos -= rotation_origin;
    pos = pos.rotate_y(theta);
    pos += rotation_origin;
}

void Sphere::rotate_z(float theta, Vec3 rotation_origin) {
    pos -= rotation_origin;
    pos = pos.rotate_z(theta);
    pos += rotation_origin;
}








Plane::Plane(Vec3 _normal) {
    normal = _normal;
    has_interior = false;
}

Plane::Plane(Vec3 _normal, Material _material) :
    Plane(_normal)
{
    material = _material;
}

Vec3 Plane::get_normal(Vec3 point) {
    return normal;
}

bool Plane::ray_intersect(const Vec3& ray, const Vec3& ray_origin, float& t) {
    float cos_theta = normal.dot(ray);
    if (abs(cos_theta) > 1e-6) {
        Vec3 vec = pos - ray_origin;
        t = normal.dot(vec) / cos_theta;
        return (t >= 0);
    }
    return false;
}

void Plane::translate(Vec3 vec) {
    pos += vec;
}

void Plane::rotate_x(float theta) {
    rotate_x(theta, pos);
}

void Plane::rotate_y(float theta) {
    rotate_y(theta, pos);
}

void Plane::rotate_z(float theta) {
    rotate_z(theta, pos);
}

void Plane::rotate_x(float theta, Vec3 rotation_origin) {
    pos -= rotation_origin;
    pos = pos.rotate_x(theta);
    pos += rotation_origin;
    normal = normal.rotate_x(theta);
}

void Plane::rotate_y(float theta, Vec3 rotation_origin) {
    pos -= rotation_origin;
    pos = pos.rotate_y(theta);
    pos += rotation_origin;
    normal = normal.rotate_y(theta);
}

void Plane::rotate_z(float theta, Vec3 rotation_origin) {
    pos -= rotation_origin;
    pos = pos.rotate_z(theta);
    pos += rotation_origin;
    normal = normal.rotate_z(theta);
}







Rectangle::Rectangle(float _width, float _height) :
    plane({0, 0, -1})
{
    corner = {-_width / 2, -_height / 2, 0};
    edge1 = {_width, 0, 0};
    edge2 = {0, _height, 0};
    has_interior = false;
}

Rectangle::Rectangle(float _width, float _height, Material _material) :
    Rectangle(_width, _height)
{
    material = _material;
}

Vec3 Rectangle::get_pos() {
    return plane.get_pos();
}

Vec3 Rectangle::get_normal(Vec3 point) {
    return plane.get_normal(point);
}

bool Rectangle::ray_intersect(const Vec3& ray, const Vec3& ray_origin, float& t) {
    if (plane.ray_intersect(ray, ray_origin, t)) {
        Vec3 point = ray_origin + ray * t;
        Vec3 vec = point - corner;
        float width = edge1.mag();
        float height = edge2.mag();
        float p1 = vec.dot(edge1) / width;
        float p2 = vec.dot(edge2) / height;
        if ((p1 < width && p1 > 0) && (p2 < height && p2 > 0)) {
            return true;
        }
    }
    return false;
}

void Rectangle::translate(Vec3 vec) {
    plane.translate(vec);
    corner += vec;
}

void Rectangle::rotate_x(float theta) {
    rotate_x(theta, plane.get_pos());
}

void Rectangle::rotate_y(float theta) {
    rotate_y(theta, plane.get_pos());
}

void Rectangle::rotate_z(float theta) {
    rotate_z(theta, plane.get_pos());
}

void Rectangle::rotate_x(float theta, Vec3 rotation_origin) {
    plane.rotate_x(theta, rotation_origin);
    edge1 = edge1.rotate_x(theta);
    edge2 = edge2.rotate_x(theta);
    corner -= rotation_origin;
    corner = corner.rotate_x(theta);
    corner += rotation_origin;
}

void Rectangle::rotate_y(float theta, Vec3 rotation_origin) {
    plane.rotate_y(theta, rotation_origin);
    edge1 = edge1.rotate_y(theta);
    edge2 = edge2.rotate_y(theta);
    corner -= rotation_origin;
    corner = corner.rotate_y(theta);
    corner += rotation_origin;
}

void Rectangle::rotate_z(float theta, Vec3 rotation_origin) {
    plane.rotate_z(theta, rotation_origin);
    edge1 = edge1.rotate_z(theta);
    edge2 = edge2.rotate_z(theta);
    corner -= rotation_origin;
    corner = corner.rotate_z(theta);
    corner += rotation_origin;
}





Box::Box(float xsize, float ysize, float zsize) :
    // left, right, front, back, top, bottom
    rectangles {
        Rectangle(zsize, ysize),
        Rectangle(zsize, ysize),
        Rectangle(xsize, ysize),
        Rectangle(xsize, ysize),
        Rectangle(xsize, zsize),
        Rectangle(xsize, zsize)
    }
{
    rectangles[0].rotate_y(PI / 2);
    rectangles[0].translate({-xsize / 2, 0, 0});

    rectangles[1].rotate_y(-PI / 2);
    rectangles[1].translate({xsize / 2, 0, 0});

    rectangles[2].translate({0, 0, -zsize / 2});

    rectangles[3].rotate_x(PI);
    rectangles[3].translate({0, 0, zsize / 2});

    rectangles[4].rotate_x(PI / 2);
    rectangles[4].translate({0, ysize / 2, 0});

    rectangles[5].rotate_x(-PI / 2);
    rectangles[5].translate({0, -ysize / 2, 0});
}

Box::Box(float xsize, float ysize, float zsize, Material _material) :
    Box(xsize, ysize, zsize)
{
    material = _material;
}

Vec3 Box::get_normal(Vec3 point) {
    return rectangles[last_intersected].get_normal(point);
}

bool Box::ray_intersect(const Vec3& ray, const Vec3& ray_origin, float& t) {
    float tmp_t;
    float closest_t = infinity;
    int closest_i = -1;
    for (int i = 0; i < 6; i++) {
        if (rectangles[i].ray_intersect(ray, ray_origin, tmp_t)) {
            if (tmp_t < closest_t) {
                closest_t = tmp_t;
                closest_i = i;
            }
        }
    }
    if (closest_i != -1) {
        t = closest_t;
        last_intersected = closest_i;
        return true;
    }
    return false;
}

void Box::translate(Vec3 vec) {
    pos += vec;
    for (int i = 0; i < 6; i++) {
        rectangles[i].translate(vec);
    }
}

void Box::rotate_x(float theta) {
    for (int i = 0; i < 6; i++) {
        rectangles[i].rotate_x(theta, pos);
    }
}

void Box::rotate_y(float theta) {
    for (int i = 0; i < 6; i++) {
        rectangles[i].rotate_y(theta, pos);
    }
}

void Box::rotate_z(float theta) {
    for (int i = 0; i < 6; i++) {
        rectangles[i].rotate_z(theta, pos);
    }
}

void Box::rotate_x(float theta, Vec3 rotation_origin) {
    for (int i = 0; i < 6; i++) {
        rectangles[i].rotate_x(theta, rotation_origin);
    }
}

void Box::rotate_y(float theta, Vec3 rotation_origin) {
    for (int i = 0; i < 6; i++) {
        rectangles[i].rotate_y(theta, rotation_origin);
    }
}

void Box::rotate_z(float theta, Vec3 rotation_origin) {
    for (int i = 0; i < 6; i++) {
        rectangles[i].rotate_z(theta, rotation_origin);
    }
}




Scene::Scene(const Camera& _cam, const Light& _light, int _width, int _height) :
    cam(_cam),
    light(_light)
{
    width = _width;
    height = _height;
    objects = std::vector<Object*>();
}

void Scene::set_bounces(int _bounces) {
    bounces = _bounces;
}

void Scene::set_fov(float _fov) {
    fov = _fov;
}

void Scene::set_background_color(Color color) {
    background_color = color;
}

void Scene::set_ambient(float _ambient) {
    ambient = _ambient;
}

void Scene::add_object(Object* object) {
    objects.push_back(object);
}

void Scene::capture(const std::string& filename) {

    float gx = tan(fov / 2.0);
    float gy = gx * (height - 1.0) / (width - 1.0);
    Vec3 dx = cam.right * (2.0 * gx / (width - 1.0));
    Vec3 dy = cam.up * (-2.0 * gy / (height - 1.0));
    Vec3 pixel = cam.pos + cam.dir - (cam.right * gx) + (cam.up * gy);

    int* pixels = new int[width * height * 3];
    
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            Vec3 ray = pixel + (dx * j) + (dy * i) - cam.pos;               
            ray.normalize_ip();
            Color color = trace(ray, cam.pos, bounces);
            int pi = (i * width + j) * 3;
            pixels[pi] = min(color.r * 255, 255);
            pixels[pi+1] = min(color.g * 255, 255);
            pixels[pi+2] = min(color.b * 255, 255);
        }
    }

    write_image_ppm_ascii(pixels, width, height, filename);
    delete[] pixels;
}

Object* Scene::get_closest_object(const Vec3& ray, const Vec3& ray_origin, float& t) {
    Object* closest_object = nullptr;
    float closest_t = infinity;
    float object_t;
    for (Object* object : objects) {
        if (object->ray_intersect(ray, ray_origin, object_t)) {
            if (object_t > 0 && object_t < closest_t) {
                closest_t = object_t;
                closest_object = object;
            }
        }
    }
    t = closest_t;
    return closest_object;
}

Color Scene::trace(const Vec3& ray, const Vec3& ray_origin, int bounce, float ior) {

    float t;
    Object* object = get_closest_object(ray, ray_origin, t);
    if (object == nullptr) {
        return background_color;
    }

    Vec3 point = ray_origin + ray * t;
    Vec3 normal = object->get_normal(point);
    Material mat = object->material;

    float transmittance_ior;
    if (ray.dot(normal) < 0 && object->has_interior) {
        transmittance_ior = object->interior_ior;
    } else {
        transmittance_ior = object->exterior_ior;
    }
    
    if (ray.dot(normal) > 0) {
        normal *= -1;
    }

    Intersection inter = process_intersection(point, normal, ray, ior, transmittance_ior);

    float shadow_t;
    Vec3 shadow_ray_origin = inter.point + inter.to_light * 0.01;
    Object* shadow_object = get_closest_object(inter.to_light, shadow_ray_origin, shadow_t);
    bool in_shadow = false;
    if (shadow_object != nullptr) {
        float dist_to_shadow_object = (inter.to_light * shadow_t).mag_squared();
        float dist_to_light = (light.pos - inter.point).mag_squared();
        in_shadow = dist_to_shadow_object < dist_to_light;
    }

    float intensity = mat.ambient * ambient;

    if (!in_shadow) {
       float diffuse_dot = inter.to_light.dot(inter.normal);
        if (diffuse_dot > 0) {
            intensity += mat.diffuse * light.diffuse * diffuse_dot;
            float specular_dot = inter.light_reflect.dot(inter.to_camera);
            if (specular_dot > 0) {
                intensity += mat.specular * light.specular * pow(specular_dot, mat.shininess);
            }
        }
    }

    if (bounce == 0) {
        return mat.color * intensity;
    }

    Vec3 reflect_origin = inter.point + inter.ray_reflect * 0.01;
    Vec3 transmit_origin = inter.point + inter.ray_transmit * 0.01;
    Color reflected_color = trace(inter.ray_reflect, reflect_origin, bounce - 1, ior);
    Color transmitted_color = trace(inter.ray_transmit, transmit_origin, bounce - 1, transmittance_ior);

    float fres = fresnel(ior, transmittance_ior, mat.reflectance, inter.normal, ray);
    float rtf = 1 - mat.phong_factor;

    return mat.color * (intensity * mat.phong_factor)
           + reflected_color * (fres * rtf)
           + transmitted_color * ((1.0 - fres) * rtf);

}

Intersection Scene::process_intersection(const Vec3& point, const Vec3& normal, const Vec3& ray, float n1, float n2) {
    Intersection inter;
    inter.point = point;
    inter.normal = normal;
    inter.to_light = light.pos - inter.point;
    inter.to_light.normalize_ip();
    inter.to_camera = cam.pos - inter.point;
    inter.to_camera.normalize_ip();
    inter.light_reflect = inter.normal * (2 * inter.to_light.dot(inter.normal)) - inter.to_light;
    inter.ray_reflect = ray + inter.normal * (-2 * ray.dot(inter.normal));
    float nr = n1 / n2;
    float ndr = inter.normal.dot(ray);
    inter.ray_transmit = ray * nr + normal * (-1 * nr * ndr - sqrt(1 - nr * nr * (1 - ndr * ndr)));
    return inter;
}
