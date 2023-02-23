#ifndef VECTOR4_H
#define VECTOR4_H

#include <math.h>
#include <iostream>
#include <glm/vec4.hpp>
//#include <rapidjson/document.h>

#pragma warning(disable : 4201)

namespace Copium::Math {
	union Vec4 {
	public:
		struct
		{
			float x, y, z, a;
		};
		float data[4];
		glm::vec4 glmVec4;

		// Constructors
		Vec4() = default;
		Vec4(float _x, float _y, float _z, float _a);
		Vec4(const glm::vec4& _src);
		Vec4(const Vec4& _src);

		// Copy assignment operator overloads
		Vec4& operator=(const Vec4& _src);
		Vec4& operator=(const glm::vec4& _src);

		// Unary operator overload
		Vec4 operator-() const;

		// Compound assignment operator overloads
		Vec4& operator+= (const Vec4& _src);
		Vec4& operator-= (const Vec4& _src);
		Vec4& operator*= (const Vec4& _src);
		Vec4& operator/= (const Vec4& _src);

		// Comparison operator overloads
		bool operator==(const Vec4& _rhs);
		bool operator!=(const Vec4& _rhs);

		// Conversions
		operator glm::vec4();

		// De/serialization
		//bool Deserialize(rapidjson::Value& _value);
		//bool Serialize(rapidjson::Value& _value, rapidjson::Document& _doc);

	};

	// Vec4 Binary Operators
	/*******************************************************************************
	/*!
	*
	\brief
		+ operator overload. Performs vector addition between two Vec4s

	\param _lhs
		the left hand operand

	\param _rhs
		the right hand operand

	\return
		the resultant Vec4
	*/
	/*******************************************************************************/
	Vec4 operator+ (const Vec4& _lhs, const Vec4& _rhs);
	/*******************************************************************************
	/*!
	*
	\brief
		- operator overload. Performs vector subtraction between two Vec4s

	\param _lhs
		the left hand operand

	\param _rhs
		the right hand operand

	\return
		the resultant Vec4
	*/
	/*******************************************************************************/
	Vec4 operator- (const Vec4& _lhs, const Vec4& _rhs);
	/*******************************************************************************
	/*!
	*
	\brief
		* operator overload. Performs scalar multiplication on each element of a Vec4

	\param _lhs
		reference to the Vec4

	\param _rhs
		the constant to be used in the multiplication

	\return
		the resultant Vec4
	*/
	/*******************************************************************************/
	Vec4 operator* (const Vec4& _lhs, float _rhs);
	/*******************************************************************************
	/*!
	*
	\brief
		* operator overload. Performs scalar multiplication on each element of a Vec4

	\param _lhs
		the constant to be used in the multiplication

	\param _rhs
		reference to the Vec4

	\return
		the resultant Vec4
	*/
	/*******************************************************************************/
	Vec4 operator* (float _lhs, const Vec4& _rhs);
	/*******************************************************************************
	/*!
	*
	\brief
		/ operator overload. Performs scalar division on each element of a Vec4

	\param _lhs
		reference to the Vec4

	\param _rhs
		the constant to be used in the division

	\return
		the resultant Vec4
	*/
	/*******************************************************************************/
	Vec4 operator/ (const Vec4& _lhs, float _rhs);

	// Vec4 vector operations
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
	void Vec4Normalize(Vec4& _result, const Vec4& _src);
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
	float Vec4Length(const Vec4& _v);
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
	float Vec4SquareLength(const Vec4& _v);
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
	float Vec4Distance(const Vec4& _v1, const Vec4& _v2);
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
	float Vec4SquaredDistance(const Vec4& _v1, const Vec4& _v2);
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
	float Vec4DotProduct(const Vec4& _v1, const Vec4& _v2);

	std::ostream& operator<<(std::ostream& _os, const Vec4& _v);

}
#endif !VECTOR4_H
