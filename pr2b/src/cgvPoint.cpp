#include <stdio.h>
#include <math.h>

#include "cgvPoint.h"

/** 
* Basic constructor
* @post The values of the coordinates is 0.  
*/
cgvPoint3D::cgvPoint3D() {
	c[X] = c[Y] = c[Z] = 0.0;
}

/** 
* Constructor
* @param x X coordinate of the point/vector
* @param y Y coordinate of the point/vector
* @param z Z coordinate of the point/vector
* @post The values of the coordinates becomes the same as the parameters.  
*/
cgvPoint3D::cgvPoint3D (const float& x, const float& y, const float& z ) {
	c[X] = x;
	c[Y] = y;
	c[Z] = z;	
}

/** 
* Copy constructor 
* @param p Point/vector
* @post The coordinates of the point/vector becomes the same as the parameter 
*/
cgvPoint3D::cgvPoint3D (const cgvPoint3D& p ) {
	c[X] = p.c[X];
	c[Y] = p.c[Y];
	c[Z] = p.c[Z];
}

/**
 * Assignment operator 
 * @param p Point/vector
 * @return A new point/vector with the same coordinates as the original
 */
cgvPoint3D& cgvPoint3D::operator = (const cgvPoint3D& p) {
	c[X] = p.c[X];
	c[Y] = p.c[Y];
	c[Z] = p.c[Z];
	return(*this);
}

/**
 * Equality operator
 * @param p The point/vector to compare with
 * @retval True if the point/vector is identical to the current one. False otherwise. Tolerance threshold CGV_EPSILON
 */
bool cgvPoint3D::operator == (const cgvPoint3D& p) {
	return ((fabs(c[X]-p[X])<CGV_EPSILON) && (fabs(c[Y]-p[Y])<CGV_EPSILON) && (fabs(c[Z]-p[Z])<CGV_EPSILON));
}

/**
 * Inequality operator
 * @param p The point/vector to compare with
 * @retval True if the point/vector is different to the current one. False otherwise. Tolerance threshold CGV_EPSILON
 */
bool cgvPoint3D::operator != (const cgvPoint3D& p) {
	return ((fabs(c[X]-p[X])>=CGV_EPSILON) || (fabs(c[Y]-p[Y])>=CGV_EPSILON) || (fabs(c[Z]-p[Z])>=CGV_EPSILON));
}

/** 
* Set method
* @param x X coordinate of the point/vector
* @param y Y coordinate of the point/vector
* @param z Z coordinate of the point/vector
* @post The values of the coordinates becomes the same as the parameters.  
*/
void cgvPoint3D::set( const float& x, const float& y, const float& z) {
	c[X] = x;
	c[Y] = y;
	c[Z] = z;
}


/////////////////////////////////////////////////////////////////////////////////////

/** 
* Basic constructor
* @post The values of the coordinates is 0, except w that becomes 1.  
*/
cgvPoint4D::cgvPoint4D() {
	c[X] = c[Y] = c[Z] = 0.0f; 
	c[W] = 1.0f;
}

/** 
* Constructor
* @param x X coordinate of the point/vector
* @param y Y coordinate of the point/vector
* @param z Z coordinate of the point/vector
* @param w W coordinate of the point/vector
* @post The values of the coordinates becomes the same as the parameters.  
*/
cgvPoint4D::cgvPoint4D(const float& x, const float& y, const float& z, const float& w) {
	c[X] = x;
	c[Y] = y;
	c[Z] = z;
	c[W] = w; 
}

/** 
* Copy constructor 
* @param p Point/vector
* @post The coordinates of the point/vector becomes the same as the parameter 
*/
cgvPoint4D::cgvPoint4D(const cgvPoint4D& p) {
	c[X] = p.c[X];
	c[Y] = p.c[Y];
	c[Z] = p.c[Z];
	c[W] = p.c[W];
}

/** 
* Constructor from a 3D point
* @param p 3D Point/vector
* @post The coordinates of the point/vector becomes the same as the parameter and the w coordinates becomes 1. 
*/
cgvPoint4D::cgvPoint4D(const cgvPoint3D& p) {
	c[X] = p[X];
	c[Y] = p[Y];
	c[Z] = p[Z];
	c[W] = 1.0f; 
}

/**
 * Assignment operator 
 * @param p Point/vector
 * @return A new point/vector with the same coordinates as the original
 */
cgvPoint4D& cgvPoint4D::operator = (const cgvPoint4D& p) {
	c[X] = p.c[X];
	c[Y] = p.c[Y];
	c[Z] = p.c[Z];
	c[W] = p.c[W];
	return(*this);
}

/**
 * Equality operator
 * @param p The point/vector to compare with
 * @retval True if the point/vector is identical to the current one. False otherwise. Tolerance threshold CGV_EPSILON
 */
bool cgvPoint4D::operator == (const cgvPoint4D& p) {
	return ((fabs(c[X] - p[X]) < CGV_EPSILON) && (fabs(c[Y] - p[Y]) < CGV_EPSILON) && (fabs(c[Z] - p[Z]) < CGV_EPSILON) && (fabs(c[W] - p[W]) < CGV_EPSILON));
}

/**
 * Inequality operator
 * @param p The point/vector to compare with
 * @retval True if the point/vector is different to the current one. False otherwise. Tolerance threshold CGV_EPSILON
 */
bool cgvPoint4D::operator != (const cgvPoint4D& p) {
	return ((fabs(c[X] - p[X]) >= CGV_EPSILON) || (fabs(c[Y] - p[Y]) >= CGV_EPSILON) || (fabs(c[Z] - p[Z]) >= CGV_EPSILON) || (fabs(c[W] - p[W]) >= CGV_EPSILON));
}

/** 
* Set method
* @param x X coordinate of the point/vector
* @param y Y coordinate of the point/vector
* @param z Z coordinate of the point/vector
* @param w W coordinate of the point/vector
* @post The values of the coordinates becomes the same as the parameters.  
*/
void cgvPoint4D::set(const float& x, const float& y, const float& z, const float& w) {
	c[X] = x;
	c[Y] = y;
	c[Z] = z;
	c[W] = w; 
}


