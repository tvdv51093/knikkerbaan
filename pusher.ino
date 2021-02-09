#include <Servo.h>

/* Bedient een servo en duwt daarmee een blokje. */
class duwer {
	private:
		Servo servo;
		int pin = 0;
	
	public:
		float rotatie; /* 0 tot 1.0f */

		duwer() { }
		duwer(int __pin) {
			servo.attach(__pin);
			pin = __pin;
			rotatie = 0.0f;
		}

		void update() {
			/* Hou binnen 0 en 1 */
			float margin = 0.03f;		
			if (rotatie > 1.0f - margin) 
				rotatie = 1.0f - margin;
			else if (rotatie < margin) 
				rotatie = margin;

			float hoek = rotatie * 180.0f;
			servo.write((int)hoek);
		}
};

#define VELD_BREEDTE 2
#define VELD_HOOGTE 2

/* Bedient alle servo's. */
class duwer_veld {
	public:
		duwer duwers[VELD_BREEDTE][VELD_HOOGTE];

		duwer_veld() {
			/* 2D veld van duwers */
			const int pins[VELD_BREEDTE][VELD_HOOGTE] = {
				{ 13, 12 },
				{ 11, 10 }	
			};

			for (int y = 0; y < VELD_HOOGTE; ++y){
				for (int x = 0; x < VELD_BREEDTE; ++x) {
					duwers[x][y] = duwer(pins[x][y]);
				}
			}
		}

		void update() {
			for (int y = 0; y < VELD_HOOGTE; ++y){
				for (int x = 0; x < VELD_BREEDTE; ++x) {
					duwers[x][y].update();
				}
			}
		}	
};

duwer_veld *veld = nullptr;

void setup() {
	veld = new duwer_veld();
}

float time = 0.0f;
int delayms = 20;

void loop() {
	for (int y = 0; y < VELD_HOOGTE; ++y) {
		for (int x = 0; x < VELD_BREEDTE; ++x){
			veld->duwers[x][y].rotatie = 0.5f + 0.5f * sin((float)(x + y)/3.0f + time);
		}
	}
	veld->update();

	time += delayms / 1000.0f;
	delay(delayms);
}
