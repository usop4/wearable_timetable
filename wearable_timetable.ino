#include <Adafruit_NeoPixel.h>
#include <avr/power.h>

// デバッグ（sp=60ULにすると60倍速になる。通常はsp = 1UL）
unsigned int sp = 60UL;

// set time
// 開始時間を設定

unsigned long start_h = 9UL * 1000UL * 60UL * 60UL;
unsigned long start_m = 2UL * 1000UL * 60UL;
unsigned long t = 0;
unsigned long t_ini = start_h + start_m;

int h, m, s, ms;//hour,minute,second
int delayval = 50;

// NeoPixel

int pin = 3; //GEMMA->1,LilyPad->3
int numpixels = 5;
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(numpixels, pin, NEO_GRB + NEO_KHZ800);

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

// 駅と駅の間の時間差
int station[] = {0,2,4,6,8};

void setup() {

#if defined (__AVR_ATtiny85__)
  if (F_CPU == 16000000) clock_prescale_set(clock_div_1);
#endif

  pixels.begin(); // This initializes the NeoPixel library.

}

void loop() {

  t = millis() * sp + t_ini;

  h = (int)( t / (1000UL * 60UL * 60UL ) % 24UL );
  m = (int)( t / (1000UL * 60UL ) % 60UL );
  s = (int)( millis() / (1000UL ) % 60UL );
  ms = (int)( millis()  % 1000UL );
  //s = (int)( t / (1000UL ) % 60UL );
  //ms = (int)( t  % 1000UL );

  for( int i = 0; i < numpixels; i++ ){
    if( s % 5 == i ){
      // 電車の進行方向を青で表示
      pixels.setPixelColor( i, pixels.Color(0,ms/30,ms/30));
      pixels.show();
    }else{
      pixels.setPixelColor( i, pixels.Color(0,0,0));
      pixels.show();      
      // 電車の近づき具合を赤で表示
      check_table2(i);
    }
  }

  delay(delayval);
}

void check_table2(int number){
  for(int i = 0; i < 9; i++ ){
    // 時刻表をサーチし、最も近づいている電車と駅の距離（時間差）を明るさにする
    int bright = 60*2 - abs((table[h][i]+station[number])*60 - (m*60+s));
    if( bright > 0 ){
      pixels.setPixelColor(number, pixels.Color(curve(bright),0,0));
      pixels.show();
    }
  }
}

int curve(int bright){
  return bright;
}

