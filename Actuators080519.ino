// //const int PotR = A5;
// const int PotR = A0;
// //const int ActR = 1;
// const int ActR = 3;
// //const int DirR = 0;
// const int DirR = 2;
//
// const int BUFFER_SIZE = 10;  //higher value means actuator runs smoother, but slower react time
//
// const float DEAD_R = 0.0001;  // VALIDATE ME each run
// const float DEAD_L = 0.0000;  // VALIDATE ME each run
// const int OFFSET_R = 0;
// const int OFFSET_L = 0;  // offset basically controls downward option
// const float AMP_R = 20;
// const float AMP_L = 20;    //amp basically controls upward motion
//
// int voltR, voltL, adjVoltL;
// double speedR, speedL;
//
// void initLeg(float* avg, int* maxIn, char leg);
// void move(float unitRatio, char leg);
//
// void setup(){
//   Serial.begin(9600);
//   Particle.variable("VoltageLeft", voltL);
//   Particle.variable("VoltageRight", voltR);
//   Particle.variable("AdjustedVoltage", adjVoltL);
//   Particle.variable("speedL", speedL);
//   Particle.variable("speedR", speedR);
//   pinMode(ActR, OUTPUT);
//   //pinMode(ActL, OUTPUT);
//   //pinMode(DirL, OUTPUT);
//   pinMode(DirR, OUTPUT);
// }
//
// bool startup = true;
// float avgR, avgL;
// int maxInR, maxInL;
//
// void loop(){
//
//   if(startup){
//     delay(10000);
//     initLeg(&avgR, &maxInR, 'R');
//     //avgL += 800;
//     startup = false;
//     Serial.println("Initialized.");
//   }
//
//   voltR = analogRead(PotR);
//   //adjVoltL = voltL + 800; //TODO: FIX
//   //maxInL += 800;
//
//   avgR = ((BUFFER_SIZE - 1)*avgR + voltR)/BUFFER_SIZE;
//
//
//   float unitRatioR = avgR/maxInR;
//
//   // float unitRatioR = (voltR - avgR)/avgR;
//   // float unitRatioL = (voltL - avgL)/avgL;
//
//   Serial.println(unitRatioR);
//
//   if(abs(voltR-avgR) > DEAD_R){
//     moveRight(unitRatioR, 'R');
//   }
// }
//
//
//
// void initLeg(float* avg, int* maxIn, char leg){
//   int pot;
//     pot = PotR;
//
//   int max = 0;
//   int avgSum = 0;
//   for(int i=0; i<10; i++){
//     int volt = analogRead(pot);
//     avgSum += volt;
//     if(volt>max){
//       max = volt;
//     }
//   }
//
//   *avg = avgSum/10;
//   *maxIn = max*2;
// }
//
//
// void moveRight(float unitRatio, char leg){
//   int act;
//   int dir;
//   int amp;
//   int offset;
//   int sign;
//   act = ActR;
//   dir = DirR;
//   amp = AMP_R;
//   offset = OFFSET_R;
//   sign = 1;
//
//   speedR = sign * 255 * amp * (unitRatio*2 - 1) + offset;
//   // float speed = sign * 255 * unitRatio;
//   analogWrite(act,abs(speedR));
//   if(speedR >= 0){
//     digitalWrite(dir,LOW);
//   }else{
//     digitalWrite(dir,HIGH);
//   }
// }
