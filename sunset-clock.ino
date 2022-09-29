#include <Wire.h>
#include <DS3231.h>
#include <Dusk2Dawn.h>

DS3231 clock;
RTCDateTime dt;

char sunset[6];
char sunrise[6];

Dusk2Dawn mainz(49.98419, 8.2791, 1);

int latchPin = 8;
int clockPin = 12;
int dataPin = 11;

int start;
int end;
int cur;
int ledNum;

byte leds = 0;
byte shiftBits = 0;
bool blink = true;


void printTime(RTCDateTime dt) {
  Serial.print("current time: ");
  Serial.print(dt.hour);   Serial.print(":");
  Serial.print(dt.minute); Serial.print(":");
  Serial.print(dt.second); Serial.println("");
}

void printDate(RTCDateTime dt) {
  Serial.print("current date: ");
  Serial.print(dt.year);   Serial.print("-");
  Serial.print(dt.month);  Serial.print("-");
  Serial.print(dt.day);    Serial.println(" ");
}

int getSunrise(RTCDateTime dt) {
  int mzsunrise = mainz.sunrise(dt.year, dt.month, dt.day, false);
  Dusk2Dawn::min2str(sunrise , mzsunrise);
  //Serial.print("sunrise today at: "); Serial.println(sunrise);
  return (int)(((float)mzsunrise / 60) + 0.5);
}

int getSunset(RTCDateTime dt) {
  int mzsunset = mainz.sunset(dt.year, dt.month, dt.day, false);
  Dusk2Dawn::min2str(sunset , mzsunset);
  //Serial.print("sunset today at: "); Serial.println(sunset);
  return (int)(((float)mzsunset / 60) + 0.5);
}

void setup()
{
  Serial.begin(9600);

  // clock setup
  clock.begin();

  // uncomment to reset time to system time
  // clock.setDateTime(__DATE__, __TIME__);

  dt = clock.getDateTime();
  printTime(dt);

  // register setup
  pinMode(latchPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
}

void loop()
{
  // get current time
  dt = clock.getDateTime();
  start = getSunrise(dt);
  end = getSunset(dt);
  cur = dt.hour;
  digitalWrite(latchPin, LOW);
  for (int j = 0; j < 3; j++) {
    leds = 0;
    for (int i = 0; i < 8; i++) {
      // get sun time
      ledNum = i+(8*j);
      if (((start<=ledNum) and (end>ledNum)) or (ledNum == cur)) {
        // blink for current time
        if(ledNum != cur or blink){
         bitSet(leds, i);
        }
      }
    }
    shiftOut(dataPin, clockPin, LSBFIRST, leds);
  }
  blink = !blink;
  digitalWrite(latchPin, HIGH);
  delay(500);
}
