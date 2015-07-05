#include <Adafruit_NeoPixel.h>
#include <avr/power.h>

// set time
// 開始時間を設定

unsigned long start_hour = 9UL * 1000UL * 60UL * 60UL;
unsigned long start_min = 9UL * 1000UL * 60UL;
unsigned long t = 0;
unsigned long t_ini = start_hour + start_min;
unsigned long t_old = 0;

int delayval = 30;

// NeoPixel

int pin = 3; //GEMMA->1,LilyPad->3
int numpixels = 5;
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(numpixels, pin, NEO_GRB + NEO_KHZ800);

// デバッグ（sp=60ULにすると60倍速になる。通常はsp = 1UL）
unsigned int sp = 10UL;

// time table
// http://www.twr.co.jp/route/kokusai/timetable.html
// ToDo:正しくは２駅前の新木場を入力。後で直す

int table[28][9] = {
  {},//0
  {},//1
  {},//2
  {},//3
  {},//4
  {},//5
  {},//6
  {},//7
  {},//8
  {2,9,18,24,31,36,42,47,57},//9
  {4,11,16,25,34,44,51,58},//10
  {9,19,30,36,44,51,58},//11
  {9,19,30,36,44,51,58},//12
  {9,19,30,36,44,51,58},//13
  {9,19,30,36,44,51,58},//14
  {9,19,30,36,44,51,58},//15
  {9,19,30,36,44,51,58},//16
  {9,19,30,36,44,51,58},//17
  {9,19,30,36,44,51,58}//18
  };


void setup() {

#if defined (__AVR_ATtiny85__)
  if (F_CPU == 16000000) clock_prescale_set(clock_div_1);
#endif

  pixels.begin(); // This initializes the NeoPixel library.

}

void loop() {

  t = millis() + t_ini;

  int h = (int)( t / (1000UL * 60UL * 60UL ) % 24UL );
  int m = (int)( t / (1000UL * 60UL /sp) % 60UL );
  int s = (int)( t / (1000UL /sp) % 60UL );

  for( int i = 0; i < numpixels; i++ ){
    if( s % 5 == i ){
      pixels.setPixelColor( i, pixels.Color(0,0,5));
      pixels.show();
    }else{
      pixels.setPixelColor( i, pixels.Color(0,0,0));
      pixels.show();      
    }
  }
  
  check_table2(h,m,0,0);//当駅
  check_table2(h,m,1,2);//＋１駅（２分後）
  check_table2(h,m,2,4);//＋２駅（４分後）
  check_table2(h,m,3,6);//＋３駅（６分後）
  check_table2(h,m,4,8);//＋４駅（８分後）

  delay(delayval);
}

void check_table2(int h, int m, int p, int d){
  for(int i = 0; i < 9; i++ ){
    if( table[h][i]+d == m ){
      pixels.setPixelColor(p, pixels.Color(10,0,0));
      pixels.show();
    }
  }
}

