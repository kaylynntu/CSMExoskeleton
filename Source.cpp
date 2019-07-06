//Make sure to make obvious edits. This just gets you a general idea of what it needs to do.

//Actuator Vars
const int ActuatorLDir = D7; //obviously change if applicable
const int ActuatorRDir = D6;
const int ActuatorLPWM = D3; //0-3 have pwm capabilities
const int ActuatorRPWM = D2;

//Linear Poteniometer
const int PotL = A0;
const int PotR = A1;

const int ROLLING_AVG_SIZE = 10;
float rollingAvgListLeft[ROLLING_AVG_SIZE] = {};
float rollingAvgListRight[ROLLING_AVG_SIZE] = {};
float rollingAvgLeft = 0;
float rollingAvgRight = 0;


void setup() {
	pinMode(ActuatorLPWM, OUTPUT);
	pinMode(ActuatorRPWM, OUTPUT);
	pinMode(ActuatorRDir, OUTPUT);
	pinMode(ActuatorLDir, OUTPUT);

	pinMode(PotL, INPUT);
	pinMode(PotR, INPUT);
	
	digitalWrite(ActuatorLPWM, 0);
	digitalWrite(ActuatorRPWM, 0);
	digitalWrite(ActuatorRDir, 0);
	digitalWrite(ActuatorLDir, 0);
	
	Particle.function("getRollingAvgs", &getRollingAvgs, VOID);
	getRollingAvgs();
}

void translate(float val, int in_min, int in_max, int out_min, int out_max) {

}

//This collects all of the rolling averages for legs
void getRollingAvgs() {
	for (int i = 0; i < ROLLING_AVG_SIZE; i++) {
		int left = translate(analogRead(PotL), 0, maxReading;
		int right = analogRead(PotR);

		rollingAvgListLeft[i] = left;
		rollingAvgListRight[i] = right;
	}

	//sums
	for (int i = 0; i < ROLLING_AVG_SIZE; i++) {
		rollingAvgLeft += rollingAvgListLeft[i];
		rollingAvgRight += rollingAvgListRight[i];
	}

	rollingAvgLeft /= ROLLING_AVG_SIZE;
	rollingAvgRight /= ROLLING_AVG_SIZE;
}


void loop() {

}