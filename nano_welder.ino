//attach mosfet gate pin to mosfetGate (PD4) and mosfet source (ground) pin to GND
//attach footswitch 1st pin  to footSwitch (A5) and 2nd pin to GND

const int footSwitch = A5;
const int mosfetGate = PD4;
void setup() {
  // put your setup code here, to run once:
  pinMode(mosfetGate, OUTPUT);
  pinMode(footSwitch, INPUT_PULLUP);
}
int firstPulseMS = 10;
int secondPulseMS = 35;
bool weldLock = false;
int loopCountSinceHigh = 0;
constexpr int pedalWeldLockHoldCount = 10000;//hold the pedal low for this many loops before we can do weld again; prevent erroneous double taps
void doWeld(int, int);
void loop() {
  int switchState = digitalRead(footSwitch);//Switch on pin footSwitch is normally open, LOW when pressed, HIGH when open
  if (switchState == LOW) { //LOW when switch is pressed
    loopCountSinceHigh = 0;
    //timeStart = millis();
    if (!weldLock){
      doWeld(firstPulseMS, secondPulseMS);
      weldLock = true;
    }
  }
  else if (switchState == HIGH){ //HIGH when switch is open
    if(loopCountSinceHigh<pedalWeldLockHoldCount){
      loopCountSinceHigh++;
    }
    if(loopCountSinceHigh==pedalWeldLockHoldCount){
       weldLock = false;
    }
  }
}
void doWeld(int firstPulseMS, int secondPulseMS) {
  closeCircuitForDuration(firstPulseMS);
  delay(20);
  closeCircuitForDuration(secondPulseMS);
}
void closeCircuitForDuration(int milliseconds) {
  if(milliseconds>0){
    digitalWrite(mosfetGate, HIGH);
    delay(milliseconds);
    digitalWrite(mosfetGate, LOW);
  }
}
