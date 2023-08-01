#include "trace.h"
#include "vec3.h"


int main(int argc, char *argv[]) {

    Light light;
    light.pos = {-50, 80, -50};

    Vec3 cam_pos = {0, 0, -135};
    Vec3 cam_up = {0, 1, 0};
    cam_pos = cam_pos.rotate_x(PI / 36);
    cam_up = cam_up.rotate_x(PI / 36);
    Vec3 cam_dir = Vec3(0, 0, 0) - cam_pos;
    cam_pos += {0, 15, 0};
    cam_dir.normalize_ip();
    Camera cam(cam_pos, cam_dir, cam_up);

    int width = 400;
    int height = 225;

    Scene scene(cam, light, width, height);
    scene.set_bounces(5);
    scene.set_fov(PI / 4);
    scene.set_background_color({0, 0, 0});

    Material wall_mat;
    wall_mat.color = {1, 1, 1};
    wall_mat.specular = 0;
    wall_mat.diffuse = 0.3;
    wall_mat.ambient = 0.7;

    Rectangle wall_floor(200, 150, wall_mat);
    wall_floor.rotate_x(PI / 2);
    scene.add_object(&wall_floor);

    Rectangle wall_back(200, 150, wall_mat);
    wall_back.translate({0, 75, 75});
    scene.add_object(&wall_back);

    Material sphere_mat;
    sphere_mat.specular = 0.2;
    sphere_mat.diffuse = 0.5;
    sphere_mat.ambient = 0.5;
    sphere_mat.shininess = 32;
    sphere_mat.reflectance = 1;
    sphere_mat.phong_factor = 0.97;

    sphere_mat.color = {0.3, 0.8, 0.7};
    Sphere sphere1(10, sphere_mat);
    sphere1.translate({0, 10.1, 0});
    scene.add_object(&sphere1);

    sphere_mat.color = {0.9, 0.55, 0.4};
    Sphere sphere2(5, sphere_mat);
    sphere2.translate({-20, 5.1, -3});
    scene.add_object(&sphere2);

    sphere_mat.color = {0.28, 0.35, 0.25};
    Sphere sphere3(20, sphere_mat);
    sphere3.translate({60, 20.1, 40});
    scene.add_object(&sphere3);

    sphere_mat.color = {0.28, 0.47, 0.6};
    Sphere sphere4(10, sphere_mat);
    sphere4.translate({-50, 10.1, -20});
    scene.add_object(&sphere4);

    Material sphere5_mat;
    sphere5_mat.specular = 0;
    sphere5_mat.diffuse = 0.7;
    sphere5_mat.ambient = 0.5;
    sphere5_mat.color = {0.96, 0.87, 0.5};
    Sphere sphere5(6, sphere5_mat);
    sphere5.translate({20, 6.1, -40});
    scene.add_object(&sphere5);

    Material mirror_mat;
    mirror_mat.reflectance = 1;
    mirror_mat.phong_factor = 0;
    Rectangle mirror(45, 30, mirror_mat);
    mirror.translate({-40, 17.5, 40});
    mirror.rotate_y(-PI / 6);
    scene.add_object(&mirror);

    Material mirror_back_mat;
    mirror_back_mat.color = {0.2, 0.2, 0.2};
    mirror_back_mat.specular = 0;
    mirror_back_mat.diffuse = 0;
    mirror_back_mat.ambient = 1;
    Rectangle mirror_back(50, 35, mirror_back_mat);
    mirror_back.translate({-40, 17.5, 40.1});
    mirror_back.rotate_y(-PI / 6);
    scene.add_object(&mirror_back);

    Material sphere6_mat;
    sphere6_mat.specular = 0.5;
    sphere6_mat.diffuse = 0.5;
    sphere6_mat.ambient = 0.5;
    sphere_mat.shininess = 32;
    sphere6_mat.reflectance = 1;
    sphere6_mat.phong_factor = 0.2;
    Sphere sphere6(12, sphere6_mat);
    sphere6.translate({25, 12.1, 30});
    scene.add_object(&sphere6);

    Material sphere7_mat;
    sphere7_mat.reflectance = 0.1;
    sphere7_mat.phong_factor = 0;
    Sphere sphere7(4, sphere7_mat);
    sphere7.interior_ior = 1.15;
    sphere7.translate({-2, 4.1, -30});
    scene.add_object(&sphere7);

    Material box1_mat;
    box1_mat.color = {1, 1, 1};
    box1_mat.specular = 0.5;
    box1_mat.diffuse = 0.5;
    box1_mat.ambient = 0.5;
    box1_mat.reflectance = 0.5;
    box1_mat.phong_factor = 0.1;
    Box box1(8, 8, 8, box1_mat);
    box1.interior_ior = 1.15;
    box1.translate({-30, 4.1, -30});
    scene.add_object(&box1);

    Material box2_mat;
    box2_mat.color = {0.8, 0.2, 0.2};
    box2_mat.specular = 0.5;
    box2_mat.diffuse = 0.5;
    box2_mat.ambient = 0.5;
    sphere_mat.shininess = 32;
    Box box2(10, 50, 10, box2_mat);
    box2.translate({80, 25.1, -50});
    scene.add_object(&box2);

    Material pyramid_mat;
    pyramid_mat.specular = 0;
    pyramid_mat.diffuse = 0.5;
    pyramid_mat.ambient = 0.8;
    pyramid_mat.reflectance = 0;
    pyramid_mat.phong_factor = 0.8;
    pyramid_mat.color = {0.2, 0.2, 0.3};

    float pyramid_base_size = 10;
    float pyramid_shrink_rate = 0.7;
    float pyramid_level_height = 1.5;
    float pyramid_level_gap = 1;
    int pyramid_level_count = 3;
    for (int i = 0; i < pyramid_level_count; i++) {
        float size = pyramid_base_size * pow(pyramid_shrink_rate, i);
        Box* pyramid_box = new Box(size, pyramid_level_height, size, pyramid_mat);
        float height = 0.1 + pyramid_level_height / 2 + i * (pyramid_level_height + pyramid_level_gap);
        pyramid_box->translate({-12, height, -45});
        scene.add_object(pyramid_box);
    }

    scene.capture("scene");

    return 0;

}
