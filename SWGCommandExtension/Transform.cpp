#include "stdafx.h"

#include "Vector.h"
#include "Transform.h"

int Transform::initialized = initialize();

int Transform::initialize() {
	//auto newFunc = xf_matrix_3x4;
	auto original = (uint32_t**)0x1945B04;

	*original = (uint32_t*) &xf_matrix_3x4;

	return 1;
}

void Transform::install() {
	originalInstall::run();

	initialize();
}

void Transform::invert(const Transform &transform) {
	matrix[0][0] = transform.matrix[0][0];
	matrix[0][1] = transform.matrix[1][0];
	matrix[0][2] = transform.matrix[2][0];

	matrix[1][0] = transform.matrix[0][1];
	matrix[1][1] = transform.matrix[1][1];
	matrix[1][2] = transform.matrix[2][1];

	matrix[2][0] = transform.matrix[0][2];
	matrix[2][1] = transform.matrix[1][2];
	matrix[2][2] = transform.matrix[2][2];

	const float x = transform.matrix[0][3];
	const float y = transform.matrix[1][3];
	const float z = transform.matrix[2][3];

	matrix[0][3] = -(matrix[0][0] * x + matrix[0][1] * y + matrix[0][2] * z);
	matrix[1][3] = -(matrix[1][0] * x + matrix[1][1] * y + matrix[1][2] * z);
	matrix[2][3] = -(matrix[2][0] * x + matrix[2][1] * y + matrix[2][2] * z);
}

void Transform::xf_matrix_3x4(float *out, const float *left, const float *right) {
	if (left == out || right == out) {
		float temp[12];

		temp[0] = left[0] * right[0] + left[1] * right[4] + left[2] * right[8];
		temp[1] = left[0] * right[1] + left[1] * right[5] + left[2] * right[9];
		temp[2] = left[0] * right[2] + left[1] * right[6] + left[2] * right[10];
		temp[3] = left[0] * right[3] + left[1] * right[7] + left[2] * right[11] + left[3];

		temp[4] = left[4] * right[0] + left[5] * right[4] + left[6] * right[8];
		temp[5] = left[4] * right[1] + left[5] * right[5] + left[6] * right[9];
		temp[6] = left[4] * right[2] + left[5] * right[6] + left[6] * right[10];
		temp[7] = left[4] * right[3] + left[5] * right[7] + left[6] * right[11] + left[7];

		temp[8] = left[8] * right[0] + left[9] * right[4] + left[10] * right[8];
		temp[9] = left[8] * right[1] + left[9] * right[5] + left[10] * right[9];
		temp[10] = left[8] * right[2] + left[9] * right[6] + left[10] * right[10];
		temp[11] = left[8] * right[3] + left[9] * right[7] + left[10] * right[11] + left[11];

		out[0] = temp[0];
		out[1] = temp[1];
		out[2] = temp[2];
		out[3] = temp[3];
		out[4] = temp[4];
		out[5] = temp[5];
		out[6] = temp[6];
		out[7] = temp[7];
		out[8] = temp[8];
		out[9] = temp[9];
		out[10] = temp[10];
		out[11] = temp[11];
	} else {
		out[0] = left[0] * right[0] + left[1] * right[4] + left[2] * right[8];
		out[1] = left[0] * right[1] + left[1] * right[5] + left[2] * right[9];
		out[2] = left[0] * right[2] + left[1] * right[6] + left[2] * right[10];
		out[3] = left[0] * right[3] + left[1] * right[7] + left[2] * right[11] + left[3];

		out[4] = left[4] * right[0] + left[5] * right[4] + left[6] * right[8];
		out[5] = left[4] * right[1] + left[5] * right[5] + left[6] * right[9];
		out[6] = left[4] * right[2] + left[5] * right[6] + left[6] * right[10];
		out[7] = left[4] * right[3] + left[5] * right[7] + left[6] * right[11] + left[7];

		out[8] = left[8] * right[0] + left[9] * right[4] + left[10] * right[8];
		out[9] = left[8] * right[1] + left[9] * right[5] + left[10] * right[9];
		out[10] = left[8] * right[2] + left[9] * right[6] + left[10] * right[10];
		out[11] = left[8] * right[3] + left[9] * right[7] + left[10] * right[11] + left[11];
	}
}

void Transform::rotate_l2p(const Vector *source, Vector *result, int count) {
	for (; count; --count, ++source, ++result)
	{
		const float x = source->getX();
		const float y = source->getY();
		const float z = source->getZ();
		result->setX(matrix[0][0] * x + matrix[0][1] * y + matrix[0][2] * z);
		result->setY(matrix[1][0] * x + matrix[1][1] * y + matrix[1][2] * z);
		result->setZ(matrix[2][0] * x + matrix[2][1] * y + matrix[2][2] * z);
	}
}

void Transform::reorthonormalize(void) {
	Vector k = getLocalFrameK_p();
	Vector j = getLocalFrameJ_p();

	k.normalize();
	j.normalize();

	Vector i = j.crossProduct(k);

	j = k.crossProduct(i);

	matrix[0][0] = i.getX();
	matrix[1][0] = i.getY();;
	matrix[2][0] = i.getZ();

	matrix[0][1] = j.getX();
	matrix[1][1] = j.getY();
	matrix[2][1] = j.getZ();

	matrix[0][2] = k.getX();
	matrix[1][2] = k.getY();
	matrix[2][2] = k.getZ();
}

void Transform::rotateTranslate_l2p(const Vector *source, Vector *result, int count) {
	for (; count; --count, ++source, ++result) {
		const float x = source->getX();
		const float y = source->getY();
		const float z = source->getZ();

		result->setX(matrix[0][0] * x + matrix[0][1] * y + matrix[0][2] * z + matrix[0][3]);
		result->setY(matrix[1][0] * x + matrix[1][1] * y + matrix[1][2] * z + matrix[1][3]);
		result->setZ(matrix[2][0] * x + matrix[2][1] * y + matrix[2][2] * z + matrix[2][3]);
	}
}

void Transform::rotate_p2l(const Vector *source, Vector *result, int count) {
	for (; count; --count, ++source, ++result) {
		const float x = source->getX();
		const float y = source->getY();
		const float z = source->getZ();

		result->setX(matrix[0][0] * x + matrix[1][0] * y + matrix[2][0] * z);
		result->setY(matrix[0][1] * x + matrix[1][1] * y + matrix[2][1] * z);
		result->setZ(matrix[0][2] * x + matrix[1][2] * y + matrix[2][2] * z);
	}
}

void Transform::rotateTranslate_p2l(const Vector *source, Vector *result, int count)  {
	for (; count; --count, ++source, ++result) {
		const float x = source->getX() - matrix[0][3];
		const float y = source->getY() - matrix[1][3];
		const float z = source->getZ() - matrix[2][3];

		result->setX(matrix[0][0] * x + matrix[1][0] * y + matrix[2][0] * z);
		result->setY(matrix[0][1] * x + matrix[1][1] * y + matrix[2][1] * z);
		result->setZ(matrix[0][2] * x + matrix[1][2] * y + matrix[2][2] * z);
	}
}

const Transform Transform::rotateTranslate_l2pTr(const Transform &t) {
	const Vector i = rotate_l2p(t.getLocalFrameI_p());
	const Vector j = rotate_l2p(t.getLocalFrameJ_p());
	const Vector k = rotate_l2p(t.getLocalFrameK_p());
	const Vector p = rotateTranslate_l2p(t.getPosition_p());

	Transform tmp;
	tmp.setLocalFrameIJK_p(i, j, k);
	tmp.setPosition_p(p);
	tmp.reorthonormalize();

	return tmp;
}

void Transform::yaw_l(float radians) {
	const float sinradians = sin(radians);
	const float cosradians = cos(radians);

	const float a = matrix[0][0];
	const float c = matrix[0][2];
	const float e = matrix[1][0];
	const float g = matrix[1][2];
	const float i = matrix[2][0];
	const float k = matrix[2][2];

	matrix[0][0] = a * cosradians + c * -sinradians;
	matrix[0][2] = a *   sinradians + c * cosradians;

	matrix[1][0] = e * cosradians + g *  -sinradians;
	matrix[1][2] = e *   sinradians + g * cosradians;

	matrix[2][0] = i * cosradians + k *  -sinradians;
	matrix[2][2] = i *   sinradians + k * cosradians;
}

void Transform::pitch_l(float radians) {
	const float sine = sin(radians);
	const float cosine = cos(radians);

	const float b = matrix[0][1];
	const float c = matrix[0][2];
	const float f = matrix[1][1];
	const float g = matrix[1][2];
	const float j = matrix[2][1];
	const float k = matrix[2][2];

	matrix[0][1] = b * cosine + c *   sine;
	matrix[0][2] = b *  -sine + c * cosine;

	matrix[1][1] = f * cosine + g *   sine;
	matrix[1][2] = f *  -sine + g * cosine;

	matrix[2][1] = j * cosine + k *   sine;
	matrix[2][2] = j *  -sine + k * cosine;
}

void Transform::roll_l(float radians) {
	const float sine = sin(radians);
	const float cosine = cos(radians);

	const float a = matrix[0][0];
	const float b = matrix[0][1];
	const float e = matrix[1][0];
	const float f = matrix[1][1];
	const float i = matrix[2][0];
	const float j = matrix[2][1];

	matrix[0][0] = a * cosine + b *   sine;
	matrix[0][1] = a *  -sine + b * cosine;

	matrix[1][0] = e * cosine + f *   sine;
	matrix[1][1] = e *  -sine + f * cosine;

	matrix[2][0] = i * cosine + j *   sine;
	matrix[2][1] = i *  -sine + j * cosine;
}