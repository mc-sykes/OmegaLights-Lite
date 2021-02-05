#include "Thread.h"
#include "ThreadController.h"
#include <FastLED.h>
#include <IRremote.h>
#include <TimerOne.h>

class ListenerThread: public Thread
{
public:
  int pin;
  decode_results results;
  static IRrecv receiver;
  long code;

  void run(){
    if(receiver.decode(&results)) {
      code = results.value;
      //Serial.println(results.value);
      receiver.resume();
  }
    runned();
  }
};
/* **************************************************** End of Class Declaration ***********************************************************/
ListenerThread IR = ListenerThread();
ThreadController controller = ThreadController();
void timerCallback(){
  controller.run();
}

#define LED_TYPE WS2812B
#define NUM_LEDS 150
#define LED_PIN 7
#define COLOR_ORDER GRB

CRGB led[NUM_LEDS];
long startTime = millis();
long currentTime;

/*Button Definitions*/
#define RED 16718565
#define ORGRED 16722645
#define ORANGE 16714485
#define PEACHPUFF 16726215
#define YELLOW 16718055
#define GREEN 16751205
#define LIME 16755285
#define AQUAMARINE 16747125
#define JELLYBEAN 16758855
#define NORDIC 16750695
//#define CURIOUSBLUE
//#define PURSSIANBLUE
#define BLUE 16753245
#define ROYALBLUE 16749165
#define MIDNIGHTBLUE 16757325
#define PURPLE 16742535
#define PINK 16734375
#define WHITE 16720605

#define CYAN 16775175
#define SKYBLUE 16767015
#define DIY1 16724175
#define DIY2 16756815
#define POWER 16712445

// Pushes pixels over one
void shift(int NUM){
  for(int j = NUM-1; j > 0; j--){
    led[j] = led[j-1];
  }
  FastLED.show();
}

// Rainbow wheel
CRGB hueRainbow(long delayTime){
  long orig = IR.code;
  while(IR.code != 16712445){
    int hue = 0;
    while(hue < 255 && IR.code != 16712445){
      currentTime = millis();
      CRGB newPixel = CHSV(hue, 255, 255);
      led[0] = newPixel;
      if(currentTime - startTime > delayTime){
        shift(NUM_LEDS);
        hue++;
        startTime = millis();
      }
    }
  }
}

//fill the strip with one color starting from middle
void fill(CRGB color, long delayTime){
  int j = (NUM_LEDS/2);
  int i;
  for(i = (NUM_LEDS/2)-1; i >= 0; i--){
    led[i] = color;
    led[j] = color;
    FastLED.show();
    delay(delayTime);
    j++;
  }
}

// Clear strip
void off(){
  int i;
  for(i = 0; i < NUM_LEDS/2; i++){
    led[i] = CRGB::Black;
    led[NUM_LEDS-i-1] = CRGB::Black;
    FastLED.show();
    delay(7);
  }
}

void setup(){
  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(led, NUM_LEDS);
  //Serial.begin(9600);
  IR.receiver.enableIRIn();
  IR.pin = 5;
  IRrecv r(IR.pin);
  IR.receiver = r;
  IR.setInterval(0);
  controller.add(&IR);
  
  Timer1.initialize(2000);
  Timer1.attachInterrupt(timerCallback);
  Timer1.start();
  off();
}

void loop(){
  switch(IR.code){
      case DIY1: hueRainbow(100); off(); break;
      //case DIY2: waterfall7(35); break;
      case RED: fill(CRGB::Red, 7); break;
      case ORGRED: fill(CRGB::OrangeRed, 7); break;
      case ORANGE: fill(CRGB::Orange, 7); break;
      case PEACHPUFF: fill(CRGB::PeachPuff, 7); break;
      case YELLOW: fill(CRGB::Yellow, 7); break;
      case GREEN: fill(CRGB::Green, 7); break;
      case LIME: fill(CRGB::Lime, 7); break;
      case AQUAMARINE: fill(CRGB::Aquamarine, 7); break;
      case JELLYBEAN: fill(CRGB(69, 129, 142), 7); break;
      case NORDIC: fill(CRGB(12, 52, 61), 7); break;
      case BLUE: fill(CRGB::Blue, 7); break;
      case ROYALBLUE: fill(CRGB:: RoyalBlue, 7); break;
      case MIDNIGHTBLUE: fill(CRGB::MidnightBlue, 7); break;
      case PURPLE: fill(CRGB::Purple, 7); break;
      case PINK: fill(CRGB::Pink, 7); break;
      case WHITE: fill(CRGB::White, 7); break;
      case CYAN: fill(CRGB::Cyan, 7); break;
      case SKYBLUE: fill(CRGB::SkyBlue, 7); break;
      case POWER: off();

      default: break;
    }
}
