#include"stdafx.h"
#include"camera.h"
void camera_at_zero(device *device, float x, float y, float z) {
	vector eye = { x, y, z, 1 }, at = { 0, 0, 0, 1 }, up = { 0, 0, 1, 1 };
	matrix_set_lookat(&device->t.view, &eye, &at, &up);
	transform_update(&device->t);
}