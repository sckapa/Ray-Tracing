#ifndef MATERIAL_H
#define MATERIAL_H

#include "hittable.h"

class material
{
public:
    virtual ~material() = default;

    virtual bool scatter(const ray &r_in, const hit_record &rec, color &attenuation, ray &scattered) const
    {
        return false;
    }
};

class lambertian : public material
{
public:
    lambertian(const color &albedo) : albedo(albedo) {}

    bool scatter(const ray &r_in, const hit_record &rec, color &attenuation, ray &scattered) const override
    {
        vec3 direction = rec.normal + random_unit_vector();

        if (direction.near_zero())
        {
            direction = rec.normal;
        }

        scattered = ray(rec.p, direction);
        attenuation = albedo;
        return true;
    }

private:
    color albedo;
};

class metal : public material
{
public:
    metal(const color &albedo, double fuzz) : albedo(albedo), fuzz(fuzz) {}

    bool scatter(const ray &r_in, const hit_record &rec, color &attenuation, ray &scattered) const override
    {
        auto direction = reflect(r_in.direction(), rec.normal);
        direction = unit_vector(direction) + (fuzz * random_unit_vector());
        scattered = ray(rec.p, direction);
        attenuation = albedo;
        return dot(scattered.direction(), rec.normal) > 0;
    }

private:
    double fuzz;
    color albedo;
};

class dielectric : public material
{
public:
    dielectric(double refraction_index) : refraction_index(refraction_index) {}

    bool scatter(const ray &r_in, const hit_record &rec, color &attenuation, ray &scattered) const override
    {
        attenuation = color(1, 1, 1);

        double ri = rec.front_face ? (1 / refraction_index) : refraction_index;
        auto unit_direction = unit_vector(r_in.direction());

        auto cos_theta = std::fmin(dot(-unit_direction, rec.normal), 1);
        auto sine_theta = std::sqrt(1 - cos_theta * cos_theta);
        vec3 direction;
        bool cannot_refract = ri * sine_theta > 1;

        if (cannot_refract || reflectance(cos_theta, ri) > random_double())
        {
            direction = reflect(unit_direction, rec.normal);
        }
        else
        {
            direction = refract(unit_direction, rec.normal, ri);
        }

        scattered = ray(rec.p, direction);
        return true;
    }

private:
    double refraction_index;

    static double reflectance(double cosine, double refraction_index)
    {
        // Use Schlick's approximation for reflectance.
        auto r0 = (1 - refraction_index) / (1 + refraction_index);
        r0 = r0 * r0;
        return r0 + (1 - r0) * std::pow((1 - cosine), 5);
    }
};
#endif