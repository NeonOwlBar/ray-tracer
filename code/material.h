#pragma once

#ifndef MATERIAL_H
#define MATERIAL_H

#include "hittable.h"

class material
{
public:
	// Compiler-generated destructor - releases no resources.
	virtual ~material() = default;

	virtual bool scatter(
		const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered
	) const
	{
		return false;
	}

};

class lambertian : public material
{
public:
	lambertian(const color& albedo) : albedo(albedo) {}

	// Calculates the scattered ray and attenuation
	bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered)
		const override
	{
		// get a random scatter direction centred around the surface normal vector
		auto scatter_direction = rec.normal + random_unit_vector();

		// Catch degenerate scatter direction
		if (scatter_direction.near_zero())
			scatter_direction = rec.normal;

		// scattered ray moves from point of intersection in random direction 
		// calculated above
		scattered = ray(rec.p, scatter_direction);
		// NOTE: WHAT IS THIS FOR??? 
		// * is attenuation the colour?
		// * is it the likelihood for a ray to be scattered and not absorbed?
		attenuation = albedo;
		return true;
	}

private:
	// Color of this material
	color albedo;
};

class metal : public material
{
public:
	metal(const color& albedo, double fuzz) : albedo(albedo), fuzz(fuzz < 1 ? fuzz : 1) {}
	
	bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered)
		const override
	{
		vec3 reflected = reflect(r_in.direction(), rec.normal);
		// Reflected vector is normalised and adjusted randomly depending on the fuzz value
		reflected = unit_vector(reflected) + (fuzz * random_unit_vector());
		// Scattered ray moves from point of intersection in reflected direction 
		// calculated above
		scattered = ray(rec.p, reflected);
		// NOTE: WHAT IS THIS FOR??? 
		// * is attenuation the colour?
		// * is it the likelihood for a ray to be scattered and not absorbed?
		attenuation = albedo;
		// Ignores rays that end up below the surface due to fuzz adjustments
		return (dot(scattered.direction(), rec.normal) > 0);
	}

private:
	// Color of this material
	color albedo;
	// Represents the roughness of a surface. More fuzz increases the "fuzzy" 
	// look in reflections, whereas a value of zero would create a specular surface
	double fuzz;
};

#endif