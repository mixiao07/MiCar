/**
 * MiCar Gamepad NRF24L01 Pin assign
 *
 * Pins:
 * Hardware SPI:
 * MISO -> 12
 * MOSI -> 11
 * SCK -> 13
 *
 * Configurable:
 * CE -> 8
 * CSN -> 7
 *
 * By -- MX
 */

#include <SPI.h>
#include <Mirf.h>
#include <nRF24L01.h>
#include <MirfHardwareSpiDriver.h>

#define ENBALE_DEBUG      (0)

void nrf24_gp_init(void);
void nrf24_gp_send(int x, int y);

unsigned long idle_cnt;

void setup() {
  Serial.begin(9600);
  nrf24_gp_init();
  idle_cnt = 0;
  Serial.println("MX Gamepad Go...");
}

void loop() {
  int x, y;
  int time;

  /* Get Gamepad X & Y value */
  x = analogRead(A1);
  y = analogRead(A0);
  nrf24_gp_send(x, y); 
  delay(70);

  /* check if idle */
  if (((x > 490) && (x < 600)) && ((y > 490) && (y < 600))) {
    if (idle_cnt >= (unsigned long)((unsigned long)2 * 60 * 1000/100)) {
#if ENBALE_DEBUG
      Serial.println(idle_cnt);  
#endif
      delay(2000); 
    }
    else {
      idle_cnt++;  
    }
  }
  else {
    idle_cnt=0;
  }

#if ENBALE_DEBUG
  Serial.println(idle_cnt);  
#endif
}


void nrf24_gp_init(void)
{
  /*
   * Setup pins / SPI.
   */
   
  /* To change CE / CSN Pins:
   * 
   * Mirf.csnPin = 9;
   * Mirf.cePin = 7;
   */
  /*
  Mirf.cePin = 7;
  Mirf.csnPin = 8;
  */
  Mirf.spi = &MirfHardwareSpi;
  Mirf.init();
  
  /*
   * Configure reciving address.
   */
   
  Mirf.setRADDR((byte *)"mxgp0");
  
  /*
   * Set the payload length to sizeof(unsigned long) the
   * return type of millis().
   *
   * NB: payload on client and server must be the same.
   */
   
  Mirf.payload = 4;
  
  /*
   * Write channel and payload config then power up reciver.
   */
   
  /*
   * To change channel:
   * 
   * Mirf.channel = 10;
   *
   * NB: Make sure channel is legal in your area.
   */
   
  Mirf.config();   
}

void nrf24_gp_send(int x, int y)
{
  byte data[4];

  data[0] = (byte)(x>>8);;
  data[1] = (byte)x;
  data[2] = (byte)(y>>8);
  data[3] = (byte)y;
  
  Mirf.setTADDR((byte *)"mxcar");
  Mirf.send((byte *)data);
  delay(20);
  while(Mirf.isSending()){
    delay(1);
  }
  //Serial.println("Finished sending");
}

