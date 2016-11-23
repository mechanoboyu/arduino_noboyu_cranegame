/*
 Arduinoでクレーンゲーム
 
   * コイン（10円）投入されたら、ゲーム開始。照明と音楽スタート。
   * S2,s3それぞれ押してる間だけ、モーターが回転する。
   * lim1～4にギアボックスが当たって押下されるとモーターはストップ。
   
   
*/

#define PBS1 8				// コインSW
#define PBS2 9				// 操作SW1
#define PBS3 10				// 操作SW2
#define lim1 14				// X軸リミットSW1
#define lim2 15				// X軸リミットSW2
#define lim3 16				// Y軸リミットSW1
#define lim4 17				// Y軸リミットSW2
#define LED 12				// 照明制御
#define MP3 11				// 音楽制御
int s1=0,s2=0,s3=0,s4=0,s5=0,s6=0,s7=0,s8=0,s9=0,i=0;
int old_s2=1;				// 操作SW1の状態を記録
int old_s5=1;				// 操作SW2の状態を記録
int state_x=0;				// 操作SW1が押下済かを記録
int state_y=0;				// 操作SW2が押下済かを記録

void setup() 
{
  pinMode(PBS1,INPUT);		// PBS1(ピン8)を入力に設定
  pinMode(PBS2,INPUT);		// PBS2(ピン9)を入力に設定
  pinMode(PBS3,INPUT);		// PBS3(ピン10)を入力に設定
  pinMode(lim1,INPUT);		// lim1(ピン14)を入力に設定
  pinMode(lim2,INPUT);		// lim1(ピン15)を入力に設定
  pinMode(lim3,INPUT);		// lim1(ピン16)を入力に設定
  pinMode(lim4,INPUT);		// lim1(ピン17)を入力に設定
  pinMode(MP3,OUTPUT);		// 音楽制御用
  pinMode(LED,OUTPUT);		// LED用
  DDRD=B11111100;			// モーター制御用
}

void loop()
{
 digitalWrite(LED, 1);		// 照明消灯
 digitalWrite(MP3, 0);		// 音楽オフ
 s1=digitalRead(PBS1);
 if(s1==0)					// コインが投入されるまで待つ
 {
  digitalWrite(LED, 0);		// 照明点灯
  digitalWrite(MP3, 1);		// 音楽スタート
  delay(5);
  while(1)
  {
   s2=digitalRead(PBS2);
   if(s2==0)				// PBS2が押されている間は、
   {
	delay(50);
    PORTD=0x4;				//Xモーター正転
    old_s2=s2;				//SW状態(押下した事)を記録
    s4=digitalRead(lim2);	
    if(s4==0)				//limitSW2がONなら、
    {
     PORTD=0x3c;			//ストップして脱出
     break;
    }
   }
   else						//SWが押下されていないなら、
   {
   PORTD=0x3c;				//モーターストップ
   s2=digitalRead(PBS2);
    if(s2==1 && old_s2==0)	//SWを既に押しているならば、
    {
     state_x=1-state_x;		// SW押下済である事を記録
    }
   }
   if(state_x==1)			// SW押下済ならば脱出
   {
    break;
   }
  }
  while(1)
  {
   s5=digitalRead(PBS3);
   if(s5==0)				// PBS3が押される間は、、
   {
	delay(50);
    PORTD=0x20;				//Yモーター正転
    old_s5=s5;				//SW状態(押下した事)を記録
    s7=digitalRead(lim4);
    if(s7==0)				//limitSW4がONなら、
    {
     PORTD=0x3c;			//ストップして脱出
     break;
    }
   }
   else						//SWが押下されていないなら、
   {
    PORTD=0x3c;				//モーターストップ
    s5=digitalRead(PBS3);
    if(s5==1 && old_s5==0)	//SWを既に押しているならば、
    {
     state_y=1-state_y;		// SW押下済である事を記録
    }
   }
   if(state_y==1)			// sw押下済ならば脱出
   {
    break;
   }
  }
  delay(500);				//limitSWで脱出したらチャタリングでbreakする対策
  PORTD=0x80;				//Zモーター反転、下降
  delay(500);
  PORTD=0x3c;
  delay(50);
  PORTD=0x40;				//Zモーター正転、上昇
  delay(500);
  PORTD=0x3c;
  delay(50);
 //原点へ
   while(1)
   {
	PORTD=0x10;				//Yモーター反転
    s6=digitalRead(lim3);
     if(s6==0)				//limit3がONになるまで回転、
     {
      PORTD=0x3c;			//ストップして脱出
      break;
     }
   }
   delay(5);
   while(1)
   {
	PORTD=0x8;				//Xモーター反転
    s8=digitalRead(lim1);
     if(s8==0)				//limitSW1がONになるまで回転、
     {
 	  delay(5);
      PORTD=0x3c;			//ストップして脱出
      break;
     }
   }
  delay(500);				//直前でlimitSWで脱出したらチャタリングでbreakする対策
  PORTD=0x80;				//Zモーター反転、下降
  delay(500);
  PORTD=0x3c;
  delay(50);
  PORTD=0x40;				//Zモーター正転、上昇
  delay(500);
  PORTD=0x3c;
  delay(50);
  digitalWrite(LED, 1);		// 照明消灯
  digitalWrite(MP3, 0);		// 音楽オフ
  
  old_s2=1;					// 操作SW1の状態を初期値に戻す
  old_s5=1; 				// 操作SW2の状態を初期値に戻す
  state_x=0;				// 操作SW1の押下記録を初期値に戻す
  state_y=0;				// 操作SW2の押下記録を初期値に戻す
 }
}
