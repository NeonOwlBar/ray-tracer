#pragma once

#ifndef VEC3_H
#define VEC3_H

/// <summary>
/// Used for colours, locations, directions, offsets, etc.
/// </summary>
class vec3
{
public:
    // Stores three values
    double e[3];

    // Default constructor
    vec3() : e{ 0,0,0 } {}
    // Constructor
    vec3(double e0, double e1, double e2) : e{ e0, e1, e2 } {}

    // Return each value of e respectively
    double x() const { return e[0]; }
    double y() const { return e[1]; }
    double z() const { return e[2]; }

    // Returns the negative vector
    vec3 operator-() const { return vec3(-e[0], -e[1], -e[2]); }

    // The below operator[] functions are used on different types of vec3 objects.
    // A const vec3 cannot be modified and is read-only, 
    //		and therefore can only return a COPY of a value.
    // A regular, non-const vec3 CAN be modified (read/write), 
    //		and therefore returns a REFERENCE to the value.
    // 
    // Returns a copy of the value at index i. Read-only.
    double operator[](int i) const { return e[i]; }
    // Returns a reference to the value at index i. Read/write.
    double& operator[](int i) { return e[i]; }

    // Adds a passed vector's values to this object's current values
    vec3& operator +=(const vec3& v)
    {
        e[0] += v.e[0];
        e[1] += v.e[1];
        e[2] += v.e[2];
        // Returns the object
        // ("this" returns a pointer to an object, therefore "*this" 
        //		dereferences the pointer, returning the object itself).
        return *this;
    }

    // Multiplies this object's vector values by a passed scalar value
    vec3& operator*=(double t)
    {
        e[0] *= t;
        e[1] *= t;
        e[2] *= t;
        return *this;
    }

    // Divides this object's vector values by a passed scalar value
    vec3& operator/=(double t)
    {
        return *this *= 1/t;
    }

    // Returns the square root of the length squared (Pythagoras' theorem)
    double length() const
    {
        // this is just sqrt(c) after using a^2 + b^2 = c^2
        return std::sqrt(length_squared());
    }

    // Returns the resulting scalar of the sum of each index multiplied by itself
    double length_squared() const
    {
        // a^2 + b^2 + c^2 = ...d^2?
        return e[0]*e[0] + e[1]*e[1] + e[2]*e[2];
    }

    bool near_zero() const
    {
        // Return true if the vector is close to zero in ALL dimensions.
        auto s = 1e-8; // 10^(-8)
        return (std::fabs(e[0]) < s) && (std::fabs(e[1]) < s) && (std::fabs(e[2]) < s);
    }

    // Returns a random vector from where each value is [0, 1) 
    static vec3 random()
    {
        return vec3(random_double(), random_double(), random_double());
    }

    // Returns a random vector where each value is [min, max)
    static vec3 random(double min, double max)
    {
        return vec3(random_double(min, max), random_double(min, max), random_double(min, max));
    }
};

// point3 is just an alias for vec3, but useful for geometric clarity in the code. (see ray class for an example)
using point3 = vec3;


// Vector Utility Functions - Define how other types can interact with a vec3

// NOTE: "The "inline" keyword suggests that the compiler substitute the code within
// the function definition in place of each call to that function."
//	*	In theory, this makes the program faster due to not needing to call the function,
//		as the function call is replaced by the code within the function.
//	*	The compiler decides which functions to inline. It may consider a function too large to be inlined.
//	*	A function defined in the body of a class declaration (like all those above) is *implicitly* an inline function.
//		Therefore, the keyword is required here because it is outside the body of the class


// Overloads the bitwise left shift operator as a streaming operator.
// Allows all 3 values within a vec3 to be streamed into the out variable, separated by a space.
// e.g.:
//		vec3 vector;			// calls default constructor
//		std::cout << output;	// prints: 0 0 0 
inline std::ostream& operator<<(std::ostream& out, const vec3& v)
{
    return out << v.e[0] << ' ' << v.e[1] << ' ' << v.e[2];
}

// Overrides the '+' operator to allow components of two vectors to be added together
inline vec3 operator+(const vec3& u, const vec3& v)
{
    return vec3(u.e[0] + v.e[0], u.e[1] + v.e[1], u.e[2] + v.e[2]);
}

// Overrides the '-' operator to allow components of one vector to be subtracted from another
inline vec3 operator-(const vec3& u, const vec3& v)
{
    return vec3(u.e[0] - v.e[0], u.e[1] - v.e[1], u.e[2] - v.e[2]);
}

// Overrides the '*' operator to allow multiplication of two vectors
inline vec3 operator*(const vec3& u, const vec3& v)
{
    return vec3(u.e[0] * v.e[0], u.e[1] * v.e[1], u.e[2] * v.e[2]);
}

// Overrides the '*' operator to allow a vector to be multiplied by a scalar
inline vec3 operator*(double t, const vec3& v)
{
    return vec3(t*v.e[0], t*v.e[1], t*v.e[2]);
}

// Overrides the '*' operator to allow multiplication when order of variables is swapped
inline vec3 operator*(const vec3& v, double t)
{
    // Calls function above
    return t * v;
}

// Overrides the '/' operator to allow a vector to be divided by a scalar value
inline vec3 operator/(const vec3& v, double t)
{
    return (1 / t) * v;
}

// Find the dot product of two given vectors
inline double dot(const vec3& u, const vec3& v)
{
    return u.e[0] * v.e[0]
         + u.e[1] * v.e[1]
         + u.e[2] * v.e[2];
}

// Find the cross product of two given vectors
inline vec3 cross(const vec3& u, const vec3& v)
{
    return vec3(u.e[1] * v.e[2] - u.e[2] * v.e[1],
                u.e[2] * v.e[0] - u.e[0] * v.e[2], 
                u.e[0] * v.e[1] - u.e[1] * v.e[0]);
}

// Returns unit vector of passed vector by dividing self by its length
inline vec3 unit_vector(const vec3& v)
{
    return v / v.length();
}

// Returns a random unit vector (surprise!)
inline vec3 random_unit_vector()
{
    while (true)
    {
        // Generate a random vector
        auto p = vec3::random(-1, 1);
        // Anything with a squared length >1 will not be within unit sphere
        auto lensq = p.length_squared();
        // Due to floating-points having finite precision, a very samll value ca
        //  can underflow to zero when squared. If all three values are small enough, 
        //  the normal will be zero and the vector will become [+/-infinity, +/-infinity, +/-infinity].
        // Therefore reject vectors within this length. As we are using doubles, 
        //  these can safely store values greater than 10^(-160)
        if (1e-160 < lensq && lensq <= 1)
            return p / sqrt(lensq);
    }
}

// Returns a random unit vector in the same hemisphere as the provided normal
inline vec3 random_on_hemisphere(const vec3& normal)
{
    // Generate random unit vector
    vec3 on_unit_sphere = random_unit_vector();

    // If unit vector is in same hemisphere as the normal, return it. Otherwise, return it inverted.
    if (dot(on_unit_sphere, normal) > 0.0)
        return on_unit_sphere;
    else
        return -on_unit_sphere;
}


// Returns the perfectly reflected vector
inline vec3 reflect(const vec3& v, const vec3& n)
{
    // v = vector of incident ray
    // n = surface normal
    // Full explanation: https://raytracing.github.io/books/RayTracingInOneWeekend.html#metal/mirroredlightreflection
    // Diagram only: https://raytracing.github.io/images/fig-1.15-reflection.jpg
    
    // In the form v + 2b = reflectedDirection
    return v - 2*dot(v, n)*n;
}

#endif
