typedef unsigned char uchar;
typedef unsigned short ushort;
typedef unsigned int uint;

#define PIN_AD A1
#define PIN_CHARGE 5
#define PIN_POWER 10
#define V_CHARGE 665 // 3.3[V]
#define V_DISCHARGE 626 // 3.1[V] = ( 5 * 572 ) / ( 2^10 - 1 )
#define INTERVAL 10 // ms
#define LPF_WIDTH 10 // max 255
#define RES_V 0.00495

ushort lpf( ushort data );

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
  ushort us_ad_avg = 0;

  us_ad = analogRead( PIN_AD );
  us_ad_avg = lpf( us_ad );
//  Serial.println( String(us_ad * RES_V ) + "," + String(us_ad_avg * RES_V ) );
//  Serial.println( String(us_ad ) + "," + String(us_ad_avg ) );
  switch ( suc_state )
  {
  case 0:
    digitalWrite( PIN_CHARGE, HIGH );
    suc_state = 1;
    sui_count = 0;
    break;
  case 1:
    if( V_CHARGE < us_ad_avg ){
      delay(1000);
      suc_state = 2;
    }
    break;
  case 2:
    digitalWrite( PIN_CHARGE, LOW );
    digitalWrite( PIN_POWER, HIGH );
    suc_state = 3;
    break;
  case 3:
    if( V_DISCHARGE > us_ad_avg ){
      digitalWrite( PIN_POWER, LOW );
      suc_state = 4;
      Serial.println( String( us_ad_avg * RES_V ) + "," + String( sui_count / ( 1000.0 / INTERVAL ) ) );
    }
    sui_count++;
    break;
  case 4:
    delay(10000);
    suc_state = 0;
    break;
  
  default:
    while(1);
    break;
  }

  delay( INTERVAL );

}

ushort lpf( ushort data ){
    static ushort buf[ LPF_WIDTH ] = {0};
    static uchar bufp = 0;
    static uint sum = 0;
    static uchar init_flg = 0;
    ushort res;
    int i;

    buf[ bufp ] = data;
    sum += data;
    if( init_flg ){
        sum -= buf[ LPF_WIDTH - bufp - 1 ];
        res = sum / LPF_WIDTH;

    }
    else{
        if( bufp >= LPF_WIDTH - 1 )init_flg = 1;
        res = sum / ( bufp + 1 );
    }

    bufp++;
    if( bufp >= LPF_WIDTH )bufp = 0;

    return res;
}
