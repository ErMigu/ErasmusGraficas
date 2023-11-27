/*
 *  Computer Graphics course
 *  Dept Computing Science, Umea University
 *  Stefan Johansson, stefanj@cs.umu.se
 */
#pragma once

#include <iostream>
#include <fstream>
#include <memory>
#include <cmath>
#include <vector>

// Macro for index to buffer pointer conversion
#define BUFFER_OFFSET(i) (reinterpret_cast<char*>(0 + (i)))

struct Vec3 {
    float values[3];
    float *px=0;
    float *py=0;
    float *pz=0;

public:
    //constructor
    Vec3 (const float x, const float y, const float z) {
        this->values[0] = x;
        this->values[1] = y;
        this->values[2] = z;
        this->px=&values[0];
        this->py=&values[1];
        this->pz=&values[2];
    }

    Vec3() {
        this->values[0] = 0;
        this->values[1] = 0;
        this->values[2] = 0;
        this->px=&values[0];
        this->py=&values[1];
        this->pz=&values[2];
    }

    //acccess
    float x() const { return values[0]; }
    float y() const { return values[1]; }
    float z() const { return values[2]; }

    //modify
    void x(float val) { values[0] = val; }
    void y(float val) { values[1] = val; }
    void z(float val) { values[2] = val; }

    Vec3 operator-(const Vec3& other) const {
        return {values[0] - other.values[0], values[1] - other.values[1], values[2] - other.values[2]};
    }

    void operator=(const Vec3& other) {
        values[0] = other.values[0];
        values[1] = other.values[1];
        values[2] = other.values[2];
    }

    // Normalizar el vector
    Vec3 normalize() const {
        float len = std::sqrt(*px * *px + *py * *py + *pz * *pz);
        return {*px / len, *py / len, *pz / len};
    }

    // Producto vectorial
    static Vec3 cross(const Vec3& a, const Vec3& b) {
        return {
                a.values[1] * b.values[2] - a.values[2] * b.values[1],
                a.values[2] * b.values[0] - a.values[0] * b.values[2],
                a.values[0] * b.values[1] - a.values[1] * b.values[0]
        };
    }

    // Producto escalar
    static float dot(const Vec3& a, const Vec3& b) {
        return a.values[0] * b.values[0] + a.values[1] * b.values[1] + a.values[2] * b.values[2];
    }
};

// *************
// Debug macros

/*
 * Usage:
 * string s = "foo";
 * DEBUG( "Value of s is '" << s << "'");
 * -> DBG: Value of s is 'foo'
 */
#define DEBUG( msg ) \
    std::cerr << "DBG " << __FILE__ << "(" << __LINE__ << "): " << msg << std::endl

/*
 * Usage:
 * string s = "foo";
 * DEBUGVAR( s );
 * -> DBG: testfile.cpp(20) s = foo
 */
#define DEBUGVAR( var )\
    std::cerr << "DBG " << __FILE__ << "(" << __LINE__ << "): " \
    << #var << " = " << (var) << std::endl

// *************





