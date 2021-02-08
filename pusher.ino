#include <Servo.h>



/*#####################################################################
 * Block pusher.
 *###################################################################*/

typedef struct pusher_t {
	Servo servo;
	uint32_t pin;
	float rotation; /* 0 thru 1.0f */
} pusher_t;

/* Create new block pusher. */
pusher_t create_pusher(uint32_t __pin)
{
	pusher_t pusher = {};
	pusher.servo.attach(__pin);
	pusher.pin = __pin;
	pusher.rotation = 0.0f;

	return pusher;
}

/* Write servo data to pin. */
void update_pusher(pusher_t __pusher)
{
	/* Clamp to margin. */
	const float margin = 0.03f;
	if (__pusher.rotation > 1.0f - margin)
		__pusher.rotation = 1.0f - margin;
	else if (__pusher.rotation < margin)
		__pusher.rotation = margin;

	float angle = __pusher.rotation * 180.0f;
	__pusher.servo.write((int)angle);
}



/*#####################################################################
 * Pusher Grid.
 *###################################################################*/

/* Define grid of pushers. */
#define GRID_WIDTH 1
#define GRID_HEIGHT 1
pusher_t grid[GRID_WIDTH][GRID_HEIGHT] = {};

/* Create all pushers. */
void create_grid(void)
{
	/* Encode grid pins. */
	const uint32_t pusher_pins[GRID_WIDTH][GRID_HEIGHT] = {
		{ 13 },
	};

	/* Create all pushers. */
	for (uint32_t y = 0; y < GRID_HEIGHT; ++y) {
		for (uint32_t x = 0; x < GRID_WIDTH; ++x) {
			uint32_t pin = pusher_pins[x][y];
			grid[x][y] = create_pusher(pin);
		}
	}
}

void update_grid()
{
	/* Update all pushers. */
	for (uint32_t y = 0; y < GRID_HEIGHT; ++y) {
		for (uint32_t x = 0; x < GRID_WIDTH; ++x) {
			pusher_t pusher = grid[x][y];
			update_pusher(pusher);
		}
	}
}



/*#####################################################################
 * Main.
 *###################################################################*/

/* Update pusher with grid[x][y].rotation = ... */

void setup() {
	create_grid();

}

void loop() {
	delay(1000);
	grid[0][0].rotation = 1.0f;
	update_grid();

	delay(1000);
	grid[0][0].rotation = 0.0f;
	update_grid();
}

