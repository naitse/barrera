#include <Servo.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif
#include <DFPlayerMini_Fast.h>
DFPlayerMini_Fast player;

unsigned long previousMillis = 0;
unsigned long previousMillisAudio = 0;
unsigned long previousMillisBarreraBaja = 0;
const long interval = 1000;
const long intervalBarreraUP = 60000; 
int ledState = LOW;
unsigned long currentMillis = 0;
unsigned long currentMillisAudio = 0;
unsigned long barreraBajaStartMills = 0;
const long intervalAudio = 2040; 



int executed = 0;
int started = 0;
int val = 0;
Servo myservo;
int pos = 18;
int led0 = 5;
int led1 = 7;
int volve = 1;
boolean isPlaying = false;

void setup() {
  Serial.begin(9600);
  pinMode(led0, OUTPUT);
  pinMode(led1, OUTPUT);
  pinMode(2, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(2), flagme, FALLING);
  myservo.attach(9);
  myservo.write(28);
  myservo.write(18);
  // Start communication with DFPlayer Mini
  player.begin(Serial);
  player.volume(30);
}

void loop() {
  val = digitalRead(2);
  if (val == LOW) {
    play();
  }
  while(volve == 0) {
    currentMillis = millis();
    currentMillisAudio = millis();
    ledControl();
    audioLoop();
    barreraState();
  }
}

void flagme() {
  volve = 1;
}

void play(){
  if(started == 0){
    started = 1;
    if(executed == 0){
      executed = 1 ;
      bajar();  
    } else {
      executed = 0;
      player.pause();
      delay(500);
      ledStop();
      subir();
    }
    started = 0;
  }  
}

void bajar() {
  for (pos = 18; pos <= 112; pos += 1) {
    myservo.write(pos);
    currentMillis = millis(); 
    currentMillisAudio = millis();
    ledControl();
    audioLoop();
    delay(40);                       
  }
  barreraBajaStartMills = millis();
  volve = 0;
}

void subir() {
    for (pos = 112; pos >= 18; pos -= 1) { 
    myservo.write(pos);             
    delay(40);                       
  }
  volve = 1;
}

void ledControl() {
   if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    if (ledState == LOW) {
      ledState = HIGH;
    } else {
      ledState = LOW;
    }
    // set the LED with the ledState of the variable:
    digitalWrite(led0, ledState);
    digitalWrite(led1, !ledState);
  }
}

void audioLoop(){
   if (currentMillisAudio - previousMillisAudio >= intervalAudio) {
    player.play(1);
    previousMillisAudio = currentMillisAudio;
  }
}

void ledStop() {
  digitalWrite(led0, LOW);
  digitalWrite(led1, LOW);  
}

void barreraState(){
    if(barreraBajaStartMills + intervalBarreraUP < millis()){
      play();
    }
}
