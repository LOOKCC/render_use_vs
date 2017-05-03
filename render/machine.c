#include "machine.h"
#include "window.h"
#include "rendering.h"
Machine machine;
int machine_init(int width, int height, const TCHAR* title, HINSTANCE hinstance) {
	memset(&machine,0, sizeof(Machine));
	machine.fps = 0;
	machine.frame_time_last = 0;
	if (window_init(&machine.window, width, height, title, hinstance)) {
		printf("can't open the window ");
		return -1;
	}
	device_init(&machine.dev, width, height, machine.window.screen_framebuf);
	machine.render_tick = NULL;
	return 0;
}
void machine_start(AppTick tick) {
	machine.render_tick = tick;
	while (machine.window.exit == 0) {
		float now = (float)GetTickCount()*0.001f;
		float time_delta = now - machine.frame_time_last;
		machine_tick(time_delta);
	}
}
void machine_tick(float time_delta) {
	window_dispatch();
	device_clear(&machine.dev,0);
	if (machine.render_tick != NULL) {
		machine.render_tick(time_delta);
	}
	window_present(&machine.window);
	machine_fps();
}
void machine_fps() {
	float now = (float)GetTickCount()*0.001f;
	if (now - machine.frame_time_last > 1.0f) {
		printf("FPS: %f\n", machine.fps);
		machine.frame_time_last = now;
		machine.fps = 0;
	}
	else {
		machine.fps++;
	}
}