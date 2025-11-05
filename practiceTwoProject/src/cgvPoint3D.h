#ifndef __IGVPUNTO3D
#define __IGVPUNTO3D

#if defined(__APPLE__) && defined(__MACH__)
#include <GLUT/glut.h>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else

#include <GL/glut.h>

#endif   // defined(__APPLE__) && defined(__MACH__)

#define IGV_EPSILON 0.000001 ///< Constante para comparaciones con 0

#ifndef __ENUM_XYZ
#define __ENUM_XYZ

/**
 * Labels for point/vector coordinates
 */
enum Coordinates
{  X   ///< X coordinate
   , Y   ///< Y coordinate
   , Z   ///< Z coordinate
};
#endif   // __ENUM_XYZ

/**
 * Objects of this class represent points and vectors in 3D
 */
class cgvPoint3D
{  private:
    double c[3] = {0,0,0}; ///< x, y, z components of the point or vector

public:
    // Constructors
    /// Default constructor
    cgvPoint3D () = default;
    cgvPoint3D ( const double &x, const double &y, const double &z );
    // Copy constructor
    cgvPoint3D ( const cgvPoint3D &p );

    // Assignment operator
    cgvPoint3D &operator= ( const cgvPoint3D &p );

    /// Destructor
    ~cgvPoint3D () = default;

    // Operators
    double &operator[] ( const unsigned char idx );

    double operator[] ( const unsigned char idx ) const;

    bool operator== ( const cgvPoint3D &p );

    bool operator!= ( const cgvPoint3D &p );

    void set ( const double &x, const double &y, const double &z );
};

/**
 * Access to a point/vector coordinate
 * @param idx Coordinate to be accessed (0, 1 or 2)
 * @return The corresponding coordinate
 * @pre It is assumed that the parameter value is correct
 */
inline double &cgvPoint3D::operator[] ( const unsigned char idx )
{  return c[idx];
}

/**
 * Access to a point/vector coordinate
 * @param idx Coordinate to be accessed (0, 1, or 2)
 * @return The corresponding coordinate
 * @pre It is assumed that the parameter value is correct
 */
inline double cgvPoint3D::operator[] ( const unsigned char idx ) const
{  return c[idx];
}
#endif   // __CGVPOINT3D

