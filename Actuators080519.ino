const int PotR = A0;
const int PotL = A1;
const int ActR = 1;
const int ActL = 3;
const int DirR = 0;
const int DirL = 2;

const int BUFFER_SIZE = 80;  //10

const float DEAD_R = 0.0001;  // const float DEAD_R = 0;
const float DEAD_L = 0.0190;  // const float DEAD_L = 0;
const int OFFSET_R = 0;
const int OFFSET_L = 150;  // const int OFFSET_L = 0
const float AMP_R = 20;
const float AMP_L = 20;

void initLeg(float* avg, int* maxIn, char leg);
void move(float unitRatio, char leg);

void setup(){
  Serial.begin(9600);
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
    initLeg(&avgR, &maxInR, 'R');
    initLeg(&avgL, &maxInL, 'L');
    startup = false;
    Serial.println("Initialized.");
  }

  int voltR = analogRead(PotR);
  int voltL = analogRead(PotL);

  avgR = ((BUFFER_SIZE - 1)*avgR + voltR)/BUFFER_SIZE;
  avgL = ((BUFFER_SIZE - 1)*avgL + voltL)/BUFFER_SIZE;

  float unitRatioR = avgR/maxInR;
  float unitRatioL = avgL/maxInL;

  // float unitRatioR = (voltR - avgR)/avgR;
  // float unitRatioL = (voltL - avgL)/avgL;

  Serial.println(unitRatioR);

  if(abs(voltR-avgR) > DEAD_R){
    move(unitRatioR, 'R');
  }
  if(abs(voltL-avgL) > DEAD_L){
    move(unitRatioL, 'L');
  }
}



void initLeg(float* avg, int* maxIn, char leg){
  int pot;
  if(leg = 'R'){
    pot = PotR;
  }else{
    pot = PotL;
  }

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
  if(leg == 'R'){
    act = ActR;
    dir = DirR;
    amp = AMP_R;
    offset = OFFSET_R;
    sign = 1;
  }else{
    act = ActL;
    dir = DirL;
    amp = AMP_L;
    offset = OFFSET_L;
    sign = -1;
  }
  float speed = sign * 255 * amp * (unitRatio*2 - 1) + offset;
  // float speed = sign * 255 * unitRatio;
  analogWrite(act,abs(speed));
  if(speed >= 0){
    digitalWrite(dir,LOW);
  }else{
    digitalWrite(dir,HIGH);
  }
}
