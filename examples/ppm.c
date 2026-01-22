#define NONSTD_IMPLEMENTATION
#include "../nonstd.h"

int main() {
	u32 width = 400;
	u32 height = 400;
	Canvas img = ppm_init(width, height);

	// Background
	ppm_fill(&img, COLOR_HEX(0x1a1a1a));

	// Draw some shapes
	ppm_draw_rect(&img, 50, 50, 100, 100, COLOR_RED);
	ppm_draw_circle(&img, 250, 100, 40, COLOR_BLUE);
	ppm_draw_triangle(&img, 50, 350, 150, 350, 100, 250, COLOR_YELLOW);
	ppm_draw_line(&img, 200, 200, 350, 350, COLOR_GREEN);

	// Random colors and macros
	ppm_draw_rect(&img, 200, 250, 50, 80, COLOR_RGB(255, 165, 0));

	if (ppm_save(&img, "example.ppm")) {
		printf("Image saved to example.ppm\n");
	} else {
		printf("Failed to save image\n");
	}

	ppm_free(&img);
	return 0;
}
