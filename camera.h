#ifndef CAMERA_H
#define CAMERA_H

#include "hittable.h"
#include "rt.h"
#include "material.h"

class camera
{
public:
    double aspect_ratio = 1.0;
    int image_width = 100;
    int samples_per_pixel = 10;
    int max_ray_bounces = 10;

    double vfov = 90;
    vec3 vup = vec3(0, 1, 0);
    point3 lookfrom = point3(0, 0, 0);
    point3 lookat = point3(0, 0, -1);

    double focus_distance = 0;
    double defocus_angle = 10;

    void render(const hittable &world)
    {
        initialize();

        std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

        for (int j = 0; j < image_height; j++)
        {
            std::clog << "\rScanlines remaining: " << (image_height - j) << ' ' << std::flush;
            for (int i = 0; i < image_width; i++)
            {
                color pixel_color(0, 0, 0);
                for (int sample = 0; sample < samples_per_pixel; sample++)
                {
                    ray r = get_ray(i, j);
                    pixel_color += ray_color(r, max_ray_bounces, world);
                }

                write_color(std::cout, pixel_color / samples_per_pixel);
            }
        }

        std::clog << "\rDone.                 \n";
    }

private:
    int image_height;   // Rendered image height
    point3 center;      // Camera center
    point3 pixel00_loc; // Location of pixel 0, 0
    vec3 pixel_delta_u; // Offset to pixel to the right
    vec3 pixel_delta_v; // Offset to pixel below
    vec3 u, v, w;

    vec3 defocus_disc_u;
    vec3 defocus_disc_v;

    void initialize()
    {
        image_height = int(image_width / aspect_ratio);
        image_height = (image_height < 1) ? 1 : image_height;

        center = lookfrom;

        // Determine viewport dimensions.
        double theta = degrees_to_radians(vfov);
        double h = std::tan(theta / 2);

        auto viewport_height = 2.0 * h * focus_distance;
        auto viewport_width = viewport_height * (double(image_width) / image_height);

        w = unit_vector(lookfrom - lookat);
        u = unit_vector(cross(vup, w));
        v = cross(w, u);

        // Calculate the vectors across the horizontal and down the vertical viewport edges.
        auto viewport_u = viewport_width * u;
        auto viewport_v = viewport_height * -v;

        // Calculate the horizontal and vertical delta vectors from pixel to pixel.
        pixel_delta_u = viewport_u / image_width;
        pixel_delta_v = viewport_v / image_height;

        // Calculate the location of the upper left pixel.
        auto viewport_upper_left = center - (focus_distance * w) - viewport_u / 2 - viewport_v / 2;
        pixel00_loc = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);

        // Calculate focus disc
        auto defocus_radius = focus_distance * std::tan(degrees_to_radians(defocus_angle / 2));
        defocus_disc_u = u * defocus_radius;
        defocus_disc_v = v * defocus_radius;
    }

    point3 defocus_disc_sample() const
    {
        auto p = random_in_unit_disk();
        return center + (p[0] * defocus_disc_u) + (p[1] * defocus_disc_v);
    }

    ray get_ray(int i, int j) const
    {
        auto offset = get_offset();
        auto sample_pixel_center = pixel00_loc + ((i + offset.x()) * pixel_delta_u) + ((j + offset.y()) * pixel_delta_v);
        auto ray_origin = (defocus_angle <= 0) ? center : defocus_disc_sample();
        auto ray_direction = sample_pixel_center - ray_origin;
        ray r(ray_origin, ray_direction);
        return r;
    }

    vec3 get_offset() const
    {
        // Returns the vector to a random point in the [-.5,-.5]-[+.5,+.5] unit square.
        return vec3(random_double() - 0.5, random_double() - 0.5, 0);
    }

    auto lerp(const color &a, const color &b, double tVal) const
    {
        return (1 - tVal) * a + tVal * b;
    }

    color ray_color(const ray &r, int depth, const hittable &world) const
    {
        if (depth <= 0)
        {
            return color(0, 0, 0);
        }

        hit_record rec;
        if (world.hit(r, interval(0.001, infinity), rec))
        {
            ray scattered;
            color attenuation;
            if (rec.mat->scatter(r, rec, attenuation, scattered))
            {
                return attenuation * ray_color(scattered, depth - 1, world);
            }
            return color(0, 0, 0);
        }

        auto unitVec = unit_vector(r.direction());
        auto a = 0.5 * (unitVec.y() + 1);
        return lerp(color(1, 1, 1), color(0.5, 0.7, 1), a);
    }
};

#endif