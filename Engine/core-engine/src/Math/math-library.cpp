/*!***************************************************************************************
\file			math-library.cpp
\project
\author			Matthew Lau

\par			Course: GAM200
\par			Section:
\date			02/09/2022

\brief
	Contains definitions for math library.
	The math library contains:
	1. 2D vector
	2. 3D vector
	3. 3x3 matrix
	Each has a class dedicated to it which contains operator overloads.
	Declarations of functions which perform common mathematical operations for the respective classes
	are also contained here.

All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
******************************************************************************************/
#include "pch.h"
#include "Math/math-library.h"
#include <rttr/registration>



namespace Copium::Math
{
	// USEFUL MATH MACROS
	#define EPSILON		0.0001
	#define PI			3.14159265358

	//Setting up for RTTR implementation----------
	RTTR_REGISTRATION
	{
		using namespace rttr;
	registration::class_<myint>("myint")
		.property("I", &myint::i);


	registration::class_<Vec2>("Vec2")
		.property("x", &Vec2::x)
		.property("y", &Vec2::y);

	registration::class_<Vec3>("Vec3")
		.property("x", &Vec3::x)
		.property("y", &Vec3::y)
		.property("z", &Vec3::z);




	}

	myint::myint() : i{0} {}
	myint::myint(int32_t _i) : i{_i} {}


	// Vec2-----------------------------------------
	// Vec2 Constructors
	Vec2::Vec2(): x{0.0}, y{0.0} {}
	Vec2::Vec2(double _x, double _y) : x{_x}, y{_y} {}
	Vec2::Vec2(glm::dvec2& _v) : x{_v.x}, y{_v.y} {}

	// Vec2 Assignment Operators + Unary Operator
	Vec2& Vec2::operator= (const Vec2& _rhs)
	{
		x = _rhs.x;
		y = _rhs.y;
		return *this;
	}
	Vec2& Vec2::operator= (const glm::dvec2& _rhs)
	{
		x = _rhs.x;
		y = _rhs.y;
		return *this;
	}
	Vec2& Vec2::operator+= (const Vec2& _rhs)
	{
		x += _rhs.x;
		y += _rhs.y;
		return *this;
	}
	Vec2& Vec2::operator-= (const Vec2& _rhs)
	{
		x -= _rhs.x;
		y -= _rhs.y;
		return *this;
	}
	Vec2& Vec2::operator*= (double _rhs)
	{
		x *= _rhs;
		y *= _rhs;
		return *this;
	}
	Vec2& Vec2::operator/= (double _rhs)
	{
		x /= _rhs;
		y /= _rhs;
		return *this;
	}
	Vec2 Vec2::operator- () const
	{
		return Vec2(-x, -y);
	}

	// Accessor Functions
	double Vec2::X() const { return x; }
	double Vec2::Y() const { return y; }
	glm::dvec2 Vec2::to_glm() const
	{
		return glm::vec2(x, y);
	}

	// Vec2 Binary Operators
	Vec2 operator+ (const Vec2& _lhs, const Vec2& _rhs)
	{
		return Vec2(_lhs.X() + _rhs.X(), _lhs.Y() + _rhs.Y());
	}
	Vec2 operator- (const Vec2& _lhs, const Vec2& _rhs)
	{
		return Vec2(_lhs.X() - _rhs.X(), _lhs.Y() - _rhs.Y());
	}
	Vec2 operator* (const Vec2& _lhs, double _rhs)
	{
		return Vec2(_lhs.X() * _rhs, _lhs.Y() * _rhs);
	}
	Vec2 operator* (double _lhs, const Vec2& _rhs)
	{
		return Vec2(_lhs * _rhs.X(), _lhs * _rhs.Y());
	}
	Vec2 operator/ (const Vec2& _lhs, double _rhs)
	{
		return Vec2(_lhs.X() / _rhs, _lhs.Y() / _rhs);
	}

	// Vec2 Functions
	void vec2_normalize(Vec2& _result, const Vec2& _src)
	{
		Vec2 v(_src.X() / vec2_length(_src), _src.Y() / vec2_length(_src));
		_result = v;

	}
	double vec2_length(const Vec2& _v)
	{
		return sqrt(vec2_squarelength(_v));
	}
	double vec2_squarelength(const Vec2& _v)
	{
		return (_v.X()*_v.X() + _v.Y()*_v.Y());
	}
	double vec2_distance(const Vec2& _v1, const Vec2& _v2)
	{
		Vec2 v(_v1.X() - _v2.X(), _v1.Y() - _v2.Y());
		return vec2_length(v);
	}
	double vec2_squaredistance(const Vec2& _v1, const Vec2& _v2)
	{
		Vec2 v(_v1.X() - _v2.X(), _v1.Y() - _v2.Y());
		return vec2_squarelength(v);
	}
	double vec2_dotproduct(const Vec2& _v1, const Vec2& _v2)
	{
		return (_v1.X() * _v2.X() + _v1.Y() * _v2.Y());
	}
	double vec2_crossproductmag(const Vec2& _v1, const Vec2& _v2)
	{
		return (_v1.X() * _v2.Y() - _v1.Y() * _v2.X());
	}
	std::ostream& operator<<(std::ostream& _os, const Vec2& _v) 
	{
		_os << "x:" << _v.X() << " " << "y:" << _v.Y() << std::endl;
		return _os;
	}
	bool Vec2::deserialize(rapidjson::Value& _value	)
	{
		if (!_value.HasMember("X") || !_value.HasMember("Y"))
			return false;

		x = _value["X"].GetDouble();
		y = _value["Y"].GetDouble();
			
		return false;

	}


	// Vec3-----------------------------------------
	// Vec3 Constructors
	Vec3::Vec3(): x{0.0}, y{0.0}, z{0.0} {}
	Vec3::Vec3(double _x, double _y, double _z): x{_x}, y{_y}, z{_z} {}
	Vec3::Vec3(glm::dvec3& _v): x{_v.x}, y{_v.y}, z{_v.z} {}

	// Vec3 Assignment Operators + Unary Operator
	Vec3& Vec3::operator= (const Vec3& _rhs)
	{
		x = _rhs.x;
		y = _rhs.y;
		z = _rhs.z;
		return *this;
	}
	Vec3& Vec3::operator= (const glm::dvec3& _rhs)
	{
		x = _rhs.x;
		y = _rhs.y;
		z = _rhs.z;
		return *this;
	}
	Vec3& Vec3::operator+= (const Vec3& _rhs)
	{
		x += _rhs.x;
		y += _rhs.y;
		z += _rhs.z;
		return *this;
	}
	Vec3& Vec3::operator-= (const Vec3& _rhs)
	{
		x -= _rhs.x;
		y -= _rhs.y;
		z -= _rhs.z;
		return *this;
	}
	Vec3& Vec3::operator*= (double _rhs)
	{
		x *= _rhs;
		y *= _rhs;
		z *= _rhs;
		return *this;
	}
	Vec3& Vec3::operator/= (double _rhs)
	{
		x /= _rhs;
		y /= _rhs;
		z /= _rhs;
		return *this;
	}
	Vec3 Vec3::operator- () const
	{
		return Vec3(-x, -y, -z);
	}

	// Vec3 Accessor functions
	double Vec3::X() const { return x; }
	double Vec3::Y() const { return y; }
	double Vec3::Z() const { return z; }

	glm::dvec3 Vec3::to_glm() const
	{
		return glm::vec3(x, y, z);
	}


	// Vec3 Binary Operators
	Vec3 operator+ (const Vec3& _lhs, const Vec3& _rhs)
	{
		return Vec3(_lhs.X() + _rhs.X(), _lhs.Y() + _rhs.Y(), _lhs.Z() + _rhs.Z());
	}
	Vec3 operator- (const Vec3& _lhs, const Vec3& _rhs)
	{
		return Vec3(_lhs.X() - _rhs.X(), _lhs.Y() - _rhs.Y(), _lhs.Z() - _rhs.Z());
	}
	Vec3 operator* (const Vec3& _lhs, double _rhs)
	{
		return Vec3(_lhs.X() * _rhs, _lhs.Y() * _rhs, _lhs.Z() * _rhs);
	}
	Vec3 operator* (double _lhs, const Vec3& _rhs)
	{
		return Vec3(_rhs.X() * _lhs, _rhs.Y() * _lhs, _rhs.Z() * _lhs);
	}
	Vec3 operator/ (const Vec3& _lhs, double _rhs)
	{
		return Vec3(_lhs.X() / _rhs, _lhs.Y() / _rhs, _lhs.Z() / _rhs);
	}

	// Vec3 Functions
	void vec3_normalize(Vec3& _result, const Vec3& _src)
	{
		Vec3 v(_src.X() / vec3_length(_src), _src.Y() / vec3_length(_src), _src.Z() / vec3_length(_src));
		_result = v;
	}
	double vec3_length(const Vec3& _v)
	{
		double d;
		d = sqrt(vec3_squarelength(_v));
		return d;
	}
	double vec3_squarelength(const Vec3& _v)
	{
		double d;
		d = _v.X() * _v.X() + _v.Y() * _v.Y() + _v.Z() * _v.Z();
		return d;
	}
	double vec3_distance(const Vec3& _v1, const Vec3& _v2)
	{
		Vec3 v(_v1.X() - _v2.X(), _v1.Y() - _v2.Y(), _v1.Z() - _v2.Z());
		return vec3_length(v);
	}
	double vec3_squaredistance(const Vec3& _v1, const Vec3& _v2)
	{
		Vec3 v(_v1.X() - _v2.X(), _v1.Y() - _v2.Y(), _v1.Z() - _v2.Z());
		return vec3_squarelength(v);
	}
	double vec3_dotproduct(const Vec3& _v1, const Vec3& _v2)
	{
		double d = _v1.X() * _v2.X() + _v1.Y() * _v2.Y() + _v1.Z() * _v2.Z();
		return d;
	}
	Vec3 vec3_crossproduct(const Vec3& _v1, const Vec3& _v2)
	{
		double vx, vy, vz;
		vx = _v1.Y() * _v2.Z() - _v1.Z() * _v2.Y();
		vy = _v1.X() * _v2.Z() - _v1.Z() * _v2.X();
		vz = _v1.X() * _v2.Y() - _v1.Y() * _v2.X();
		return Vec3(vx, vy, vz);
	}
	std::ostream& operator<<(std::ostream& _os, const Vec3& _v) 
	{
		_os << "x:" << _v.X() << " y:" << _v.Y() << " z:" << _v.Z() << std::endl;
		return _os;
	}
	bool Vec3::deserialize(rapidjson::Value& _value)
	{
		if (!_value.HasMember("X") || !_value.HasMember("Y") || !_value.HasMember("Z"))
			return false;

		x = _value["X"].GetDouble();
		y = _value["Y"].GetDouble();
		z = _value["Z"].GetDouble();
		return true;

	}



	// Matrix3x3-----------------------------------------
	Matrix3x3::Matrix3x3()
	{
		for (int i{ 0 }; i < 3; ++i)
		{
			for (int j{ 0 }; j < 3; ++j)
			{
				m[i][j] = 0.0;
			}
		}
	}
	Matrix3x3::Matrix3x3(const double(&_rhs)[9]) {
		for (int i{ 0 }, k{ 0 }; i < 3; ++i)
		{
			for (int j{ 0 }; j < 3; ++j)
			{
				m[i][j] = _rhs[k];
				++k;
			}
		}
	}
	Matrix3x3::Matrix3x3(double _00, double _01, double _02,
							double _10, double _11, double _12,
							double _20, double _21, double _22)
	{
		m[0][0] = _00;
		m[0][1] = _01;
		m[0][2] = _02;

		m[1][0] = _10;
		m[1][1] = _11;
		m[1][2] = _12;

		m[2][0] = _20;
		m[2][1] = _21;
		m[2][2] = _22;

	}
	Matrix3x3::Matrix3x3(const glm::mat3x3& _rhs)
	{
		for (int i{ 0 }; i < 3; ++i)
		{
			for (int j{ 0 }; j < 3; ++j)
			{
				m[i][j] = _rhs[i][j];
			}
		}
	}

	Matrix3x3& Matrix3x3::operator= (const Matrix3x3& _rhs)
	{
		for (int i{ 0 }; i < 3; ++i)
		{
			for (int j{ 0 }; j < 3; ++j)
			{
				m[i][j] = _rhs.m[i][j];
			}
		}

		return *this;
	}
	Matrix3x3& Matrix3x3::operator= (const glm::mat3x3& _rhs)
	{
		for (int i{ 0 }; i < 3; ++i)
		{
			for (int j{ 0 }; j < 3; ++j)
			{
				m[i][j] = _rhs[i][j];
			}
		}

		return *this;
	}

	Matrix3x3& Matrix3x3::operator*= (const Matrix3x3& _rhs)
	{
		Matrix3x3 result;
		result = *this;

		for (int i = 0; i < 3; ++i) {
			for (int j = 0; j < 3; ++j) {
				result.m[i][j] = result.m[i][0] * _rhs.m[0][j]
					+ result.m[i][1] * _rhs.m[1][j]
					+ result.m[i][2] * _rhs.m[2][j];
			}
		}

		*this = result;
		return *this;
	}
	Matrix3x3 operator* (const Matrix3x3& _lhs, const Matrix3x3& _rhs) {
		Matrix3x3 result;
		result = _lhs;
		for (int i = 0; i < 3; ++i) {
			for (int j = 0; j < 3; ++j) {

				result.m[i][j] = _lhs.m[i][0] * _rhs.m[0][j]
					+ _lhs.m[i][1] * _rhs.m[1][j]
					+ _lhs.m[i][2] * _rhs.m[2][j];
			}
		}
		return result;
	}

	glm::mat3x3 Matrix3x3::to_glm() const
	{
		glm::mat3x3 tmp;
		for (int i{ 0 }; i < 3; ++i)
		{
			for (int j{ 0 }; j < 3; ++j)
			{
				tmp[i][j] = (float) m[i][j];
			}
		}

		return tmp;
	}

	void matrix3x3_identity(Matrix3x3& _mtx) 
	{
		for (int i = 0; i < 3; ++i) {
			for (int j = 0; j < 3; ++j) {

				if (i == j) {
					_mtx.m[i][j] = 1.0f;
				}
				else {
					_mtx.m[i][j] = 0.0f;
				}


			}
		}
	}
	void matrix3x3_translate(Matrix3x3& _mtx, double _x, double _y) 
	{
		matrix3x3_identity(_mtx);
		_mtx.m[0][2] = _x;
		_mtx.m[1][2] = _y;

	}
	void matrix3x3_scale(Matrix3x3& _mtx, double _x, double _y)
	{
		matrix3x3_identity(_mtx);
		_mtx.m[0][0] = _x;
		_mtx.m[1][1] = _y;
	}
	void matrix3x3_rotrad(Matrix3x3& _mtx, double _angle) 
	{
		matrix3x3_identity(_mtx);

		_mtx.m[0][0] = cosf((float) _angle);
		_mtx.m[0][1] = -sinf((float) _angle);
		_mtx.m[1][0] = sinf((float) _angle);
		_mtx.m[1][1] = cosf((float) _angle);
	}
	void matrix3x3_rotdeg(Matrix3x3& _mtx, double _angle) 
	{
		matrix3x3_identity(_mtx);

		//Convert to radian
		double result = _angle * (PI / 180.0);

		_mtx.m[0][0] = cosf((float) result);
		_mtx.m[0][1] = -sinf((float) result);
		_mtx.m[1][0] = sinf((float) result);
		_mtx.m[1][1] = cosf((float) result);

	}
	void matrix3x3_transpose(Matrix3x3& _mtx, const Matrix3x3& _rhs) 
	{
		_mtx = _rhs;
		double tmp;

		//Swap values across the diagonal
		tmp = _mtx.m[0][1];
		_mtx.m[0][1] = _mtx.m[1][0];
		_mtx.m[1][0] = tmp;

		tmp = _mtx.m[0][2];
		_mtx.m[0][2] = _mtx.m[2][0];
		_mtx.m[2][0] = tmp;

		tmp = _mtx.m[1][2];
		_mtx.m[1][2] = _mtx.m[2][1];
		_mtx.m[2][1] = tmp;

	}
	void matrix3x3_inverse(Matrix3x3* _dest, double* _determinant, const Matrix3x3& _src) 
	{
		*_determinant = _src.m[0][0] * (_src.m[1][1] * _src.m[2][2] - _src.m[1][2] * _src.m[2][1])
			- _src.m[0][1] * (_src.m[1][0] * _src.m[2][2] - _src.m[1][2] * _src.m[2][0])
			+ _src.m[0][2] * (_src.m[1][0] * _src.m[2][1] - _src.m[2][0] * _src.m[1][2]);
		if (*_determinant <= EPSILON) {
			_dest = nullptr;
			return;
		}


		Matrix3x3 tmp;
		tmp = _src;

		//Compute Matrix of Minors
		tmp.m[0][0] = _src.m[1][1] * _src.m[2][2] - _src.m[1][2] * _src.m[2][1];
		tmp.m[0][1] = _src.m[1][0] * _src.m[2][2] - _src.m[1][2] * _src.m[2][0];
		tmp.m[0][2] = _src.m[1][0] * _src.m[2][1] - _src.m[1][1] * _src.m[2][0];

		tmp.m[1][0] = _src.m[0][1] * _src.m[2][2] - _src.m[0][2] * _src.m[2][1];
		tmp.m[1][1] = _src.m[0][0] * _src.m[2][2] - _src.m[0][2] * _src.m[2][0];
		tmp.m[1][2] = _src.m[0][0] * _src.m[2][1] - _src.m[0][1] * _src.m[2][0];

		tmp.m[2][0] = _src.m[0][1] * _src.m[1][2] - _src.m[0][2] * _src.m[1][1];
		tmp.m[2][1] = _src.m[0][0] * _src.m[1][2] - _src.m[0][2] * _src.m[1][0];
		tmp.m[2][2] = _src.m[0][0] * _src.m[1][1] - _src.m[0][1] * _src.m[1][0];

		//Compute Matrix of Cofactors
		for (int i = 0; i < 3; ++i) {
			if (i % 2 == 0) {
				_dest->m[i][0] = -_dest->m[i][0];
				_dest->m[i][2] = -_dest->m[i][2];

			}
			else {
				_dest->m[i][1] = -_dest->m[i][1];
			}

		}

		//Transpose to obtain Adjugate
		matrix3x3_transpose(*_dest, *_dest);

		//Multiply Adjugate by 1/determinant
		for (int i{ 0 }; i < 3; ++i) {
			for (int j{ 0 }; j < 3; ++j) {
				tmp.m[i][j] *= (1.0 / (*_determinant));
			}
		}

		//Store result
		*_dest = tmp;

	}
	std::ostream& operator<<(std::ostream& _os, const Matrix3x3& _mtx)
	{
		_os << '{';
		for (int i{ 0 }; i < 3; ++i) {
			for (int j{ 0 }; j < 3; ++j) {
				_os << _mtx.m[i][j];
				if (j == 2)
					_os << '\n';
				else
					_os << ',';
			}

		}
		_os << "}\n";
		return _os;
	}

	double deg_to_rad(double _angle)
	{
		double result = _angle * (PI / 180.0);
		return result;
	}
	double rad_to_deg(double _angle)
	{
		double result = _angle * (180.0 / PI);
		return result;
	}
	
}


