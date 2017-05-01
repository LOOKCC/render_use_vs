#pragma once
#ifndef LIGHT_H_
#define LIGHT_H_
#include"type.h"
Light * light_create_point(Color color, vector position);
void light_destory(Light* light);
#endif // !LIGHT_H_
