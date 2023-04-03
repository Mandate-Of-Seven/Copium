/*!***************************************************************************************
\file			math-library.h
\project
\author			Matthew Lau

\par			Course: GAM250
\par			Section:
\date			02/09/2022

\brief
	Contains declarations for math library.
	The math library contains:
	1. 2D vector
	2. 3D vector
	3. 3x3 matrix
	4. 4x4 matrix
	Each has a class dedicated to it which contains operator overloads.
	Declarations of functions which perform common mathematical operations for the respective classes
	are also contained here.

All content © 2023 DigiPen Institute of Technology Singapore. All rights reserved.
******************************************************************************************/
#ifndef MATHLIBRARY_H
#define MATHLIBRARY_H

#include <math.h>
#include <iostream>
#include <rttr/type>
#include <glm/vec2.hpp>
#include <glm/mat3x3.hpp>
#include "Math/Vector4.h"

#pragma warning(disable : 4201)

namespace Copium::Math 
{

	//Ignore this, for debug purposes
	class myint 
	{
	public:
		myint();
		myint(int32_t _i);
		int32_t i;
		RTTR_ENABLE();
	};

	union Vec3;

	// Vec2 ////////////////////
	union Vec2 {
	public:
		struct
		{
			float x, y;
		};
		float data[2];
		glm::vec2 glmVec2;

		// Constructors
		Vec2();
		Vec2(float _x, float _y);
		Vec2(glm::vec2& _v);
		Vec2(const Vec2& _src);

		// Copy assignment operator overload
		Vec2& operator= (const Vec2& _rhs)
		{
			x = _rhs.x;
			y = _rhs.y;
			return *this;
		}

		// Compound assignment operators
		/*******************************************************************************
		/*!
		*
		\brief
			+= operator overload. Perform compound vector addition on this Vec2

		\param _rhs
			reference to the Vec2 which will be subtracted off this Vec2

		\return
			reference to this Vec2
		*/
		/*******************************************************************************/
		Vec2& operator+= (const Vec2& _rhs);
		/*******************************************************************************
		/*!
		*
		\brief
			-= operator overload. Perform compound vector subtraction on this Vec2

		\param _rhs
			reference to the Vec2 which will be subtracted off this Vec2

		\return
			reference to this Vec2
		*/
		/*******************************************************************************/
		Vec2& operator-= (const Vec2& _rhs);
		/*******************************************************************************
		/*!
		*
		\brief
			*= operator overload. Perform compound scalar multiplication on each element in this Vec2.

		\param _rhs
			the constant which will be used in the compound multiplication

		\return
			reference to this Vec2
		*/
		/*******************************************************************************/
		Vec2& operator*= (float _rhs);
		/*******************************************************************************
		/*!
		*
		\brief
			/= operator overload. Perform compound scalar division on each element in this Vec2.

		\param _rhs
			the constant which will be used in the compound division

		\return
			reference to this Vec2
		*/
		/*******************************************************************************/
		Vec2& operator/= (float _rhs);

		// Unary operators
		/*******************************************************************************
		/*!
		*
		\brief
			- unary operator overload. Make a Vec2 that is a copy of this Vec2 but with opposite signs.

		\return
			the resultant Vec2
		*/
		/*******************************************************************************/
		Vec2 operator -() const;

		// Comparison operators
		bool operator==(const Vec2& _rhs);
		bool operator!=(const Vec2& _rhs);

		/*******************************************************************************
		/*!
		*
		\brief
			Deserialize this Vec2's data from the specified rapidjson Value

		\param _value
			reference to the rapidJson value to deserialize from

		\return
			on success, return true
			on failure, return false
		*/
		/*******************************************************************************/
		//bool deserialize(rapidjson::Value& _value);
		/*******************************************************************************
		/*!
		*
		\brief
			Serialize this Vec2's data to the specified rapidjson Value

		\param _value
			reference to the rapidJson value to serialize to

		\param _doc
			reference to the rapidJson Document associated with the save file

		\return
			on success, return true
			on failure, return false
		*/
		/*******************************************************************************/
		//bool serialize(rapidjson::Value& _value, rapidjson::Document& _doc);

		// Conversion operators		
		/*******************************************************************************
		/*!
		*
		\brief
			Creates a glm::vec2 and copies the data from this Vec2 into the glm::vec2

		\return
			the resulting glm::vec2
		*/
		/*******************************************************************************/
		glm::vec2 to_glm() const;
		/*******************************************************************************
		/*!
		*
		\brief
			Returns this Vec2 as a glm::vec2

		\return
			this Vec2 as a glm::vec2
		*/
		/*******************************************************************************/
		operator glm::vec2();
	};

	// Vec2 Binary Operators
	/*******************************************************************************
	/*!
	*
	\brief
		+ operator overload. Performs vector addition between two Vec2s

	\param _lhs
		the left hand operand

	\param _rhs
		the right hand operand

	\return
		the resultant Vec2
	*/
	/*******************************************************************************/
	Vec2 operator+ (const Vec2& lhs, const Vec2& rhs);
	/*******************************************************************************
	/*!
	*
	\brief
		- operator overload. Performs vector subtraction between two Vec2s

	\param _lhs
		the left hand operand

	\param _rhs
		the right hand operand

	\return
		the resultant Vec2
	*/
	/*******************************************************************************/
	Vec2 operator- (const Vec2& _lhs, const Vec2& _rhs);
	/*******************************************************************************
	/*!
	*
	\brief
		* operator overload. Performs scalar multiplication on each element of a Vec2

	\param _lhs
		reference to the Vec2

	\param _rhs
		the constant to be used in the multiplication

	\return
		the resultant Vec2
	*/
	/*******************************************************************************/
	Vec2 operator* (const Vec2& _lhs, float _rhs);
	/*******************************************************************************
	/*!
	*
	\brief
		* operator overload. Performs scalar multiplication on each element of a Vec2

	\param _lhs
		the constant to be used in the multiplication

	\param _rhs
		reference to the Vec2

	\return
		the resultant Vec2
	*/
	/*******************************************************************************/
	Vec2 operator* (float _lhs, const Vec2& _rhs);
	/*******************************************************************************
	/*!
	*
	\brief
		/ operator overload. Performs scalar division on each element of a Vec2

	\param _lhs
		reference to the Vec2

	\param _rhs
		the constant to be used in the division

	\return
		the resultant Vec2
	*/
	/*******************************************************************************/
	Vec2 operator/ (const Vec2& _lhs, float _rhs);

	// Vec2 Functions
	/*******************************************************************************
	/*!
	*
	\brief
		Compute the normalized vector of a Vec2 and store the result in another Vec2.

	\param _result
		reference to the Vec2 in which the computed normalized Vec2 is stored in

	\param _src
		reference to the Vec2 whose normalized vector is to be computed

	\return
		void
	*/
	/*******************************************************************************/
	void vec2_normalize(Vec2& _result, const Vec2& _src);
	/*******************************************************************************
	/*!
	*
	\brief
		Compute the length of a Vec2

	\param _v
		reference to the Vec2 whose length is to be computed

	\return
		the computed length of the Vec2
	*/
	/*******************************************************************************/
	float vec2_length(const Vec2& _v);
	/*******************************************************************************
	/*!
	*
	\brief
		Compute the square length of a Vec2

	\param _v
		reference to the Vec2 whose square length is to be computed

	\return
		the computed square length of the Vec2
	*/
	/*******************************************************************************/
	float vec2_squarelength(const Vec2& _v);
	/*******************************************************************************
	/*!
	*
	\brief
		Compute the distance between two Vec2s

	\param _v1
		the left hand operand

	\param _v2
		the right hand operand

	\return
		the computed distance between the two Vec2s
	*/
	/*******************************************************************************/
	float vec2_distance(const Vec2& _v1, const Vec2& _v2);
	/*******************************************************************************
	/*!
	*
	\brief
		Compute the squared distance between two Vec2s

	\param _v1
		the left hand operand

	\param _v2
		the right hand operand

	\return
		the computed squared distance between the two Vec2s
	*/
	/*******************************************************************************/
	float vec2_squaredistance(const Vec2& _v1, const Vec2& _v2);
	/*******************************************************************************
	/*!
	*
	\brief
		Compute the dot product between two Vec2s.

	\param _v1
		the left hand operand

	\param _v2
		the right hand operand

	\return
		the result of the dot product
	*/
	/*******************************************************************************/
	float vec2_dotproduct(const Vec2& _v1, const Vec2& _v2);
	/*******************************************************************************
	/*!
	*
	\brief
		Computes the cross product between two Vec2s. The result will be a constant(magnitude).

	\param _v1
		the left hand operand

	\param _v2
		the right hand operand

	\return
		the computed magnitude of the cross product
	*/
	/*******************************************************************************/
	float vec2_crossproductmag(const Vec2& _v1, const Vec2& _v2);
	/*******************************************************************************
	/*!
	*
	\brief
		<< operator overload. Allows for Vec2 to be printed to std::cout

	\param _os
		reference to the output stream

	\param _v
		reference to the Vec2 that is to be printed

	\return
		reference to the output stream
	*/
	/*******************************************************************************/
	std::ostream& operator<<(std::ostream& _os, const Vec2& _v);
	////////////////////////////

	// Vec3 ////////////////////
	union Vec3 {
		struct
		{
			float x, y, z;
		};
		float data[3];
		glm::vec3 glmVec3;

		// Constructors
		Vec3();
		Vec3(float _x, float _y, float _z);
		Vec3(glm::vec3& _v);
		Vec3(const Vec3& _src);

		// Copy assignment operator overload
		Vec3& operator= (const Vec3& _rhs)
		{
			x = _rhs.x;
			y = _rhs.y;
			z = _rhs.z;
			return *this;
		}

		// Compound assignment operator overloads
		/*******************************************************************************
		/*!
		*
		\brief
			+= operator overload. Perform compound vector addition on this Vec3

		\param _rhs
			reference to the Vec3 which will be used in the compound addition

		\return
			reference to this Vec3
		*/
		/*******************************************************************************/
		Vec3& operator+= (const Vec3& _rhs);
		/*******************************************************************************
		/*!
		*
		\brief
			-= operator overload. Perform compound vector subtraction on this Vec3

		\param _rhs
			reference to the Vec3 which will be used to subtract off from this Vec3

		\return
			reference to this Vec3
		*/
		/*******************************************************************************/
		Vec3& operator-= (const Vec3& _rhs);
		/*******************************************************************************
		/*!
		*
		\brief
			*= operator overload. Perform compound multiplication on each element in this Vec3.

		\param _rhs
			the constant which will be used in the compound multiplication

		\return
			reference to this Vec3
		*/
		/*******************************************************************************/
		Vec3& operator*= (float _rhs);

		Vec3& operator*= (const Vec3& _rhs);


		/*******************************************************************************
		/*!
		*
		\brief
			/= operator overload. Perform compound division on each element in this Vec3.

		\param _rhs
			the constant which will be used in the compound division

		\return
			reference to this Vec3
		*/
		/*******************************************************************************/
		Vec3& operator/= (float _rhs);

		// Unary operator overloads
		/*******************************************************************************
		/*!
		*
		\brief
			- unary operator overload. Compute and return a copy of this Vec3 with values with opposite signs

		\return
			the resultant Vec3
		*/
		/*******************************************************************************/
		Vec3 operator- () const;

		/*******************************************************************************
		/*!
		*
		\brief
			* operator overload.
			
		\param	_rhs
			read-only reference to the right side Vec3 operand

		\return
			the resultant Vec3
		*/
		/*******************************************************************************/
		Vec3 operator* (const Vec3& _rhs);

		// Comparison operators
		/*******************************************************************************
		/*!
		*
		\brief
			Compare this Vec3 with the specified Vec3

		\param	_rhs
			read-only reference to the right hand side operand of the comparison

		\return
			true if both Vec3s are equal
			false if any of the values are different
		*/
		/*******************************************************************************/
		bool operator==(const Vec3& _rhs);
		/*******************************************************************************
		/*!
		*
		\brief
			Compare this Vec3 with the specified Vec3

		\param	_rhs
			read-only reference to the right hand side operand of the comparison

		\return
			true if this Vec3 is not equal to the specified Vec3
			false if both are the same
		*/
		/*******************************************************************************/
		bool operator!=(const Vec3& _rhs);

		// Conversion operators
		/*******************************************************************************
		/*!
		*
		\brief
			Returns this Vec3 as a glm::vec3

		\return
			this Vec3 as a glm::vec3
		*/
		/*******************************************************************************/
		operator glm::vec3()
		{
			return glmVec3;
		}
		/*******************************************************************************
		/*!
		*
		\brief
			Returns a Vec2 which is made by converting this Vec3 to a Vec2. Conversion is done by taking 
			the x and y value of this Vec3

		\return
			the resultant Vec2
		*/
		/*******************************************************************************/
		operator Vec2()
		{
			return { x,y };
		}
	};

	// Vec3 Binary Operators
	/*******************************************************************************
	/*!
	*
	\brief
		+ operator overload. Compute addition between two Vec3s.

	\param _lhs
		reference to the left hand operand

	\param _rhs
		reference to the right hand operand

	\return
		the resultant Vec3
	*/
	/*******************************************************************************/
	Vec3 operator+ (const Vec3& _lhs, const Vec3& _rhs);
	/*******************************************************************************
	/*!
	*
	\brief
		- operator overload. Compute subtraction between two Vec3s. 

	\param _lhs
		reference to the left hand operand

	\param _rhs
		reference to the right hand operand

	\return
		the resultant Vec3
	*/
	/*******************************************************************************/
	Vec3 operator- (const Vec3& _lhs, const Vec3& _rhs);
	/*******************************************************************************
	/*!
	*
	\brief
		* operator overload. Multiply each element in a Vec3 with a constant.

	\param _lhs
		reference to the Vec3 that the multiplication is to be applied on

	\param _rhs
		the constant that each element of _lhs is multiplied by

	\return
		the resultant Vec3
	*/
	/*******************************************************************************/
	Vec3 operator* (const Vec3& _lhs, float _rhs);
	/*******************************************************************************
	/*!
	*
	\brief
		* operator overload. Multiply each element in a Vec3 with a constant.

	\param _lhs
		the constant that each element of _rhs is multiplied by

	\param _rhs
		reference to the Vec3 that the multiplication is to be applied on

	\return
		the resultant Vec3
	*/
	/*******************************************************************************/
	Vec3 operator* (float _lhs, const Vec3& _rhs);
	/*******************************************************************************
	/*!
	*
	\brief
		/ operator overload. Divide each element in the Vec3 by a constant and return the resultant Vec3.

	\param _lhs
		reference to the Vec3 that the division is to be applied on

	\param _rhs
		the constant that each element of _lhs is to be divided by

	\return
		the resultant Vec3
	*/
	/*******************************************************************************/
	Vec3 operator/ (const Vec3& _lhs, float _rhs);

	// Vec3 Functions
	/*******************************************************************************
	/*!
	*
	\brief
		Compute the normalized Vec3 of the specified Vec3 and place the result in _result.

	\param _result
		reference to a Vec3 which will hold the resultant normalized Vec3

	\param _src
		reference to the Vec3 whose normalized Vec3 is to be computed.

	\return
		the computed length of _v
	*/
	/*******************************************************************************/
	void vec3_normalize(Vec3& _result, const Vec3& _src);
	/*******************************************************************************
	/*!
	*
	\brief
		Compute the length of a Vec3

	\param _v
		The vector whose squared length is to be computed

	\return
		the computed length of _v
	*/
	/*******************************************************************************/
	float vec3_length(const Vec3& _v);
	/*******************************************************************************
	/*!
	*
	\brief
		Compute the squared length of a Vec3

	\param _v
		The vector whose squared length is to be computed

	\return
		the computed squared length of _v
	*/
	/*******************************************************************************/
	float vec3_squarelength(const Vec3& _v);
	/*******************************************************************************
	/*!
	*
	\brief
		Compute the distance between two Vec3s.

	\param _v1
		The left hand operand

	\param _v2
		The right hand operand

	\return
		the computed distance between _v1, _v2
	*/
	/*******************************************************************************/
	float vec3_distance(const Vec3& _v1, const Vec3& _v2);
	/*******************************************************************************
	/*!
	*
	\brief
		Compute the squared distance between two Vec3s.

	\param _v1
		The left hand operand

	\param _v2
		The right hand operand

	\return
		the computed squared distance between _v1, _v2
	*/
	/*******************************************************************************/
	float vec3_squaredistance(const Vec3& _v1, const Vec3& _v2);
	/*******************************************************************************
	/*!
	*
	\brief
		Compute the dot product of two Vec3s.

	\param _v1
		The left hand operand of the dot product

	\param _v2
		The right hand operand of the dot product

	\return
		the computed result of the dot product between _v1, _v2
	*/
	/*******************************************************************************/
	float vec3_dotproduct(const Vec3& _v1, const Vec3& _v2);
	/*******************************************************************************
	/*!
	*
	\brief
		Compute the cross product between two Vec3s. 

	\param _v1
		The left hand operand of the cross product

	\param _v2
		The right hand operand of the cross product

	\return
		the computed Vec3 from the cross product
	*/
	/*******************************************************************************/
	Vec3 vec3_crossproduct(const Vec3& _v1, const Vec3& _v2);
	/*******************************************************************************
	/*!
	*
	\brief
		<< operator overload. Allows for printing of a Vec3 to std::cout.

	\param _os
		reference to the output stream

	\param _v
		reference to the Vec3 that is to be printed to output stream

	\return
		reference to the output stream
	*/
	/*******************************************************************************/
	std::ostream& operator<<(std::ostream& _os, const Vec3& _v);
	////////////////////////////
	
	// Matrix3x3 ////////////////////
	union Matrix3x3 {
	public:		
		
		float data[3][3];
		float m[9];
		glm::mat3x3 glmMat3x3;

		// Constructors
		Matrix3x3();
		Matrix3x3(const float(&_rhs)[9]);
		Matrix3x3(float _00, float _01, float _02,
					float _10, float _11, float _12,
					float _20, float _21, float _22);
		Matrix3x3(const glm::mat3x3& _rhs);
		Matrix3x3(const Matrix3x3& _src);

		// Copy assignment operator overloads
		/*******************************************************************************
		/*!
		*
		\brief
			= operator overload for Matrix3x3. Copies _rhs into this Matrix3x3.

		\param _rhs
			the Matrix3x3 that is to be copied into this Matrix3x3

		\return
			reference to this Matrix3x3
		*/
		/*******************************************************************************/
		Matrix3x3& operator= (const Matrix3x3& _rhs);
		/*******************************************************************************
		/*!
		*
		\brief
			Copies the data in the specified glm::mat33 into this Matrix3x3

		\param _rhs
			the glm::mat3x3 that is to be copied into this Matrix3x3

		\return
			reference to this Matrix3x3
		*/
		/*******************************************************************************/
		Matrix3x3& operator= (const glm::mat3x3& _rhs);
		
		// Compound assignment operator overloads
		/*******************************************************************************
		/*!
		*
		\brief
			*= operator overload for Matrix3x3. Compute matrix3x3 multiplication between this matrix and _rhs.
			Places the result in this matrix. 
			Note: this matrix is considered the left hand operand of the multiplication.

		\param _rhs
			the Matrix3x3 that is to be the right hand operand of the multiplication

		\return
			reference to this Matrix3x3
		*/
		/*******************************************************************************/
		Matrix3x3& operator*= (const Matrix3x3& _rhs);

		// Conversion operator overloads
		/*******************************************************************************
		/*!
		*
		\brief
			Creates a glm::mat3x3 and copies the data from this Matrix3x3 into the glm::mat3x3

		\return
			the resulting glm::mat3x3
		*/
		/*******************************************************************************/
		glm::mat3x3 to_glm() const;
		/*******************************************************************************
		/*!
		*
		\brief
			Returns this Matrix3x3 as a glm::mat3x3

		\return
			this Matrix3x3 as a glm::mat3x3
		*/
		/*******************************************************************************/
		operator glm::mat3x3();


	};

	// Matrix3x3 Binary operators
	/*******************************************************************************
	/*!
	*
	\brief
		Compute the resulting matrix from multiplication between two Matrix3x3 objects

	\param _lhs
		the left hand matrix of the multiplication

	\param _rhs
		the right hand matrix of the multiplication

	\return
		the resultant matrix of the multiplication
	*/
	/*******************************************************************************/
	Matrix3x3 operator* (const Matrix3x3& _lhs, const Matrix3x3& _rhs);
	/*******************************************************************************
	/*!
	*
	\brief
		Compute the resulting Vec3 from multiplying Matrix3x3 to a Vec3

	\param _lhs
		the Matrix3x3 in the operation

	\param _rhs
		the Vec3 in the operation

	\return
		the resultant Vec3 from the multiplication
	*/
	/*******************************************************************************/
	Vec3 operator*(const Matrix3x3& _lhs, const Vec3& _rhs);

	// Matrix3x3 functions
	/*******************************************************************************
	/*!
	*
	\brief
		This function sets the matrix _mtx to the identity matrix

	\param _mtx
		reference to the matrix that is to be set to the identity matrix

	\return
		void
	*/
	/*******************************************************************************/
	void matrix3x3_identity(Matrix3x3& _mtx);
	/*******************************************************************************
	/*!
	*
	\brief
		Create a translation matrix using _x and _y and put the result into _mtx.

	\param _mtx
		reference to the matrix that is to hold the resultant translation matrix

	\param _x
		the x value of the translation

	\param _y
		the y value of the translation

	\return
		void
	*/
	/*******************************************************************************/
	void matrix3x3_translate(Matrix3x3& _mtx, float _x, float _y);
	/*******************************************************************************
	/*!
	*
	\brief
		Create a scale matrix using _x and _y and put the result into _mtx.

	\param _mtx
		reference to the matrix that is to hold the resultant scale matrix

	\param _x
		the x value of the scale

	\param _y
		the y value of the scale

	\return
		void
	*/
	/*******************************************************************************/
	void matrix3x3_scale(Matrix3x3& _mtx, float _x, float _y);
	/*******************************************************************************
	/*!
	*
	\brief
		Create a rotation matrix using _angle and put the result into _mtx.
		Note: it is assumed that _angle is in radians.

	\param _mtx
		reference to the matrix that is to hold the resultant rotation matrix

	\param _angle
		the rotation angle in radians

	\return
		void
	*/
	/*******************************************************************************/
	void matrix3x3_rotrad(Matrix3x3& _mtx, float _angle);
	/*******************************************************************************
	/*!
	*
	\brief
		Create a rotation matrix using _angle and put the result into _mtx.
		Note: it is assumed that _angle is in degrees.

	\param _mtx
		reference to the matrix that is to hold the resultant rotation matrix

	\param _angle
		the rotation angle in degrees

	\return
		void
	*/
	/*******************************************************************************/
	void matrix3x3_rotdeg(Matrix3x3& _mtx, float _angle);
	/*******************************************************************************
	/*!
	*
	\brief
		Compute the transpose matrix of _rhs and place the resultant matrix into _mtx

	\param _mtx
		reference to the matrix that is to hold the resultant transpose matrix

	\param _rhs
		reference to the matrix of which its transpose matrix is to be computed

	\return
		void
	*/
	/*******************************************************************************/
	void matrix3x3_transpose(Matrix3x3& _mtx, const Matrix3x3& _rhs);
	/*******************************************************************************
	/*!
	*
	\brief
		Computes the inverse of _src and places the result into _dest.
		Note: if inversion does not work, _dest will be set to NULL.

	\param _mtx
		reference to the matrix that is to hold the resultant rotation matrix

	\param _angle
		the rotation angle in radians

	\return
		void
	*/
	/*******************************************************************************/
	void matrix3x3_inverse(Matrix3x3* _dest, float* _determinant, const Matrix3x3& _src);
	/*******************************************************************************
	/*!
	*
	\brief
		<< operator overload for Matrix3x3 struct. 

	\param _os
		reference to the output stream

	\param _mtx
		reference to the Matrix3x3 object to be printed to output stream

	\return
		reference to the output stream
	*/
	/*******************************************************************************/
	std::ostream& operator<<(std::ostream& _os, const Matrix3x3& _mtx);
	////////////////////////////


	// Matrix4x4 ////////////////////
	union Matrix4x4
	{
	public:

		float m[4][4];
		float data[16];
		glm::mat4x4 glmMat4x4;

		/*******************************************************************************
		/*!
		*
		\brief
			Default constructor for Matrix4x4. Will initialize all elements of the matrix to 0.0

		\return
			void
		*/
		/*******************************************************************************/
		Matrix4x4();
		/*******************************************************************************
		/*!
		*
		\brief
			Constructor for Matrix4x4 using read-only reference to an array of 16 floats. 
			Elements of array should be in row major order.

		\param _rhs
			read-only reference to the array of 16 floats which holds the values that the elements of 
			the Matrix4x4 are to be assigned

		\return
			void
		*/
		/*******************************************************************************/
		Matrix4x4(const float(&_rhs)[16]);
		/*******************************************************************************
		/*!
		*
		\brief
			Constructor for Matrix4x4 using 16 individual float values.
			Note: Arguments are used in row major order

		\return
			void
		*/
		/*******************************************************************************/
		Matrix4x4(float _00, float _01, float _02, float _03,
			float _10, float _11, float _12, float _13,
			float _20, float _21, float _22, float _23);
		/*******************************************************************************
		/*!
		*
		\brief
			Constructor for Matrix4x4 using a read-only reference to a glm::mat4x4.
			Note: this function does conversion from column major(glm::mat4x4) to row major(Matrix4x4)

		\param _rhs
			read-only reference to the glm::mat4x4 that will be used to initialize this Matrix4x4

		\return
			void
		*/
		/*******************************************************************************/
		Matrix4x4(const glm::mat4x4& _rhs);
		/*******************************************************************************
		/*!
		*
		\brief
			Copy constructor for Matrix4x4

		\param _rhs
			read-only reference to the Matrix4x4 which is to be copied into this Matrix4x4

		\return
			void
		*/
		/*******************************************************************************/
		Matrix4x4(const Matrix4x4& _rhs);
		/*******************************************************************************
		/*!
		*
		\brief
			Constructor for Matrix4x4 using 4x glm::vec4s.
			Note: each vec4 represents 1x row.
			E.g. the elements of _r0 will be assigned to the elements of row 0

		\param _r0
			read-only reference to the glm::vec4 which copied into row 0 of the Matrix4x4

		\param _r1
			read-only reference to the glm::vec4 which copied into row 1 of the Matrix4x4

		\param _r2
			read-only reference to the glm::vec4 which copied into row 2 of the Matrix4x4

		\param _r3
			read-only reference to the glm::vec4 which copied into row 3 of the Matrix4x4

		\return
			void
		*/
		/*******************************************************************************/
		Matrix4x4(const glm::vec4& _r0, const glm::vec4& _r1, const glm::vec4& _r2, const glm::vec4& _r3);

		/*******************************************************************************
		/*!
		*
		\brief
			= operator overload for Matrix4x4. Copies _rhs into this Matrix4x4.

		\param _rhs
			the Matrix4x4 that is to be copied into this Matrix4x4

		\return
			reference to this Matrix4x4
		*/
		/*******************************************************************************/
		Matrix4x4& operator=(const Matrix4x4& _rhs);
		/*******************************************************************************
		/*!
		*
		\brief
			Copies the data in the specified glm::mat4x4 into this Matrix4x4
			Note: this function does conversion from column major(glm::mat4x4) to row major(Matrix4x4) 

		\param _rhs
			the glm::mat4x4 that is to be copied into this Matrix4x4

		\return
			reference to this Matrix4x4
		*/
		/*******************************************************************************/
		Matrix4x4& operator=(const glm::mat4x4& _rhs);
		/*******************************************************************************
		/*!
		*
		\brief
			*= operator overload for Matrix4x4. Compute matrix4x4 multiplication between this matrix and _rhs.
			Places the result in this matrix.
			Note: this matrix is considered the left hand operand of the multiplication.

		\param _rhs
			the Matrix4x4 that is to be the right hand operand of the multiplication

		\return
			reference to this Matrix4x4
		*/
		/*******************************************************************************/
		Matrix4x4& operator*=(const Matrix4x4& _rhs);

		/*******************************************************************************
		/*!
		*
		\brief
			Creates a glm::mat4x4 and copies the data from this Matrix4x4 into the glm::mat4x4
			Note: this function does conversion from row major(Matrix4x4) to column major(glm::mat4x4)

		\return
			the resulting glm::mat4x4
		*/
		/*******************************************************************************/
		glm::mat4x4 to_glm() const;
		/*******************************************************************************
		/*!
		*
		\brief
			Returns this Matrix4x4 as a glm::Mat4x4

		\return
			this Matrix4x4 as a glm::Mat4x4
		*/
		/*******************************************************************************/
		operator glm::mat4x4();

	};

	/*******************************************************************************
	/*!
	*
	\brief
		Perform matrix addition between two matrix4x4s

	\param _lhs
		the left hand matrix of the addition

	\param _rhs
		the right hand matrix of the addition

	\return
		the resultant matrix of the addition
	*/
	/*******************************************************************************/
	Matrix4x4 operator+(const Matrix4x4& _lhs, const Matrix4x4& _rhs);
	/*******************************************************************************
	/*!
	*
	\brief
		Perform matrix subtraction between two matrix4x4s

	\param _lhs
		the left hand matrix of the subtraction

	\param _rhs
		the right hand matrix of the subtraction

	\return
		the resultant matrix of the subtraction
	*/
	/*******************************************************************************/
	Matrix4x4 operator-(const Matrix4x4& _lhs, const Matrix4x4& _rhs);
	/*******************************************************************************
	/*!
	*
	\brief
		Perform matrix multiplication between two matrix4x4s

	\param _lhs
		the left hand matrix of the multiplication

	\param _rhs
		the right hand matrix of the multiplication

	\return
		the resultant matrix of the multiplication
	*/
	/*******************************************************************************/
	Matrix4x4 operator*(const Matrix4x4& _lhs, const Matrix4x4& _rhs);
	/*******************************************************************************
	/*!
	*
	\brief
		Perform scalar multiplication on a Matrix4x4

	\param _scalar
		the scalar value to multiply with

	\param _mtx
		the matrix involved in the scalar multiplication

	\return
		the resultant matrix of the scalar multiplication
	*/
	/*******************************************************************************/
	Matrix4x4 operator*(float _scalar, const Matrix4x4& _mtx);

	/*******************************************************************************
	/*!
	*
	\brief
		Perform matrix multiplication between Matrix4x4 and Vec4

	\param _mat
		the Matrix4x4 which is the Left Hand Operand

	\param _vec4
		the Vec4 which will be the Right Hand Operand

	\return
		the resultant Vec4
	*/
	/*******************************************************************************/
	Vec4 operator*(const Matrix4x4& _mat, const Vec4& _vec4);

	/*******************************************************************************
	/*!
	*
	\brief
		Turns specified Matrix4x4 into its identity matrix

	\param _rhs
		reference to the Matrix4x4 to perform this op on

	\return
		void
	*/
	/*******************************************************************************/
	void matrix4x4_identity(Matrix4x4& _rhs);
	/*******************************************************************************
	/*!
	*
	\brief
		Create a translation matrix using specified arguments and place result in specified Matrix4x4

	\param _mtx
		reference to the Matrix4x4 to perform this op on

	\param _x
		the x value of the translation

	\param _y
		the y value of the translation

	\param _z
		the z value of the translation

	\return
		void
	*/
	/*******************************************************************************/
	void matrix4x4_translation(Matrix4x4& _mtx, float _x, float _y, float _z);
	/*******************************************************************************
	/*!
	*
	\brief
		Create a scale matrix using specified arguments and place result in specified Matrix4x4

	\param _mtx
		reference to the Matrix4x4 to perform this op on

	\param _x
		the x value of the scaling

	\param _y
		the y value of the scaling

	\param _z
		the z value of the scaling

	\return
		void
	*/
	/*******************************************************************************/
	void matrix4x4_scale(Matrix4x4& _mtx, float _x, float _y, float _z);
	/*******************************************************************************
	/*!
	*
	\brief
		Create a 2D rotation matrix using specified angle and place result in specified Matrix4x4

	\param _mtx
		reference to the Matrix4x4 to perform this op on

	\param _deg
		the angle of rotation that this matrix will apply
		Note: this value MUST be in radians

	\return
		void
	*/
	/*******************************************************************************/
	void matrix4x4_rot_2D(Matrix4x4& _mtx, float _deg);
	/*******************************************************************************
	/*!
	*
	\brief
		Create a 3D rotation matrix around X-axis using specified angle and place result in specified Matrix4x4

	\param _mtx
		reference to the Matrix4x4 to perform this op on

	\param _deg
		the angle of rotation that this matrix will apply
		Note: this value MUST be in radians

	\return
		void
	*/
	/*******************************************************************************/
	void matrix4x4_rot_x(Matrix4x4& _mtx, float _deg);
	/*******************************************************************************
	/*!
	*
	\brief
		Create a 3D rotation matrix around Y-axis using specified angle and place result in specified Matrix4x4

	\param _mtx
		reference to the Matrix4x4 to perform this op on

	\param _deg
		the angle of rotation that this matrix will apply
		Note: this value MUST be in radians

	\return
		void
	*/
	/*******************************************************************************/
	void matrix4x4_rot_y(Matrix4x4& _mtx, float _deg);
	/*******************************************************************************
	/*!
	*
	\brief
		Create a 3D rotation matrix around Z-axis using specified angle and place result in specified Matrix4x4

	\param _mtx
		reference to the Matrix4x4 to perform this op on

	\param _deg
		the angle of rotation that this matrix will apply
		Note: this value MUST be in radians

	\return
		void
	*/
	/*******************************************************************************/
	void matrix4x4_rot_z(Matrix4x4& _mtx, float _deg);

	/*******************************************************************************
	/*!
	*
	\brief
		<< operator overload. Allow for printing of Matrix4x4 to cout in a readable format

	\param _os
		reference to the ostream

	\param _mtx
		read-only reference to the Matrix4x4 that is to be printed

	\return
		reference to the ostream
	*/
	/*******************************************************************************/
	std::ostream& operator<<(std::ostream& _os, const Matrix4x4& _mtx);
	////////////////////////////

	/*******************************************************************************
	/*!
	*
	\brief
		Convert _angle to radians.
		Note: it is assumed that _angle is in degrees.

	\param _angle
		the angle (in degrees) that is to be converted into radians

	\return
		the result of the conversion of _angle to radians
	*/
	/*******************************************************************************/
	float deg_to_rad(float _angle);
	/*******************************************************************************
	/*!
	*
	\brief
		Convert _angle to degrees
		Note: it is assumed that _angle is in radians

	\param _angle
		the angle (in radians) that is to be converted into degrees

	\return
		the result of the conversion of _angle to degrees
	*/
	/*******************************************************************************/
	float rad_to_deg(float _angle);
	
	/*******************************************************************************
	/*!
	*
	\brief
		Deserialize a glm::vec4 from a rapidjson value
		Note: this function was written specifically for color vec4

	\param _value
		reference to the rapidjson value

	\param _v
		reference to the glm::vec4 which will hold the deserialized data

	\return
		if there are errors with the format of the rapidjson value specified, return false
		if deserialization was successful, return true
	*/
	/*******************************************************************************/
	//bool deserialize_color(rapidjson::Value& _value, glm::vec4& _v);
}
#endif !MATH_LIBRARY_H
 