#pragma once
#include <string>
//struct resprenting a data type for 3D Vectors
struct Vector3
{
	float x = 0;
	float y = 0;
	float z = 0;

	Vector3(float x = 0.0f, float y = 0.0f, float z = 0.0f)
	{
		this->x = x;
		this->y = y;
		this->z = z;
	}

	/// <summary>
	/// + operator overload to add 2 Vector3s
	/// </summary>
	/// <param name="other">other vector to add to current vector</param>
	/// <returns>the sum of the two vectors</returns>
	Vector3 operator+(Vector3 other)
	{
		Vector3 output;

		output.x = this->x + other.x;
		output.y = this->y + other.y;
		output.z = this->z + other.z;

		return output;
	}

	/// <summary>
	/// + operator overload to add 2 Vector3s
	/// </summary>
	/// <param name="other">other vector to add to current vector</param>
	/// <returns>the sum of the two vectors</returns>
	void operator+=(Vector3 other)
	{
		this->x += other.x;
		this->y += other.y;
		this->z += other.z;
	}

	/// <summary>
	/// - operator overload to subtract 2 Vector3s
	/// </summary>
	/// <param name="other">other vector to add to current vector</param>
	/// <returns>the difference of the two vectors</returns>
	Vector3 operator-(Vector3 other)
	{
		Vector3 output;

		output.x = this->x - other.x;
		output.y = this->y - other.y;
		output.z = this->z - other.z;

		return output;
	}

	/// <summary>
	/// - operator overload to subtract 2 Vector3s
	/// </summary>
	/// <param name="other">other vector to add to current vector</param>
	/// <returns>the difference of the two vectors</returns>
	void operator-=(Vector3 other)
	{
		this->x -= other.x;
		this->y -= other.y;
		this->z -= other.z;
	}

	/// <summary>
	/// * operator overload to multiply a vector by a scalar
	/// </summary>
	/// <param name="other">scalar to multiply with</param>
	/// <returns>vector product</returns>
	Vector3 operator*(float scalar)
	{
		Vector3 output;

		output.x = this->x * scalar;
		output.y = this->y * scalar;
		output.z = this->z * scalar;

		return output;
	}

	/// <summary>
	/// * operator overload to multiply a vector by a scalar
	/// </summary>
	/// <param name="other">scalar to multiply with</param>
	/// <returns>vector product</returns>
	Vector3 operator*(int scalar)
	{
		Vector3 output;

		output.x = this->x * scalar;
		output.y = this->y * scalar;
		output.z = this->z * scalar;

		return output;
	}

	/// <summary>
	/// * operator overload to multiply a vector by a scalar
	/// </summary>
	/// <param name="other">scalar to multiply with</param>
	/// <returns>vector product</returns>
	void operator*=(float scalar)
	{
		this->x *= scalar;
		this->y *= scalar;
		this->z *= scalar;
	}

	/// <summary>
	/// * operator overload to multiply a vector by a scalar
	/// </summary>
	/// <param name="other">scalar to multiply with</param>
	/// <returns>vector product</returns>
	void operator*=(int scalar)
	{
		this->x *= scalar;
		this->y *= scalar;
		this->z *= scalar;
	}



	/// <summary>
	/// * operator overload to multiply a vector by a scalar
	/// </summary>
	/// <param name="other">scalar to multiply with</param>
	/// <returns>vector product</returns>
	Vector3 operator/(float divisor)
	{
		Vector3 output;

		output.x = this->x / divisor;
		output.y = this->y / divisor;
		output.z = this->z / divisor;

		return output;
	}

	/// <summary>
	/// * operator overload to multiply a vector by a scalar
	/// </summary>
	/// <param name="other">scalar to multiply with</param>
	/// <returns>vector product</returns>
	Vector3 operator/(int divisor)
	{
		Vector3 output;

		output.x = this->x / divisor;
		output.y = this->y / divisor;
		output.z = this->z / divisor;

		return output;
	}

	/// <summary>
	/// * operator overload to multiply a vector by a scalar
	/// </summary>
	/// <param name="other">scalar to multiply with</param>
	/// <returns>vector product</returns>
	void operator/=(float divisor)
	{
		this->x /= divisor;
		this->y /= divisor;
		this->z /= divisor;
	}

	/// <summary>
	/// * operator overload to multiply a vector by a scalar
	/// </summary>
	/// <param name="other">scalar to multiply with</param>
	/// <returns>vector product</returns>
	void operator/=(int divisor)
	{
		this->x /= divisor;
		this->y /= divisor;
		this->z /= divisor;
	}

	/// <summary>
	/// = operator overload to assign another vector to a vector
	/// </summary>
	/// <param name="other">other vector to copy values</param>
	void operator=(Vector3 other)
	{
		Vector3 output;

		this->x = other.x;
		this->y = other.y;
		this->z = other.z;

	}

	/// <summary>
	/// == operator overload to compare 2 Vector 3s
	/// </summary>
	/// <param name="other">other vector to compare</param>
	/// <returns>result of sameness comparison</returns>
	bool operator==(Vector3 other)
	{
		if (this->x == other.x && this->y == other.y && this->z == other.z)
		{
			return true;
		}
		
		return false;
	}

	/// <summary>
	/// != operator overload to compare 2 Vector 3s
	/// </summary>
	/// <param name="other">other vector to compare</param>
	/// <returns>result of difference comparison</returns>
	bool operator!=(Vector3 other)
	{
		if (this->x == other.x && this->y == other.y && this->z == other.z)
		{
			return false;
		}

		return true;
	}
};


/// <summary>
/// Responsible for operating and managing 3D Vectors
/// </summary>
class Vector3D
{

private:
	

public:

	Vector3D();
	~Vector3D();

	int DotProduct(Vector3 a = Vector3(), Vector3 b = Vector3());
	Vector3 CrossProduct(Vector3 a = Vector3(), Vector3 b = Vector3());
	float GetMagnitude(Vector3 vector = Vector3());
	Vector3 Normalize(Vector3 vector = Vector3());
	std::string ToString(Vector3 vector = Vector3());
};

