#include <Adafruit_NeoPixel.h>
#include <avr/power.h>

// set time

unsigned long start_hour = 9UL * 1000UL * 60UL * 60UL;
unsigned long start_min = 9UL * 1000UL * 60UL;

// NeoPixel

int pin = 3; //GEMMA->1,Uno->3
int numpixels = 3;
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(numpixels, pin, NEO_GRB + NEO_KHZ800);

// GEMMAに接続するときは0にする
int serial = 1;

// デバッグ（sp=60ULにすると60倍速になる）
int debug = 1;
unsigned int sp = 60UL;

// time table
// http://www.twr.co.jp/route/kokusai/timetable.html
// ToDo:正しくは２駅前の東雲を入力。後で直す

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

// その他（設定不要）

unsigned long t = 0;
unsigned long t_ini = start_hour + start_min;
unsigned long t_old = 0;

int h = 0;
int m = 0;
int s = 0;
int m_old = 0;
int s_old = 0;

int delayval = 300;

void setup() {

#if defined (__AVR_ATtiny85__)
  if (F_CPU == 16000000) clock_prescale_set(clock_div_1);
#endif

  pixels.begin(); // This initializes the NeoPixel library.

  pixels.setPixelColor(0, pixels.Color(0,0,0));
  pixels.show();
  pixels.setPixelColor(1, pixels.Color(0,0,0));
  pixels.show();
  pixels.setPixelColor(2, pixels.Color(0,0,0));
  pixels.show();

  if( serial == 1 ){
    Serial.begin(9600);    
  }

}

void loop() {

  pixels.setPixelColor(0, pixels.Color(0,0,0));
  pixels.show();
  pixels.setPixelColor(1, pixels.Color(0,0,0));
  pixels.show();
  pixels.setPixelColor(2, pixels.Color(0,0,0));
  pixels.show();
  
  t = millis() + t_ini;

  h = (int)( t / (1000UL * 60UL * 60UL ) % 24UL );
  m = (int)( t / (1000UL * 60UL /sp) % 60UL );
  s = (int)( t / (1000UL /sp) % 60UL );
  
  if( m != m_old ){
    if( serial == 1 ){
      Serial.println();      
      Serial.print(h);
      Serial.print(":");
      Serial.print(m);
      Serial.print(":");
      Serial.print(s);
    }
    m_old = m;
    s_old = s;
  }
  
  check_table2(h,m,0,0);//当駅
  check_table2(h,m,1,2);//＋１駅（２分後）
  check_table2(h,m,2,4);//＋２駅（４分後）

  delay(delayval);
}

void check_table2(int h, int m, int p, int d){
  for(int i = 0; i < 9; i++ ){
    if( table[h][i]+d == m ){
      pixels.setPixelColor(p, pixels.Color(100,100,100));
      pixels.show();
      if( serial == 1 ){
        Serial.print("[");        
        Serial.print(p);
        Serial.print("]");        
      }
    }
  }
}

