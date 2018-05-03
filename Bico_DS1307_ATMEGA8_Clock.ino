#include <Wire.h>
#include "DS1307.h"
#include "Bico_Led7Seg.h"

#define PRESS_BUTTON_0 ((analogRead(A3) >= 750) && (analogRead(A3) <= 850)) //793
#define PRESS_BUTTON_1 ((analogRead(A3) >= 550) && (analogRead(A3) <= 650)) //606
#define PRESS_BUTTON_2 ((analogRead(A3) >= 400) && (analogRead(A3) <= 500)) //448
#define PRESS_BUTTON_3 ((analogRead(A3) >= 250) && (analogRead(A3) <= 350)) //300
#define PRESS_BUTTON_4 ((analogRead(A3) >= 100) && (analogRead(A3) <= 200)) //155
#define PRESS_NO_BUTTON (analogRead(A3) <= 50) //0

#define INDICATE_TIME (indicate_time_date == 0)
#define INDICATE_DATE (indicate_time_date == 1)

DS1307 clock;
RTCDateTime dt;

Bico_Led7Seg my_leg7seg(COMMON_ANODE, 8, 2000, 1);
uint8_t digit[8] = {7, 6, 5, 4, 3, 2, 1, 0};
uint8_t code[8] = {A2, A1, A0, 13, 12, 11, 10, 9};

uint8_t point_position = 0; //use to indicate the number which need to be change
uint8_t indicate_time_date = 0; //0: indicate time, 1: indicate date

void setup() {
  // put your setup code here, to run once:
  clock.begin();
  if(PRESS_BUTTON_4)
  {
    clock.setDateTime(__DATE__, __TIME__);
  }
  my_leg7seg.setup(digit, code);
}

void loop() {
  // put your main code here, to run repeatedly:
  dt = clock.getDateTime();

  if(PRESS_NO_BUTTON)
  {
    if(INDICATE_TIME)
    {
      my_leg7seg.setDigit(7, dt.hour/10, false);
      my_leg7seg.setDigit(6, dt.hour%10, (point_position==3));
    
      my_leg7seg.setSymbol(5, '-');
      
      my_leg7seg.setDigit(4, dt.minute/10, false);
      my_leg7seg.setDigit(3, dt.minute%10, (point_position==2));
      
      my_leg7seg.setSymbol(2, '-');
      
      my_leg7seg.setDigit(1, dt.second/10, false);
      my_leg7seg.setDigit(0, dt.second%10, (point_position==1));
    }


    if(INDICATE_DATE)
    {
      my_leg7seg.setDigit(0, (dt.year-2000)%10, (point_position==1));
      my_leg7seg.setDigit(1, (dt.year-2000)/10, false);
    
      my_leg7seg.setSymbol(2, '-');
      
      my_leg7seg.setDigit(3, dt.month%10, (point_position==2));
      my_leg7seg.setDigit(4, dt.month/10, false);
      
      my_leg7seg.setSymbol(5, '-');
      
      my_leg7seg.setDigit(6, dt.day%10, (point_position==3));
      my_leg7seg.setDigit(7, dt.day/10, false);
    }
  }

  if(PRESS_BUTTON_3)
  {
    indicate_time_date = (indicate_time_date+1)%2;
    delay(200);
  }

  if(PRESS_BUTTON_2)
  {
    point_position = (point_position+1)%4;
    delay(200);
  }

  if(PRESS_BUTTON_1)
  {
    switch(point_position)
    {
      case 1:
      {
        if(INDICATE_TIME)
        {
          clock.setDateTime(dt.year, dt.month, dt.day, dt.hour, dt.minute, (dt.second < 59)? (dt.second + 1):0);
        }
        if(INDICATE_DATE)
        {
          clock.setDateTime(((dt.year - 2000) < 99)? (dt.year + 1):2000, dt.month, dt.day, dt.hour, dt.minute, dt.second);
        }
        break;
      }

      case 2:
      {
        if(INDICATE_TIME)
        {
          clock.setDateTime(dt.year, dt.month, dt.day, dt.hour, (dt.minute < 59)? (dt.minute + 1):0, dt.second);
        }
        if(INDICATE_DATE)
        {
          clock.setDateTime(dt.year, (dt.month < 12)? (dt.month + 1):1, dt.day, dt.hour, dt.minute, dt.second);
        }
        break;
      }

      case 3:
      {
        if(INDICATE_TIME)
        {
          clock.setDateTime(dt.year, dt.month, dt.day, (dt.hour < 23)? (dt.hour + 1):0, dt.minute, dt.second);
        }
        if(INDICATE_DATE)
        {
          clock.setDateTime(dt.year, dt.month, (dt.day < 31)? (dt.day + 1):1, dt.hour, dt.minute, dt.second);
        }
        break;
      }
    }
    delay(200);
  }

  if(PRESS_BUTTON_0)
  {
    switch(point_position)
    {
      case 1:
      {
        if(INDICATE_TIME)
        {
          clock.setDateTime(dt.year, dt.month, dt.day, dt.hour, dt.minute, (dt.second > 0)? (dt.second - 1):58);
        }
        if(INDICATE_DATE)
        {
          clock.setDateTime(((dt.year - 2000) > 0)? (dt.year - 1):2000, dt.month, dt.day, dt.hour, dt.minute, dt.second);
        }
        break;
      }

      case 2:
      {
        if(INDICATE_TIME)
        {
          clock.setDateTime(dt.year, dt.month, dt.day, dt.hour,(dt.minute > 0)? (dt.minute - 1):59, dt.second);
        }
        if(INDICATE_DATE)
        {
          clock.setDateTime(dt.year, (dt.month > 1)? (dt.month - 1):12, dt.day, dt.hour, dt.minute, dt.second);
        }
        break;
      }

      case 3:
      {
        if(INDICATE_TIME)
        {
          clock.setDateTime(dt.year, dt.month, dt.day, (dt.hour > 0)? (dt.hour - 1):23, dt.minute, dt.second);
        }
        if(INDICATE_DATE)
        {
          clock.setDateTime(dt.year, dt.month, (dt.day > 1)? (dt.day - 1):31, dt.hour, dt.minute, dt.second);
        }
        break;
      }
    }
    delay(200);
  }

  my_leg7seg.show();  
}
