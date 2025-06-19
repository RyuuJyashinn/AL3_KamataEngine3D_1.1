#pragma once

namespace KamataEngine {

struct Vector3 final {
	float x;
	float y;
	float z;

	Vector3() : x(0.0f), y(0.0f), z(0.0f) {}
	Vector3(float x, float y, float z) : x(x), y(y), z(z) {}

	// 仅保留 operator+= 作为成员函数
	Vector3& operator+=(const Vector3& other) {
		x += other.x;
		y += other.y;
		z += other.z;
		return *this;
	}
};
inline Vector3 operator*(const Vector3& vec, float scalar) { return Vector3(vec.x * scalar, vec.y * scalar, vec.z * scalar); }

inline Vector3 operator+(const Vector3& a, const Vector3& b) { return Vector3(a.x + b.x, a.y + b.y, a.z + b.z); }

// 全局 operator-（避免冲突）
inline Vector3 operator-(const Vector3& a, const Vector3& b) { return Vector3(a.x - b.x, a.y - b.y, a.z - b.z); }
} // namespace KamataEngine