typedef unsigned char uchar;
typedef unsigned short ushort;
typedef unsigned int uint;

#define PIN_AD A1
#define PIN_CHARGE 9
#define PIN_POWER 10
//#define V_CHARGE 1020 // 4.98[V] = ( 5 * 1020 ) / ( 2^10 - 1 )
#define V_CHARGE 660 // 3.3[V]
#define V_DISCHARGE 572 // 2.79[V] = ( 5 * 572 ) / ( 2^10 - 1 )
#define INTERVAL 100 // ms

void setup() {
  // put your setup code here, to run once:
  Serial.begin( 115200 );
  digitalWrite( PIN_CHARGE, LOW );
  digitalWrite( PIN_POWER, LOW );
  pinMode( PIN_CHARGE, OUTPUT );
  pinMode( PIN_POWER, OUTPUT );
  pinMode( PIN_AD, INPUT );
}

void loop() {
  // put your main code here, to run repeatedly:
  static uchar suc_state = 0;
  static uint sui_count = 0;
  ushort us_ad = 0;

  switch ( suc_state )
  {
  case 0:
    Serial.println( "chager start" );
    digitalWrite( PIN_CHARGE, HIGH );
    suc_state = 1;
    sui_count = 0;
    Serial.print( "state:" );
    Serial.println( suc_state );
    break;
  case 1:
    us_ad = analogRead( PIN_AD );
    Serial.println( us_ad );
    if( V_CHARGE < us_ad ){
      Serial.println( "charge complate" );
      delay(500);
      suc_state = 2;
      Serial.print( "state:" );
      Serial.println( suc_state );
    }
    break;
  case 2:
    Serial.println( "discharge start" );
    digitalWrite( PIN_CHARGE, LOW );
    digitalWrite( PIN_POWER, HIGH );
    suc_state = 3;
    Serial.print( "state:" );
    Serial.println( suc_state );
    break;
  case 3:
    us_ad = analogRead( PIN_AD );
    Serial.println( us_ad * 0.005 );
    if( V_CHARGE > us_ad ){
      Serial.println( "discharge complate" );
      digitalWrite( PIN_POWER, LOW );
      suc_state = 4;
      Serial.print( "state:" );
      Serial.println( suc_state );
    }
    sui_count++;
    break;
  case 4:
    Serial.println("end:" + String( sui_count / ( 1000.0 / INTERVAL ) ));
    delay(10000);
    suc_state = 0;
    break;
  
  default:
    while(1);
    break;
  }

//  delayMicroseconds( INTERVAL );
  delay( INTERVAL );

//    Serial.println("suc_state:" + String(us_ad_tmp));
//    Serial.println("ad:" + String(us_ad_tmp));

/*
  switch ( uc_suc_state )
  {
  case 0:
    Serial.println("suc_state:" + String(us_ad_tmp));
    Serial.println("ad:" + String(us_ad_tmp));
    break;
  
  default:
    break;
  }

  delay(1000);
*/
//  digitalWrite(PIN_POWER, HIGH);   // turn the LED on (HIGH is the voltage level)
//  delay(30);                       // wait for a second
}
