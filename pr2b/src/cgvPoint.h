#pragma once

#include <array>

#define CGV_EPSILON 0.000001 // for comparisons with 0

#ifndef __ENUM_XYZ
#define __ENUM_XYZ

/**
 * Labels for the coordinates of the point/vector
 */
enum {
	X, ///< X coordinate
	Y, ///< Y coordinate
	Z, ///< Z coordinate
	W  ///< W coordinate
};
#endif


/**
 * The class cgvPoint3D implements the functionality of the objects Point and Vector in 3D 
 */
class cgvPoint3D {

	std::array<float, 3> c; ///< components x, y, z of a point or vector

	public:
		// Constructors
		cgvPoint3D(); 
		cgvPoint3D( const float& x, const float& y, const float& z );
		
		// Copy Constructor 
		cgvPoint3D( const cgvPoint3D& p );

		// Assignment operator
		cgvPoint3D& operator = (const cgvPoint3D& p);

		// Destructor
		~cgvPoint3D()=default;

		// Operators
		/** Write/read access to an element of the array
		 * @param idx the position of the element in the array
		 * @pre It is assumed that the value of the parameter is valid
		 * @return The corresponding coordinate
		 */
		inline float& operator[] ( const unsigned char idx ) {return c[idx];};
		/** Read access to an element of the array
		 */
		inline float operator[] (const unsigned char idx) const {return c[idx];};

		bool operator == (const cgvPoint3D& p);
		bool operator != (const cgvPoint3D& p);

		void set( const float& x, const float& y, const float& z);
		
		/**
		 * Method to get C-like array of the point/vector
		 * @return a pointer to the first element of the array
		 */
		float *data() { return c.data(); }
};

class cgvPoint4D {

	std::array<float, 4> c; ///< components x, y, z, w of a point or vector

public:
	// Constructors
	cgvPoint4D();
	cgvPoint4D(const float& x, const float& y, const float& z, const float& w = 1.0f);

	// Copy Constructor 
	cgvPoint4D(const cgvPoint4D& p);
	cgvPoint4D(const cgvPoint3D& p);


	// Assignment operator
	cgvPoint4D& operator = (const cgvPoint4D& p);

	// Destructor
	~cgvPoint4D()=default;

	// Operators
	/** Write/read access to an element of the array
	 * @param idx the position of the element in the array
	 * @pre It is assumed that the value of the parameter is valid
	 * @return The corresponding coordinate
	 */	
	inline float& operator[] (const unsigned char idx) { return c[idx]; };
	/** Write/read access to an element of the array **/
	inline float operator[] (const unsigned char idx) const { return c[idx]; };

	bool operator == (const cgvPoint4D& p);
	bool operator != (const cgvPoint4D& p);

	void set(const float& x, const float& y, const float& z, const float& w);

	/**
	 * Method to get C-like array of the point/vector
	 * @return a pointer to the first element of the array
	 */
	float *data() { return c.data(); }
};

