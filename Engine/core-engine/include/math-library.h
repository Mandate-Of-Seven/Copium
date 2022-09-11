/*!***************************************************************************************
****
\file			math-library.h
\project
\author			Matthew Lau
\co-authors

\par			Course: GAM200
\par			Section:
\date			02/09/2022

\brief
	Contains declarations for math library.
	The math library contains:
	1. 2D vector
	2. 3D vector
	3. 3x3 matrix
	Each has a class dedicated to it which contains operator overloads.
	Declarations of functions which perform common mathematical operations for the respective classes
	are also contained here.

All content � 2022 DigiPen Institute of Technology Singapore. All rights reserved.
******************************************************************************************/
#pragma once
#ifndef MATHLIBRARY_H
#define MATHLIBRARY_H
#include <math.h>
#include <iostream>
#include <rapidjson/prettywriter.h>

namespace Copium 
{
	namespace Math 
	{
		//Ignore this, for debug purposes
		class myint {
		public:
			int i;
			bool serialize(rapidjson::PrettyWriter<rapidjson::StringBuffer>& _writer) const
			{
				_writer.Int(i);
				return false;
			}
		};

		struct Vec2 {
		public:
			// Constructors
			Vec2();
			Vec2(double _x, double _y);

			// Assignment operators
			/*******************************************************************************
			/*!
			*
			\brief
				= operator overload. Copy the specified Vec2 into this Vec2

			\param _rhs
				reference to the Vec2 which will be copied into this Vec2

			\return
				reference to this Vec2
			*/
			/*******************************************************************************/
			Vec2& operator= (const Vec2& _rhs);
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
			Vec2& operator*= (double _rhs);
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
			Vec2& operator/= (double _rhs);

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
	
			// Accessors
			double X() const;
			double Y() const;

			/*******************************************************************************
			/*!
			*
			\brief
				Serialize this Vec2 to a rapidJson PrettyWriter String Buffer

			\param _writer
				reference to the rapidJson buffer to serialize to

			\return
				the resultant Vec2
			*/
			/*******************************************************************************/
			bool serialize(rapidjson::PrettyWriter<rapidjson::StringBuffer>& _writer) const;
			bool deserialize(std::ifstream& _ifs) const
			{
				return false;
			}

			double x, y;

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
		Vec2 operator* (const Vec2& _lhs, double _rhs);
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
		Vec2 operator* (double _lhs, const Vec2& _rhs);
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
		Vec2 operator/ (const Vec2& _lhs, double _rhs);

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
		double vec2_length(const Vec2& _v);
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
		double vec2_squarelength(const Vec2& _v);
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
		double vec2_distance(const Vec2& _v1, const Vec2& _v2);
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
		double vec2_squaredistance(const Vec2& _v1, const Vec2& _v2);
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
		double vec2_dotproduct(const Vec2& _v1, const Vec2& _v2);
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
		double vec2_crossproductmag(const Vec2& _v1, const Vec2& _v2);

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



		struct Vec3 {
		public:
			//Ctors
			Vec3();
			Vec3(double _x, double _y, double _z);

			/*******************************************************************************
			/*!
			*
			\brief
				= operator overload. Copy the values in specified Vec3 into this Vec3

			\param _rhs
				reference to the Vec3 whose values is to be copied

			\return
				reference to this Vec3
			*/
			/*******************************************************************************/
			Vec3& operator= (const Vec3& _rhs);
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
			Vec3& operator*= (double _rhs);
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
			Vec3& operator/= (double _rhs);

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

			double X() const;
			double Y() const;
			double Z() const;

			/*******************************************************************************
			/*!
			*
			\brief
				Serialize this Vec3 to a rapidJson PrettyWriter String Buffer

			\param _writer
				reference to the rapidJson buffer to serialize to

			\return
				void
			*/
			/*******************************************************************************/
			void serialize(rapidjson::PrettyWriter<rapidjson::StringBuffer>& _writer) const;

			double x, y, z;
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
		Vec3 operator* (const Vec3& _lhs, double _rhs);
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
		Vec3 operator* (double _lhs, const Vec3& _rhs);
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
		Vec3 operator/ (const Vec3& _lhs, double _rhs);

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
		double vec3_length(const Vec3& _v);
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
		double vec3_squarelength(const Vec3& _v);
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
		double vec3_distance(const Vec3& _v1, const Vec3& _v2);
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
		double vec3_squaredistance(const Vec3& _v1, const Vec3& _v2);
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
		double vec3_dotproduct(const Vec3& _v1, const Vec3& _v2);
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

		struct Matrix3x3 {
		public:
			Matrix3x3();
			Matrix3x3(const double(&_rhs)[9]);
			Matrix3x3(double _00, double _01, double _02,
					  double _10, double _11, double _12,
					  double _20, double _21, double _22);
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
			double m[3][3];
		};

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
		void matrix3x3_translate(Matrix3x3& _mtx, double _x, double _y);

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
		void matrix3x3_scale(Matrix3x3& _mtx, double _x, double _y);

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
		void matrix3x3_rotrad(Matrix3x3& _mtx, double _angle);

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
		void matrix3x3_rotdeg(Matrix3x3& _mtx, double _angle);

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
		void matrix3x3_inverse(Matrix3x3* _dest, double* _determinant, const Matrix3x3& _src);
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
		double deg_to_rad(double _angle);
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
		double rad_to_deg(double _angle);
	}
}
#endif
 