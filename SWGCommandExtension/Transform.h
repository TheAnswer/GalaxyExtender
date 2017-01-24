#pragma once
#include "Vector.h"
#include "soewrappers.h"

class Transform {
	float matrix[3][4];

	static int initialized;
public:
	static int initialize();

	static void install();

	DEFINE_HOOK(0x00AB8DD0, install, originalInstall);

	Vector getPosition_p() const {
		return Vector(matrix[0][3], matrix[1][3], matrix[2][3]);
	}

	void setPosition_p(const Vector &vec) {
		matrix[0][3] = vec.getX();
		matrix[1][3] = vec.getY();
		matrix[2][3] = vec.getZ();
	}

	void invert(const Transform &transform);
	DEFINE_HOOK(0x00AB93B0, invert, originalInvert);

	void rotate_l2p(const Vector *source, Vector *result, int count);
	DEFINE_HOOK(0x00AB9560, rotate_l2p, originalRotate_l2p);

	void reorthonormalize();
	DEFINE_HOOK(0x00AB9440, reorthonormalize, origReortho);

	void rotateTranslate_l2p(const Vector * source, Vector * result, int count);
	DEFINE_HOOK(0x00AB95D0, rotateTranslate_l2p, origRotateTra);

	void rotate_p2l(const Vector * source, Vector * result, int count);
	DEFINE_HOOK(0x00AB9640, rotate_p2l, originalP2lRotate);

	void rotateTranslate_p2l(const Vector * source, Vector * result, int count);
	DEFINE_HOOK(0x00AB96B0, rotateTranslate_p2l, originalP2lRotateTranslate);

	const Transform rotateTranslate_l2pTr(const Transform & t);
	DEFINE_HOOK(0x00AB9720, rotateTranslate_l2pTr, originalTranslatel2pTr);

	void yaw_l(float radians);
	DEFINE_HOOK(0x00AB8E10, yaw_l, originalYawl);

	void pitch_l(float radians);
	DEFINE_HOOK(0x00AB8EA0, pitch_l, originalPitchL);

	void roll_l(float radians);
	DEFINE_HOOK(0x00AB8F30, roll_l, originalRollL);

	static void xf_matrix_3x4(float *out, const float *left, const float *right);

	inline const Vector Transform::getLocalFrameK_p(void) const {
		return Vector(matrix[0][2], matrix[1][2], matrix[2][2]);
	}

	inline const Vector Transform::getLocalFrameJ_p(void) const {
		return Vector(matrix[0][1], matrix[1][1], matrix[2][1]);
	}

	inline const Vector Transform::getLocalFrameI_p(void) const {
		return Vector(matrix[0][0], matrix[1][0], matrix[2][0]);
	}

	inline void Transform::setLocalFrameIJK_p(const Vector &i, const Vector &j, const Vector &k) {
		matrix[0][0] = i.getX();
		matrix[1][0] = i.getY();
		matrix[2][0] = i.getZ();

		matrix[0][1] = j.getX();
		matrix[1][1] = j.getY();
		matrix[2][1] = j.getZ();

		matrix[0][2] = k.getX();
		matrix[1][2] = k.getY();
		matrix[2][2] = k.getZ();
	}

	inline const Vector Transform::rotate_l2p(const Vector &vec) const {
		return Vector(
			matrix[0][0] * vec.getX() + matrix[0][1] * vec.getY() + matrix[0][2] * vec.getZ(),
			matrix[1][0] * vec.getX() + matrix[1][1] * vec.getY() + matrix[1][2] * vec.getZ(),
			matrix[2][0] * vec.getX() + matrix[2][1] * vec.getY() + matrix[2][2] * vec.getZ());
	}

	inline const Vector Transform::rotateTranslate_l2p(const Vector &vec) const {
		return Vector(
			matrix[0][0] * vec.getX() + matrix[0][1] * vec.getY() + matrix[0][2] * vec.getZ() + matrix[0][3],
			matrix[1][0] * vec.getX() + matrix[1][1] * vec.getY() + matrix[1][2] * vec.getZ() + matrix[1][3],
			matrix[2][0] * vec.getX() + matrix[2][1] * vec.getY() + matrix[2][2] * vec.getZ() + matrix[2][3]);
	}

};