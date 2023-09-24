#include <LovyanGFX.hpp>
#include <lgfx_user/LGFX_ESP32S3_SenseCAPindicator.hpp>

#define Xmin  768
#define Xmax  256
#define Ymin  200
#define Ymax  460

//LGFX_Sprite face_normal(&lcd);

static LGFX lcd;
int16_t x=240;
int16_t y=240;
int16_t map_x, map_y;

void face_change(){
  int s;
  s=random(0,6);
  switch(s){
    case 0:
      lcd.fillScreen(TFT_BLACK);
      face_normal();
      break;
    case 1:
      lcd.fillScreen(TFT_BLACK);
      face_smile();
      break;
    case 2:
      lcd.fillScreen(TFT_BLACK);
      face_sad1();
      break;
    case 3:
      lcd.fillScreen(TFT_BLACK);
      face_sad2();
      break;
    case 4:
      lcd.fillScreen(TFT_BLACK);
      face_angry();
      break;
    case 5:
      lcd.fillScreen(TFT_BLACK);
      face_surprise();
      break;
  }
}

void face_normal(){
  lcd.fillCircle(120,220,10,TFT_WHITE);
  lcd.fillCircle(360,220,10,TFT_WHITE);
  lcd.fillRect(170,310,150,5,TFT_WHITE);
}

void face_smile(){
  lcd.fillRect(170,310,150,5,TFT_WHITE);
  lcd.drawBezier( 110, 220, 120, 210, 130, 220, TFT_WHITE);
  lcd.drawBezier( 350, 220, 360, 210, 370, 220, TFT_WHITE);
}

void face_sad1(){
  lcd.drawBezier( 220, 320, 240, 310, 260, 320, TFT_WHITE); //口
  lcd.drawBezier( 110, 220, 120, 230, 130, 220, TFT_WHITE); //左目
  lcd.drawBezier( 350, 220, 360, 230, 370, 220, TFT_WHITE); //右目
}

void face_sad2(){
  lcd.drawBezier( 220, 320, 240, 310, 260, 320, TFT_WHITE); //口
  lcd.fillCircle(120,220,10,TFT_WHITE);
  lcd.fillCircle(360,220,10,TFT_WHITE);
}

void face_angry(){
  lcd.drawLine(100,190,140,200,TFT_WHITE);
  lcd.fillCircle(120,220,10,TFT_WHITE);
  lcd.drawLine(380,190,340,200,TFT_WHITE);
  lcd.fillCircle(360,220,10,TFT_WHITE);
  lcd.drawBezier( 220, 320, 240, 310, 260, 320, TFT_WHITE); //口
}

void face_surprise(){
  lcd.fillCircle(120,220,10,TFT_WHITE);
  lcd.fillCircle(360,220,10,TFT_WHITE);
  lcd.drawEllipse(240,310,20,30,TFT_WHITE);
}

void moveRandom() {
  for (;;) {
    // ランダムモード
    map_x = random(Xmax, Xmin);
    map_y = random(Ymin, Ymax);
    
    if(lcd.getTouch(&x, &y)){      
      if ( (400<=x && x<=480) && (400<=y && y<=480)) {
        lcd.fillRect(300,440,200,50,TFT_BLACK);
        lcd.setCursor(0,440);
        lcd.println("Manual");
        break;
      }
    }   
    int delay_time = random(10);
    face_change();
    Serial1.printf("%u,%u\n",map_x,map_y);
    delay(4000 + 500 * delay_time);
  }
}

void setup(void)
{
  Serial.begin(9600);
  Serial1.begin(115200,SERIAL_8N1,20,19);
  lcd.init();
  lcd.setRotation(0);
  lcd.fillScreen(TFT_BLACK);
  lcd.setTextSize(2.5,2.5);
  lcd.setTextColor(TFT_WHITE,TFT_BLACK);
  face_normal();
  //face_smile();
  //face_sad();
  //face_angry();
  map_x = map(x, 0, 480, Xmin, Xmax);
  //map_y = map(y, 0, 480, Ymin, Ymax);
  map_y = 450;
  Serial1.printf("%u,%u\n",map_x,map_y);
  delay(5000);
  moveRandom() ;
}

void loop(void)
{
  if (lcd.getTouch(&x, &y)) {
    if ( (0<=x && x<=80) && (400<=y && y<=480)) {
      Serial.printf("Random mode\n");
      lcd.fillRect(0,440,200,50,TFT_BLACK);
      lcd.setCursor(380,440);
      lcd.println("Random");
      moveRandom() ;
    }
    map_x = map(x, 0, 480, Xmin, Xmax);
    map_y = map(y, 0, 480, Ymin, Ymax);
    Serial1.printf("%u,%u\n",map_x,map_y);
  }
}
