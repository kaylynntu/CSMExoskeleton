//This code was working as of Aug 21, 2019 (hopefully for Celebration of Mines)

const int PotL = A5;
const int PotR = A0;
const int ActR = 1;
const int ActL = 3;
const int DirL = 0;
const int DirR = 2;

const int BUFFER_SIZE = 80;  //higher value means actuator runs smoother, but slower react time

const float DEAD_R = 0.0001;  // VALIDATE ME each run
const float DEAD_L = 0.0000;  // VALIDATE ME each run
const int OFFSET_R = 0;
const int OFFSET_L = 0;  // offset basically controls downward option
const float AMP_R = 20;
const float AMP_L = 20;    //amp basically controls upward motion

int voltR, voltL, adjVoltL;
double speedR, speedL, unitRatioR, unitRatioL;

void initLeg(float* avg, int* maxIn, char leg);
void move(float unitRatio, char leg);

void setup(){
  Serial.begin(9600);
  Particle.variable("VoltageLeft", voltL);
  Particle.variable("VoltageRight", voltR);
  Particle.variable("AdjustedVoltage", adjVoltL);
  Particle.variable("speedL", speedL);
  Particle.variable("speedR", speedR);
  Particle.variable("unitRatioR", unitRatioR);
  Particle.variable("unitRatioL", unitRatioL);
  pinMode(ActR, OUTPUT);
  pinMode(ActL, OUTPUT);
  pinMode(DirL, OUTPUT);
  pinMode(DirR, OUTPUT);
}

bool startup = true;
float avgR, avgL;
int maxInR, maxInL;

void loop(){

  if(startup){
    delay(10000);
    initLeg(&avgR, &maxInR, PotR);
    initLeg(&avgL, &maxInL, PotL);
    startup = false;
    Serial.println("Initialized.");
  }

  voltR = analogRead(PotR);
  voltL = analogRead(PotL);

  avgR = ((BUFFER_SIZE - 1)*avgR + voltR)/BUFFER_SIZE;
  avgL = ((BUFFER_SIZE - 1)*avgL + voltL)/BUFFER_SIZE;

  unitRatioR = avgR/maxInR;
  unitRatioL = avgL/maxInL;

  if(abs(voltR-avgR) > DEAD_R){
    move(unitRatioR, 'R');
  } else if(abs(voltL-avgL) > DEAD_L){
    move(unitRatioL, 'L');
  }
}



void initLeg(float* avg, int* maxIn, int pot){
  int max = 0;
  int avgSum = 0;
  for(int i=0; i<10; i++){
    int volt = analogRead(pot);
    avgSum += volt;
    if(volt>max){
      max = volt;
    }
  }

  *avg = avgSum/10;
  *maxIn = max*2;
}


void move(float unitRatio, char leg){
  int act;
  int dir;
  int amp;
  int offset;
  int sign;
  if (leg == 'R') {
    act = ActR;
    dir = DirR;
    amp = AMP_R;
    offset = OFFSET_R;
  } else if (leg == 'L') {
    act = ActL;
    dir = DirL;
    amp = AMP_L;
    offset = OFFSET_L;
  }
  sign = 1;

  int speed = sign * 255 * amp * (unitRatio*2 - 1) + offset;
  analogWrite(act,abs(speed));
  if(speed >= 0){
    digitalWrite(dir,LOW);
  }else{
    digitalWrite(dir,HIGH);
  }
}
