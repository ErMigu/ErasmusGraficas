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

public:
    //constructor
    Vec3 (const float x, const float y, const float z) {
        this->values[0] = x;
        this->values[1] = y;
        this->values[2] = z;
    }

    Vec3() {
        this->values[0] = 0;
        this->values[1] = 0;
        this->values[2] = 0;
    }

    //cccess
    float x() const { return values[0]; }
    float y() const { return values[1]; }
    float z() const { return values[2]; }

    //modify
    void x(float val) { values[0] = val; }
    void y(float val) { values[1] = val; }
    void z(float val) { values[2] = val; }
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





