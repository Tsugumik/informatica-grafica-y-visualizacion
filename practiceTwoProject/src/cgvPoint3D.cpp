#include <stdio.h>
#include <math.h>

#include "cgvPoint3D.h"

// Constructors
/**
 * Parameterised constructor
 * @param x X coordinate of the point/vector
 * @param y Y coordinate of the point/vector
 * @param z Z coordinate of the point/vector
 * @post The coordinates of the new point/vector will be those passed as parameters
 */
cgvPoint3D::cgvPoint3D ( const double &x, const double &y, const double &z ):
                                                           c{x,y,z }
{ }

/**
 * Copy constructor
 * @param p Point/vector from which the coordinates are copied
 * @post The coordinates of the new point/vector will be the same as those of the original
 */
cgvPoint3D::cgvPoint3D ( const cgvPoint3D &p ): c{p.c[X], p.c[Y], p.c[Z]}
{ }

/**
 * Assignment operator
 * @param p Point/vector from which the coordinates are copied
 * @return The modified point/vector itself
 * @post The coordinates of the point/vector become the same as those of the original
 */
cgvPoint3D &cgvPoint3D::operator= ( const cgvPoint3D &p )
{  c[X] = p.c[X];
    c[Y] = p.c[Y];
    c[Z] = p.c[Z];
    return *this;
}

/**
 * Equality operator
 * @param p Point/vector with which to compare
 * @retval true If the point/vector is equal to the one passed as a parameter, with
 *         a tolerance given by IGV_EPSILON
 */
bool cgvPoint3D::operator== ( const cgvPoint3D &p )
{  return ( ( fabs ( c[X] - p[X] ) < IGV_EPSILON )
            && ( fabs ( c[Y] - p[Y] ) < IGV_EPSILON )
            && ( fabs ( c[Z] - p[Z] ) < IGV_EPSILON )
          );
}

/**
 * Not equal operator
 * @param p Point/vector to compare with
 * @retval true If the point/vector is not equal to the one passed as a parameter, with
 *         a tolerance given by IGV_EPSILON
 */
bool cgvPoint3D::operator!= ( const cgvPoint3D &p )
{  return ( ( fabs ( c[X] - p[X] ) >= IGV_EPSILON )
            || ( fabs ( c[Y] - p[Y] ) >= IGV_EPSILON )
            || ( fabs ( c[Z] - p[Z] ) >= IGV_EPSILON )
          );
}
/**
* Assignment of the three coordinates of the point/vector
* @param x Value for the X coordinate
* @param y Value for the Y coordinate
* @param z Value for the Z coordinate
* @post The coordinates of the point/vector change to those passed as the
* parameter
*/
void cgvPoint3D::set ( const double &x, const double &y, const double &z )
{
    c[X] = x;
    c[Y] = y;
    c[Z] = z;
}