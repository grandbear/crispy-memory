#include<WiFi.h>
#include <SPI.h>
#include <TFT_eSPI.h>
#include "basic.h"
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <RTClib.h>
#include <ESP32Time.h>

TFT_eSPI tft;       //实例化
ESP32Time rtc(0);   //时间补偿

//编写函数
  void led_init(void)
  {
    for(int i=0;i<5;i++)
    {
      pinMode(led_pin[i],OUTPUT);
      digitalWrite(led_pin[i], HIGH);
    }
  }

  void led_mode(int mode)          //共阳极
  {
    if(mode==1)
    {
      for(int i=0;i<5;i++)
      {
        digitalWrite(led_pin[i], LOW);
        for(int j=0;j<5&&j!=i;j++)
        {
          digitalWrite(led_pin[j], HIGH);   
        }
        delay(200);
      }
      
      for(int i=0;i<5;i++)
      {
        digitalWrite(led_pin[i], HIGH);   
      }
      
    }
    // else if(mode==2)
    // {
    // digitalWrite(LED1, HIGH);
    // digitalWrite(LED2, HIGH);
    // digitalWrite(LED3, HIGH);
    // digitalWrite(LED4, HIGH);
    // digitalWrite(LED5, HIGH);
    // }
   
    
  }




String city;
String weather_condition;
String winddirection;
int tem;

void attain_weather(String url)
{
   HTTPClient http;
   if (WiFi.status() == WL_CONNECTED)
    {
      // 设置目标网站的URL
      http.begin(url);

      // 发起GET请求并等待响应
      int httpCode = http.GET();
      
        if (httpCode > 0) 
        {
          if (httpCode == HTTP_CODE_OK)
          {
            // 获取响应的内容
            String response = http.getString();
            DynamicJsonDocument doc(1024);
            DeserializationError error = deserializeJson(doc, response);

            city = doc["lives"][0]["city"].as<String>();
            weather_condition = doc["lives"][0]["weather"].as<String>();
            tem = doc["lives"][0]["temperature"];
            winddirection = doc["lives"][0]["winddirection"].as<String>();
          }
        }
          
    }
}

String date_information;
String date;
int hour=0,minute=0,second=0;
int year=2023,month=5,day=2;

void attain_time(String url)
{
HTTPClient http;
   if (WiFi.status() == WL_CONNECTED)
    {
      // 设置目标网站的URL
      http.begin(url);

      // 发起GET请求并等待响应
      int httpCode = http.GET();
      
        if (httpCode > 0) 
        {
          if (httpCode == HTTP_CODE_OK)
          {
            // 获取响应的内容
            String response = http.getString();
            DynamicJsonDocument doc(1024);
            DeserializationError error = deserializeJson(doc, response);

            date_information = doc["datetime"].as<String>();
            date = date_information.substring(0,10);

            year = date_information.substring(0,4).toInt();
            month = date_information.substring(5,7).toInt();
            day = date_information.substring(8,10).toInt();
            hour = date_information.substring(11,13).toInt();
            minute = date_information.substring(14,16).toInt();
            second = date_information.substring(17,19).toInt();
            
          }
        }
          
    }
}

//动画帧的索引
static int frameIndex = 0;
//动画的宽度
#define animationWidth 40
//动画的高度
#define animationHeight 40
//局部刷新的参数，都是”像素/8“的值
int x = 11;
int y = 3;
int w = animationWidth / 8;
int h = animationHeight / 8;

//定义太空人动画显示所需要的帧延时变量
unsigned long previousTime = 0;  // 上一次执行任务的时间
unsigned long delayTime = 100;  // 延时时间（毫秒）
void astronaut_show(void)
{
  unsigned long currentTime = millis();  // 获取当前时间
  if (currentTime - previousTime >= delayTime) 
  {
    tft.drawXBitmap(120, 88, frameData[frameIndex], 40, 40, TFT_BLACK, TFT_WHITE);
    frameIndex++;
    // tft.setAddrWindow(x, y, 40, 40);

    //播放到最后一帧之后，又回到第一帧开始播放
    if (frameIndex >= FRAME_NUM) 
    {
      frameIndex = 0;
    }

    previousTime = currentTime;  // 更新上一次执行任务的时间
  }
  
}

//定义时间显示延时的相关变量
unsigned long previousTime2 = 0;  // 上一次执行任务的时间
unsigned long delayTime2 = 500;  // 延时时间（毫秒）    间隔设置小于1s，在分钟更新时连续刷新，防止刷新失败
unsigned long currentTime2;
byte xcolon = 0;
uint8_t initial_flag=0,time_refresh_flag=0;
void time_show(uint8_t hour,uint8_t minute,uint8_t second)
{
  uint8_t hh=hour,mm=minute,ss=second;
  // tft.setTextColor(TFT_BROWN);                                        //第一种显示
  // if (millis() - previousTime2 >= delayTime2) 
  //   {
  //     tft.fillRect(0, 26, 160, 59, TFT_WHITE);
  //     previousTime2 = millis();  //        更新上一次的时间
  //     tft.setCursor(6,30,7);
  //     tft.print(rtc.getTime("%H:%M"));
      
  //   }        


    if((ss==0)||(initial_flag==0))                                       //第二种显示
    {
      time_refresh_flag=1;
    }
    else
    {
      time_refresh_flag=0;
    }
    byte xpos = 0;
    byte ypos = 32;
    if(hh>=20) xpos=8;         //调整时间显示位置
    else xpos =0;
    // time_refresh_flag==1
    if (time_refresh_flag==1) 
    { 
      if (millis() - previousTime2 >= delayTime2) 
      {
        tft.fillRect(0, 26, 160, 59, TFT_WHITE);
        previousTime2 = millis();  //        更新上一次的时间
      }
      tft.setTextColor(TFT_BLACK); // Orange
      if (hh<10) xpos+= tft.drawChar('0',xpos,ypos,7);
      xpos+= tft.drawNumber(hh,xpos,ypos,7);
      xcolon=xpos;
      xpos+= tft.drawChar(':',xpos,ypos,7);
      if (mm<10) xpos+= tft.drawChar('0',xpos,ypos,7);
      tft.drawNumber(mm,xpos,ypos,7);
      initial_flag=1;
    }
    if (ss%2)  // Flash the colon
    {
      tft.setTextColor(TFT_WHITE, TFT_WHITE);
      xpos+= tft.drawChar(':',xcolon,ypos,7);
      tft.setTextColor(TFT_BLACK, TFT_WHITE);
    }
    else
    {
      tft.drawChar(':',xcolon,ypos,7);
    }
}




