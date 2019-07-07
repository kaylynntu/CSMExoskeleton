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

//TODO: (confirm) newest value is at the 0th index and oldest is at the largest index
const int NEW_MEASUREMENT_INDEX = 0;
const int OLDEST_MEASUREMENT_INDEX = ROLLING_AVG_SIZE;

//Declare Vars
const int IN_MIN = 0;
const int OUT_MIN = -1;
const int OUT_MAX = 1;

class Actuator {
public:
	Actuator(int pinAct, int pinPot);

	//TODO: WE NEED TO ADD A FUNCTION THE MOVES THE MOTOR BACK AND FORWARD (DECLARED BUT NOT DEFINED)
	void forward();
	void backward();
	void stop();
	void retract();
	void move(speed);

	void moveLeg(int chan, float maxReading, float deadZone, float scaleFactor);
	float translate(float val, int inMin, int inMax,
		int outMin, int outMax);

	int getPin() const { return _pinNum; }
	int getRollingAvgList() const { return _rollingAvgList; }
	int getRollingAvg() const { return _rollingAvg; }
private:
	//pin for the actuator
	int _pinActNum;
	//pin for the potentiometer
	int _pinPotNum;

	float _rollingAvgList[ROLLING_AVG_SIZE] = {};
	float _rollingAvg = 0;
};

//This collects all of the rolling averages for legs and overall intitalizes the leg
Actuator::Actuator(int pinAct, int pinPot) {
	//this assigns the pin number for the actuator
	this->_pinNum = pinAct;

	//this obtains the 
	for (int i = 0; i < ROLLING_AVG_SIZE; i++) {
		int val = translate(analogRead(pinPot), IN_MIN, maxReading, OUT_MIN, OUT_MAX);

		_rollingAvgList[i] = val;
	}

	//sums
	for (int i = 0; i < ROLLING_AVG_SIZE; i++) {
		_rollingAvg += _rollingAvgList[i];
	}

	_rollingAvg /= ROLLING_AVG_SIZE;
}

//Stops actuator
void Actuator::stop() {
	forward(0);
	//TODO: see if these commands work in C++
	actuator.stop();
}

//Retracts actuator
void Actuator::retract() {
	backward(1);
	sleep(2);
	stop(actuator);
}

//Moves actuator (expands)
//A negative speed value will move the actuator backwards
void Actuator::move(speed) {

	//TODO: what happens if speed == 0?, do we want it to move backwards?
	//		Or do we need an error statement?
	if (speed > 0) {
		actuator.forward(speed);
	}
	else {
		actuator.backward(abs(speed));
	}

}


//Translates the raw input from the ADC into a 0-1 number
float Actuator::translate(float val, int inMin, int inMax,
	int outMin, int outMax) {

	int rangeIn = inMax - inMin;
	int rangeOut = outMax - outMin;

	//Converts the value from ADC into a 0-1 range (float value)
	float valueScaled = (float)(val - inMin) / (float)(rangeIn);

	//Shift the 0-1 number into the correct range
	return outMin + (valueScaled * rangeOut);

}



//Here is where we tell things to move
void Actuator::moveLeg(int chan, float maxReading, float deadZone, float scaleFactor) {

	//Insert new reading and delete oldest reading
	rollingAvgList[NEW_MEASUREMENT_INDEX] = translate(chan.voltage, IN_MIN, maxReading, OUT_MIN, OUT_MAX);
	//TODO: change rollingAvgLists to be a queue

	//If the average is larger than the dead zone, then move it
	if (abs(rollingAvg <= deadZone)) {
		stop(actuator);
	}
	else {
		//Do stuff
	}
}


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


//Void loop acts as main
void loop() {

	moveLeg(/*left actuator, some stuff*/);
	moveLeg(/*right actuator, some stuff*/);
}