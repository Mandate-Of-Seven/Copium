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
	#define EPSILON		0.0001f
	#define PI			3.14159265358f

	//Setting up for RTTR implementation----------
	//RTTR_REGISTRATION
	//{
	//	using namespace rttr;
	//registration::class_<myint>("myint")
	//	.property("I", &myint::i);


	//registration::class_<Vec2>("Vec2")
	//	.property("x", &Vec2::x)
	//	.property("y", &Vec2::y);

	//registration::class_<Vec3>("Vec3")
	//	.property("x", &Vec3::x)
	//	.property("y", &Vec3::y)
	//	.property("z", &Vec3::z);
	//}

	myint::myint() : i{0} {}
	myint::myint(int32_t _i) : i{_i} {}


	// Vec2-----------------------------------------
	// Vec2 Constructors
	Vec2::Vec2() : data{ 0.0f } {}
	Vec2::Vec2(float _x, float _y) : x{_x}, y{_y} {}
	Vec2::Vec2(glm::vec2& _v) : x{_v.x}, y{_v.y} {}

	// Vec2 Assignment Operators + Unary Operator
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
	Vec2& Vec2::operator*= (float _rhs)
	{
		x *= _rhs;
		y *= _rhs;
		return *this;
	}
	Vec2& Vec2::operator/= (float _rhs)
	{
		x /= _rhs;
		y /= _rhs;
		return *this;
	}
	Vec2 Vec2::operator- () const
	{
		return Vec2(-x, -y);
	}

	glm::vec2 Vec2::to_glm() const
	{
		return glm::vec2(x, y);
	}

	// Vec2 Binary Operators
	Vec2 operator+ (const Vec2& _lhs, const Vec2& _rhs)
	{
		return Vec2(_lhs.x + _rhs.x, _lhs.y + _rhs.y);
	}
	Vec2 operator- (const Vec2& _lhs, const Vec2& _rhs)
	{
		return Vec2(_lhs.x - _rhs.x, _lhs.y - _rhs.y);
	}
	Vec2 operator* (const Vec2& _lhs, float _rhs)
	{
		return Vec2(_lhs.x * _rhs, _lhs.y * _rhs);
	}
	Vec2 operator* (float _lhs, const Vec2& _rhs)
	{
		return Vec2(_lhs * _rhs.x, _lhs * _rhs.y);
	}
	Vec2 operator/ (const Vec2& _lhs, float _rhs)
	{
		return Vec2(_lhs.x / _rhs, _lhs.y / _rhs);
	}

	// Vec2 Functions
	void vec2_normalize(Vec2& _result, const Vec2& _src)
	{
		Vec2 v(_src.x / vec2_length(_src), _src.y / vec2_length(_src));
		_result = v;

	}
	float vec2_length(const Vec2& _v)
	{
		return sqrt(vec2_squarelength(_v));
	}
	float vec2_squarelength(const Vec2& _v)
	{
		return (_v.x*_v.x + _v.y*_v.y);
	}
	float vec2_distance(const Vec2& _v1, const Vec2& _v2)
	{
		Vec2 v(_v1.x - _v2.x, _v1.y - _v2.y);
		return vec2_length(v);
	}
	float vec2_squaredistance(const Vec2& _v1, const Vec2& _v2)
	{
		Vec2 v(_v1.x - _v2.x, _v1.y - _v2.y);
		return vec2_squarelength(v);
	}
	float vec2_dotproduct(const Vec2& _v1, const Vec2& _v2)
	{
		return (_v1.x * _v2.x + _v1.y * _v2.y);
	}
	float vec2_crossproductmag(const Vec2& _v1, const Vec2& _v2)
	{
		return (_v1.x * _v2.y - _v1.y * _v2.x);
	}
	std::ostream& operator<<(std::ostream& _os, const Vec2& _v) 
	{
		_os << "x:" << _v.x << " " << "y:" << _v.y << std::endl;
		return _os;
	}
	bool Vec2::deserialize(rapidjson::Value& _value	)
	{
		if (!_value.HasMember("X") || !_value.HasMember("Y"))
			return false;

		x = _value["X"].GetFloat();
		y = _value["Y"].GetFloat();
			
		return false;

	}
	bool Vec2::serialize(rapidjson::Value& _value, rapidjson::Document& _doc)
	{
		_value.AddMember("X", x, _doc.GetAllocator());
		_value.AddMember("Y", y, _doc.GetAllocator());

		return true;

	}


	// Vec3-----------------------------------------
	// Vec3 Constructors
	Vec3::Vec3() : data{0.0f} {}
	Vec3::Vec3(float _x, float _y, float _z): x{_x}, y{_y}, z{_z} {}
	Vec3::Vec3(glm::vec3& _v): x{_v.x}, y{_v.y}, z{_v.z} {}

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
	Vec3& Vec3::operator*= (float _rhs)
	{
		x *= _rhs;
		y *= _rhs;
		z *= _rhs;
		return *this;
	}
	Vec3& Vec3::operator/= (float _rhs)
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
	// Vec3 Binary Operators
	Vec3 operator+ (const Vec3& _lhs, const Vec3& _rhs)
	{
		return Vec3(_lhs.x + _rhs.x, _lhs.y + _rhs.y, _lhs.z + _rhs.z);
	}
	Vec3 operator- (const Vec3& _lhs, const Vec3& _rhs)
	{
		return Vec3(_lhs.x - _rhs.x, _lhs.y - _rhs.y, _lhs.z - _rhs.z);
	}
	Vec3 operator* (const Vec3& _lhs, float _rhs)
	{
		return Vec3(_lhs.x * _rhs, _lhs.y * _rhs, _lhs.z * _rhs);
	}
	Vec3 operator* (float _lhs, const Vec3& _rhs)
	{
		return Vec3(_rhs.x * _lhs, _rhs.y * _lhs, _rhs.z * _lhs);
	}
	Vec3 operator/ (const Vec3& _lhs, float _rhs)
	{
		return Vec3(_lhs.x / _rhs, _lhs.y / _rhs, _lhs.z / _rhs);
	}

	// Vec3 Functions
	void vec3_normalize(Vec3& _result, const Vec3& _src)
	{
		Vec3 v(_src.x / vec3_length(_src), _src.y / vec3_length(_src), _src.z / vec3_length(_src));
		_result = v;
	}
	float vec3_length(const Vec3& _v)
	{
		float d;
		d = sqrt(vec3_squarelength(_v));
		return d;
	}
	float vec3_squarelength(const Vec3& _v)
	{
		float d;
		d = _v.x * _v.x + _v.y * _v.y + _v.z * _v.z;
		return d;
	}
	float vec3_distance(const Vec3& _v1, const Vec3& _v2)
	{
		Vec3 v(_v1.x - _v2.x, _v1.y - _v2.y, _v1.z - _v2.z);
		return vec3_length(v);
	}
	float vec3_squaredistance(const Vec3& _v1, const Vec3& _v2)
	{
		Vec3 v(_v1.x - _v2.x, _v1.y - _v2.y, _v1.z - _v2.z);
		return vec3_squarelength(v);
	}
	float vec3_dotproduct(const Vec3& _v1, const Vec3& _v2)
	{
		float d = _v1.x * _v2.x + _v1.y * _v2.y + _v1.z * _v2.z;
		return d;
	}
	Vec3 vec3_crossproduct(const Vec3& _v1, const Vec3& _v2)
	{
		float vx, vy, vz;
		vx = _v1.y * _v2.z - _v1.z * _v2.y;
		vy = _v1.x * _v2.z - _v1.z * _v2.x;
		vz = _v1.x * _v2.y - _v1.y * _v2.x;
		return Vec3(vx, vy, vz);
	}
	std::ostream& operator<<(std::ostream& _os, const Vec3& _v) 
	{
		_os << "x:" << _v.x << " y:" << _v.y << " z:" << _v.z << std::endl;
		return _os;
	}
	bool Vec3::deserialize(rapidjson::Value& _value)
	{
		if (!_value.HasMember("X") || !_value.HasMember("Y") || !_value.HasMember("Z"))
			return false;

		x = _value["X"].GetFloat();
		y = _value["Y"].GetFloat();
		z = _value["Z"].GetFloat();
		return true;

	}
	bool Vec3::serialize(rapidjson::Value& _value, rapidjson::Document& _doc)
	{
		_value.AddMember("X", x, _doc.GetAllocator());
		_value.AddMember("Y", y, _doc.GetAllocator());
		_value.AddMember("Z", z, _doc.GetAllocator());
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
	Matrix3x3::Matrix3x3(const float(&_rhs)[9]) {
		for (int i{ 0 }, k{ 0 }; i < 3; ++i)
		{
			for (int j{ 0 }; j < 3; ++j)
			{
				m[i][j] = _rhs[k];
				++k;
			}
		}
	}
	Matrix3x3::Matrix3x3(float _00, float _01, float _02,
							float _10, float _11, float _12,
							float _20, float _21, float _22)
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
				m[i][j] = _rhs[j][i];
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
				tmp[j][i] = (float) m[i][j];
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
	void matrix3x3_translate(Matrix3x3& _mtx, float _x, float _y) 
	{
		matrix3x3_identity(_mtx);
		_mtx.m[0][2] = _x;
		_mtx.m[1][2] = _y;

	}
	void matrix3x3_scale(Matrix3x3& _mtx, float _x, float _y)
	{
		matrix3x3_identity(_mtx);
		_mtx.m[0][0] = _x;
		_mtx.m[1][1] = _y;
	}
	void matrix3x3_rotrad(Matrix3x3& _mtx, float _angle) 
	{
		matrix3x3_identity(_mtx);

		_mtx.m[0][0] = cosf((float) _angle);
		_mtx.m[0][1] = -sinf((float) _angle);
		_mtx.m[1][0] = sinf((float) _angle);
		_mtx.m[1][1] = cosf((float) _angle);
	}
	void matrix3x3_rotdeg(Matrix3x3& _mtx, float _angle) 
	{
		matrix3x3_identity(_mtx);

		//Convert to radian
		float result = _angle * (PI / 180.0f);

		_mtx.m[0][0] = cosf((float) result);
		_mtx.m[0][1] = -sinf((float) result);
		_mtx.m[1][0] = sinf((float) result);
		_mtx.m[1][1] = cosf((float) result);

	}
	void matrix3x3_transpose(Matrix3x3& _mtx, const Matrix3x3& _rhs) 
	{
		_mtx = _rhs;
		float tmp;

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
	void matrix3x3_inverse(Matrix3x3* _dest, float* _determinant, const Matrix3x3& _src) 
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
				tmp.m[i][j] *= (1.0f / (*_determinant));
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

	float deg_to_rad(float _angle)
	{
		float result = _angle * (PI / 180.0f);
		return result;
	}
	float rad_to_deg(float _angle)
	{
		float result = _angle * (180.0f / PI);
		return result;
	}

	//M2
	// Matrix4x4-----------------------------------------
	Matrix4x4::Matrix4x4()
	{
		for (size_t i{ 0 }; i < 4; ++i)
		{
			for (size_t j{ 0 }; j < 4; ++j)
			{
				m[i][j] = 0.0;
			}
		}
	}

	Matrix4x4::Matrix4x4(const float(&_rhs)[16])
	{
		for (int i{ 0 }, k{ 0 }; i < 4; ++i)
		{
			for (int j{ 0 }; j < 4; ++j)
			{
				m[i][j] = _rhs[k];
				++k;
			}
		}
	}

	Matrix4x4::Matrix4x4(float _00, float _01, float _02, float _03,
		float _10, float _11, float _12, float _13,
		float _20, float _21, float _22, float _23)
	{
		m[0][0] = _00;
		m[0][1] = _01;
		m[0][2] = _02;
		m[0][3] = _03;

		m[1][0] = _10;
		m[1][1] = _11;
		m[1][2] = _12;
		m[1][3] = _13;

		m[2][0] = _20;
		m[2][1] = _21;
		m[2][2] = _22;
		m[2][3] = _23;

	}

	Matrix4x4::Matrix4x4(const glm::mat4x4& _rhs)
	{
		for (int i{ 0 }; i < 4; ++i)
		{
			for (int j{ 0 }; j < 4; ++j)
			{
				m[i][j] = _rhs[j][i];
			}
		}

	}

	Matrix4x4::Matrix4x4(const Matrix4x4& _rhs)
	{
		for (size_t i{ 0 }; i < 4; ++i)
		{
			for (size_t j{ 0 }; j < 4; ++j)
			{
				m[i][j] = _rhs.m[i][j];
			}
		}
	}

	Matrix4x4::Matrix4x4(const glm::vec4& _r0, const glm::vec4& _r1, const glm::vec4& _r2, const glm::vec4& _r3)
	{
		for (unsigned int i{ 0 }; i < 4; ++i)
		{
			m[0][i] = _r0[i];
			m[1][i] = _r1[i];
			m[2][i] = _r2[i];
			m[3][i] = _r3[i];
		}
	}

	Matrix4x4& Matrix4x4::operator=(const Matrix4x4& _rhs)
	{
		for (unsigned int i{ 0 }; i < 4; ++i)
		{
			for (unsigned int j{ 0 }; j < 4; ++j)
			{

				m[i][j] = _rhs.m[i][j];

			}
		}
		return *this;
	}
	Matrix4x4& Matrix4x4::operator=(const glm::mat4x4& _rhs)
	{
		for (unsigned int i{ 0 }; i < 4; ++i)
		{
			for (unsigned int j{ 0 }; j < 4; ++j)
			{

				m[i][j] = _rhs[i][j];

			}
		}
		return *this;
	}
	Matrix4x4& Matrix4x4::operator*=(const Matrix4x4& _rhs)
	{
		Matrix4x4 tmp(*this);
		for (unsigned int i{ 0 }; i < 4; ++i)
		{
			for (unsigned int j{ 0 }; j < 4; ++j)
			{
				m[i][j] = 0.0;
				for (unsigned int k{ 0 }; k < 4; ++k)
				{
					m[i][j] += tmp.m[i][k] * _rhs.m[k][j];
				}
			}
		}
		return *this;
	}
	Matrix4x4::Matrix4x4Proxy::Matrix4x4Proxy(Matrix4x4& _parent, size_t _row) : parent{ _parent }, rowIndex{ _row }{}
	Matrix4x4::Matrix4x4Proxy Matrix4x4::operator[](size_t _row)
	{
		return Matrix4x4::Matrix4x4Proxy(*this, _row);
	}
	float& Matrix4x4::Matrix4x4Proxy::operator[](size_t _col)
	{
		return parent.m[rowIndex][_col];
	}

	Matrix4x4::Matrix4x4ProxyConst::Matrix4x4ProxyConst(const Matrix4x4& _parent, size_t _row) : parent{_parent}, rowIndex{_row}{}
	Matrix4x4::Matrix4x4ProxyConst Matrix4x4::operator[](size_t _row) const
	{
		return Matrix4x4::Matrix4x4ProxyConst(*this, _row);
	}
	const float& Matrix4x4::Matrix4x4ProxyConst::operator[](size_t _col)
	{
		return parent.m[rowIndex][_col];
	}

	glm::mat4x4 Matrix4x4::to_glm() const
	{
		glm::mat4x4 tmp{};
		for (int i{ 0 }; i < 4; ++i)
		{
			for (int j{ 0 }; j < 4; ++j)
			{
				tmp[j][i] = (float)m[i][j];
			}
		}
		return tmp;
	}

	Matrix4x4 operator+(const Matrix4x4& _lhs, const Matrix4x4& _rhs)
	{
		Matrix4x4 tmp;
		for (size_t i{ 0 }; i < 4; ++i)
		{
			for (size_t j{ 0 }; j < 4; ++j)
			{
				tmp.m[i][j] = _lhs.m[i][j] + _rhs.m[i][j];
			}
		}
		return tmp;
	}
	Matrix4x4 operator-(const Matrix4x4& _lhs, const Matrix4x4& _rhs)
	{
		Matrix4x4 tmp;
		for (size_t i{ 0 }; i < 4; ++i)
		{
			for (size_t j{ 0 }; j < 4; ++j)
			{
				tmp.m[i][j] = _lhs.m[i][j] - _rhs.m[i][j];
			}
		}
		return tmp;
	}
	Matrix4x4 operator*(const Matrix4x4& _lhs, const Matrix4x4& _rhs)
	{
		Matrix4x4 tmp(_lhs);
		tmp *= _rhs;
		return tmp;
	}
	Matrix4x4 operator*(float _scalar, const Matrix4x4& _mtx)
	{
		Matrix4x4 tmp(_mtx);
		for (size_t i{ 0 }; i < 4; ++i)
		{
			for (size_t j{ 0 }; j < 4; ++j)
			{
				tmp.m[i][j] *= _scalar;
			}
		}
		return tmp;
	}
	void matrix4x4_identity(Matrix4x4& _rhs)
	{
		for (size_t i{ 0 }; i < 4; ++i)
		{
			for (size_t j{ 0 }; j < 4; ++j)
			{
				if (i == j)
					_rhs.m[i][j] = 1.0;
				else
					_rhs.m[i][j] = 0.0;
			}
		}
	}
	void matrix4x4_translation(Matrix4x4& _mtx, float _x, float _y, float _z)
	{
		// Make mtx into identity mtx first
		matrix4x4_identity(_mtx);
		_mtx.m[0][3] = _x;
		_mtx.m[1][3] = _y;
		_mtx.m[2][3] = _z;
	}
	void matrix4x4_scale(Matrix4x4& _mtx, float _x, float _y, float _z)
	{
		matrix4x4_identity(_mtx);
		_mtx.m[0][0] = _x;
		_mtx.m[1][1] = _y;
		_mtx.m[2][2] = _z;

	}
	void matrix4x4_rot_2D(Matrix4x4& _mtx, float _deg)
	{
		matrix4x4_identity(_mtx);
		_mtx.m[0][0] = cos(_deg);
		_mtx.m[0][1] = -sin(_deg);
		_mtx.m[1][0] = sin(_deg);
		_mtx.m[1][1] = cos(_deg);
	}

	// SPECIFIED DEGREE MUST BE IN RADIANS - for 3D :)
	void matrix4x4_rot_x(Matrix4x4& _mtx, float _deg)
	{
		matrix4x4_identity(_mtx);
		_mtx.m[1][1] = cos(_deg);
		_mtx.m[1][2] = sin(_deg);
		_mtx.m[2][1] = -sin(_deg);
		_mtx.m[2][2] = cos(_deg);
	}
	void matrix4x4_rot_y(Matrix4x4& _mtx, float _deg)
	{
		matrix4x4_identity(_mtx);
		_mtx.m[0][0] = cos(_deg);
		_mtx.m[0][2] = -sin(_deg);
		_mtx.m[2][0] = sin(_deg);
		_mtx.m[2][2] = cos(_deg);
	}
	void matrix4x4_rot_z(Matrix4x4& _mtx, float _deg)
	{
		matrix4x4_identity(_mtx);
		_mtx.m[0][0] = cos(_deg);
		_mtx.m[0][1] = -sin(_deg);
		_mtx.m[1][0] = sin(_deg);
		_mtx.m[1][1] = cos(_deg);
	}

	std::ostream& operator<<(std::ostream& _os, const Matrix4x4& _mtx)
	{
		_os << '{';
		for (int i{ 0 }; i < 4; ++i) {
			for (int j{ 0 }; j < 4; ++j) {
				_os << _mtx.m[i][j];

				if (j == 3 && i == 3)
					continue;

				if (j == 3)
					_os << '\n';
				else
					_os << ',';
			}

		}
		_os << "}";
		return _os;
	}

	// Deserialize support 
	bool deserialize_color(rapidjson::Value& _value, glm::vec4& _v)
	{
		if (!_value.HasMember("R"))
			return false;
		if (!_value.HasMember("G"))
			return false;
		if (!_value.HasMember("B"))
			return false;
		if (!_value.HasMember("A"))
			return false;

		_v[0] = _value["R"].GetFloat();
		_v[1] = _value["G"].GetFloat();
		_v[2] = _value["B"].GetFloat();
		_v[3] = _value["A"].GetFloat();
		return true;

	}

}


