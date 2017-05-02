#pragma once
#ifndef MACHINE_H_
#define MACHINE_H_
#include"type.h"
extern Machine machine;
int machine_init(int width, int heght, const TCHAR* title, HINSTANCE hinstance);
void machine_start(AppTick tick);
void machine_tick(float time_delta);
void machine_fps();
#endif // !MACHINE_H_
