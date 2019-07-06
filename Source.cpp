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
//TODO: (confirm) newest value is at the 0th index and oldest is at the largest index
const int NEW_MEASUREMENT_INDEX = 0;
const int OLDEST_MEASUREMENT_INDEX = ROLLING_AVG_SIZE;
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

//Stops actuator
void stop(actuator) {
	actuator.forward(0);
	//TODO: see if these commands work in C++
	actuator.stop();
}

//Retracts actuator
void retract(actuator) {
	actuator.backward(1);
	sleep(2);
	stop(actuator);
}

//Moves actuator (expands)
	//A negative speed value will move the actuator backwards
void move(actuator, speed) {
	
	//TODO: what happens if speed == 0?, do we want it to move backwards?
	//		Or do we need an error statement?
	if (speed > 0) {
		actuator.forward(speed);
	} else {
		actuator.backward(abs(speed));
	}
		
}


//Translates the raw input from the ADC into a 0-1 number
float translate(float val, int inMin, int inMax, 
				int outMin, int outMax) {
	
	int rangeIn = inMax - inMin;
	int rangeOut = outMax - outMin;
	
	//Converts the value from ADC into a 0-1 range (float value)
	float valueScaled = (float)(val - inMin) / (float)(rangeIn);
	
	//Shift the 0-1 number into the correct range
	return outMin + (valueScaled * rangeOut);

}

//This collects all of the rolling averages for legs
void getRollingAvgs() {
	
	for (int i = 0; i < ROLLING_AVG_SIZE; i++) {
		int left = translate(analogRead(PotL), IN_MIN, maxReading, OUT_MIN, OUT_MAX);
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

//Here is where we tell things to move
void moveLegs(actuator, chan, maxReading, deadZone, scaleFactor, &rollingAvgList) {
	//Declare Vars
	int IN_MIN = 0;
	int OUT_MIN = -1;
	int OUT_MAX = 1;
	
	//Insert new reading and delete oldest reading
	rollingAvgList[NEW_MEASUREMENT_INDEX] = translate(chan.voltage, IN_MIN, maxReading, OUT_MIN, OUT_MAX);
	//TODO: change rollingAvgLists to be a queue
	
	//If the average is larger than the dead zone, then move it
	if (abs(rollingAvg <= deadZone)) {
		stop(actuator);
	} else {
		//Do stuff
	}
}


//Void loop acts as main
void loop() {
	moveLeg(/*left actuator, some stuff*/);
	moveLeg(/*right actuator, some stuff*/);
}