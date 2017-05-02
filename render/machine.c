#include "machine.h"
#include "window.h"
#include "rendering.h"
Machine mymachine;
int machine_init(int width, int height, const TCHAR* title, HINSTANCE hinstance) {
	memset(&mymachine,0, sizeof(Machine));
	mymachine.fps = 0;
	mymachine.frame_time_last = 0;
	if (window_init(&mymachine.window, width, height, title, hinstance)) {
		printf("can't open the window ");
		return -1;
	}
	device_init(&mymachine.dev, width, height, mymachine.window.screen_framebuf);
	mymachine.render_tick = NULL;
	return 0;
}
void machine_start(AppTick tick) {
	mymachine.render_tick = tick;
	while (mymachine.window.exit == 0) {
		float now = (float)GetTickCount()*0.001f;
		float time_delta = now - mymachine.frame_time_last;
		machine_tick(time_delta);
	}
}
void machine_tick(float time_delta) {
	window_dispatch();
	device_clear(&mymachine.dev,0);
	if (mymachine.render_tick != NULL) {
		mymachine.render_tick(time_delta);
	}
	window_present(&mymachine.window);
	machine_fps();
}
void machine_fps() {
	float now = (float)GetTickCount()*0.001f;
	if (now - mymachine.frame_time_last > 1.0f) {
		printf("FPS: %f\n", mymachine.fps);
		mymachine.frame_time_last = now;
		mymachine.fps = 0;
	}
	else {
		mymachine.fps++;
	}
}