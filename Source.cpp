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
	Actuator(int pinAct, int pinPot, int pinDir);

	//Basic movement controls
	void forward(int speed);
	void backward(int speed);
	void stop();
	void retract();
	void move(int speed);

	//These are the "main" functions that run all necessary bits 
	void moveLeg(float maxReading, float deadZone, float scaleFactor);
	float translate(float val, int inMin, int inMax,
		int outMin, int outMax);

	//GETTERS
	int getActPin() const { return _pinActNum; }
	int getDirPin() const { return _pinDirNum; }
	int getPotPin() const { return _pinPotNum; }
	int getRollingAvgList() const { return _rollingAvgList; }
	int getRollingAvg() const { return _rollingAvg; }
private:
	//pin for the actuator
	int _pinActNum;
	//pin for the potentiometer
	int _pinPotNum;
	//pin for the direction for our linear actuator
	int _pinDirNum;

	float _rollingAvgList[ROLLING_AVG_SIZE] = {};
	float _rollingAvg = 0;
};

//This collects all of the rolling averages for legs and overall intitalizes the leg
Actuator::Actuator(int pinAct, int pinPot, int pinDir) {
	//this assigns the pin number for the actuator
	this->_pinActNum = pinAct;
	this->_pinPotNum = pinPot;
	this->_pinDirNum = pinDir;

	//this obtains the 
	for (int i = 0; i < ROLLING_AVG_SIZE; i++) {
		//TODO: FIND OUT MAXREADING VALUE HERE 
		int val = translate(analogRead(_pinPotNum), IN_MIN, maxReading, OUT_MIN, OUT_MAX);

		_rollingAvgList[i] = val;
	}

	//sums
	for (int i = 0; i < ROLLING_AVG_SIZE; i++) {
		_rollingAvg += _rollingAvgList[i];
	}

	_rollingAvg /= ROLLING_AVG_SIZE;
}

void Actuator::forward(int speed) {
	analogWrite(_pinActNum, speed);
	digitalWrite(_pinDirNum, HIGH);
}

void Actuator::backward(int speed) {
	analogWrite(_pinActNum, speed);
	digitalWrite(_pinDirNum, LOW);
}

//Stops actuator
void Actuator::stop() {
	forward(0);
}

//Retracts actuator
void Actuator::retract() {
	backward(1);
	delay(2000);
	stop()
}

//Moves actuator (expands)
//A negative speed value will move the actuator backwards
void Actuator::move(int speed) {

	if (speed > 0) {
		forward(speed);
	}
	else if (speed < 0){
		backward(abs(speed));
	}
	else {
		stop()
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
void Actuator::moveLeg(float maxReading, float deadZone, float scaleFactor) {

	//Insert new reading and delete oldest reading
	rollingAvgList[NEW_MEASUREMENT_INDEX] = translate(analogRead(_pinPotNum), IN_MIN, maxReading, OUT_MIN, OUT_MAX);
	//TODO: change rollingAvgLists to be a queue

	//If the average is larger than the dead zone, then move it
	if (abs(_rollingAvg <= deadZone)) {
		stop();
	}
	else {
		move(_rollingAvg)
	}
}

//initalizing the acutators
actuatorL = Actuator(ActuatorLPWM, PotL, ActuatorLDir);
actuatorR = Actuator(ActuatorRPWM, PotR, ActuatorRDir);

void setup() {
	pinMode(ActuatorLPWM, OUTPUT);
	pinMode(ActuatorRPWM, OUTPUT);
	pinMode(ActuatorRDir, OUTPUT);
	pinMode(ActuatorLDir, OUTPUT);

	pinMode(PotL, INPUT);
	pinMode(PotR, INPUT);
	
	digitalWrite(ActuatorLPWM, LOW);
	digitalWrite(ActuatorRPWM, LOW);
	digitalWrite(ActuatorRDir, LOW);
	digitalWrite(ActuatorLDir, LOW);
	
	//TODO: ADD FUNCTIONS FOR THE PARTICLE
	//Particle.function("name", &name, TYPE);
}


//Void loop acts as main
void loop() {
	
	actuatorL.moveLeg(/*some stuff*/);
	actuatorR.moveLeg(/*some stuff*/);
	
}