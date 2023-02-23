
#include "pch.h"
#include "Math/Vector4.h"

namespace Copium::Math {

	// Vec4 ////////////////////
	
	// Constructors
	Vec4::Vec4(float _x, float _y, float _z, float _a) : x{_x}, y{_y}, z{_z}, a{_a} {}
	Vec4::Vec4(const glm::vec4& _src) : x{_src[0]}, y{_src[1]}, z{_src[2]}, a{_src[3]} {}
	Vec4::Vec4(const Vec4& _src) : x{_src.x}, y{_src.y}, z{_src.z}, a{_src.a} {}

	// Copy assignment operator overloads
	Vec4& Vec4::operator=(const Vec4& _src)
	{
		x = _src.x;
		y = _src.y;
		z = _src.z;
		a = _src.a;

		return *this;
	}
	Vec4& Vec4::operator=(const glm::vec4& _src)
	{
		glmVec4 = _src;
		return *this;
	}

	// Unary operator overloads
	Vec4 Vec4::operator-() const
	{
		return Vec4(-x, -y, -z, -a);
	}

	// Compound assignment operator overloads
	Vec4& Vec4::operator+= (const Vec4& _src)
	{
		x += _src.x;
		y += _src.y;
		z += _src.z;
		a += _src.a;

		return *this;
	}
	Vec4& Vec4::operator-= (const Vec4& _src)
	{
		x -= _src.x;
		y -= _src.y;
		z -= _src.z;
		a -= _src.a;

		return *this;
	}
	Vec4& Vec4::operator*= (const Vec4& _src)
	{
		x *= _src.x;
		y *= _src.y;
		z *= _src.z;
		a *= _src.a;

		return *this;
	}
	Vec4& Vec4::operator/= (const Vec4& _src)
	{
		x /= _src.x;
		y /= _src.y;
		z /= _src.z;
		a /= _src.a;

		return *this;
	}

	// Comparison operator overloads
	bool Vec4::operator==(const Vec4& _rhs)
	{
		for (int i{ 0 }; i < 4; ++i)
		{
			if (data[i] != _rhs.data[i])
				return false;
		}

		return true;
	}
	bool Vec4::operator!=(const Vec4& _rhs)
	{
		return !(*this == _rhs);
	}

	// Conversions
	Vec4::operator glm::vec4() { return glmVec4; }

	// De/serialization
	//bool Vec4::Deserialize(rapidjson::Value& _value)
	//{
	//	if (!_value.HasMember("X") || !_value.HasMember("Y") || !_value.HasMember("Z") || !_value.HasMember("A"))
	//		return false;

	//	x = _value["X"].GetFloat();
	//	y = _value["Y"].GetFloat();
	//	z = _value["Z"].GetFloat();
	//	a = _value["A"].GetFloat();


	//	return true;

	//}
	//bool Vec4::Serialize(rapidjson::Value& _value, rapidjson::Document& _doc)
	//{
	//	//_value.AddMember("X", x, _doc.GetAllocator());
	//	//_value.AddMember("Y", y, _doc.GetAllocator());
	//	//_value.AddMember("Z", z, _doc.GetAllocator());
	//	//_value.AddMember("A", a, _doc.GetAllocator());

	//	Copium::Serialize(x, _value, _doc, "X");
	//	Copium::Serialize(y, _value, _doc, "Y");
	//	Copium::Serialize(z, _value, _doc, "Z");
	//	Copium::Serialize(a, _value, _doc, "A");


	//	return true;
	//}

	// Binary operator overloads
	Vec4 operator+ (const Vec4& _lhs, const Vec4& _rhs)
	{
		Vec4 tmp(_lhs);
		tmp += _rhs;
		return tmp;
	}
	Vec4 operator- (const Vec4& _lhs, const Vec4& _rhs)
	{
		Vec4 tmp(_lhs);
		tmp -= _rhs;
		return tmp;
	}
	Vec4 operator* (const Vec4& _lhs, float _rhs)
	{
		Vec4 tmp(_lhs);
		for (int i{ 0 }; i < 4; ++i)
		{
			tmp.data[i] *= _rhs;
		}
		return tmp;
	}
	Vec4 operator* (float _lhs, const Vec4& _rhs)
	{
		return _rhs * _lhs;
	}
	Vec4 operator/ (const Vec4& _lhs, float _rhs)
	{
		Vec4 tmp(_lhs);
		for (int i{ 0 }; i < 4; ++i)
		{
			tmp.data[i] /= _rhs;
		}
		return tmp;
	}

	// Vec4 vector operations
	void Vec4Normalize(Vec4& _result, const Vec4& _src)
	{
		Vec4 tmp(_src.x / Vec4Length(_src), _src.y / Vec4Length(_src),
				_src.z / Vec4Length(_src), _src.a / Vec4Length(_src));
		_result = tmp;
	}
	float Vec4Length(const Vec4& _v)
	{
		return sqrtf(Vec4SquareLength(_v));
	}
	float Vec4SquareLength(const Vec4& _v)
	{
		return (_v.x * _v.x) + (_v.y * _v.y) + (_v.z * _v.z) + (_v.a * _v.a);
	}
	float Vec4Distance(const Vec4& _v1, const Vec4& _v2)
	{
		return sqrtf(Vec4SquaredDistance(_v1, _v2));

	}
	float Vec4SquaredDistance(const Vec4& _v1, const Vec4& _v2)
	{
		return Vec4SquareLength(_v1 - _v2);
	}
	float Vec4DotProduct(const Vec4& _v1, const Vec4& _v2)
	{
		float result{ 0.0f };
		for (int i{ 0 }; i < 4; ++i)
		{
			result += (_v1.data[i] * _v2.data[i]);
		}
		return result;
	}

	// Misc operations
	std::ostream& operator<<(std::ostream& _os, const Vec4& _v)
	{
		_os << "x:" << _v.x << " y:" << _v.y << " z:" << _v.z << "a:" << _v.a << std::endl;
		return _os;
	}
	////////////////////////////

}
