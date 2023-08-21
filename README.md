# crispy-memory
  使用Arduino基于ESP32-WROOM32开发板的太空人天气时钟，可联网显示对应城市的天气以及时间。
  
  b站效果演示：https://www.bilibili.com/video/BV1TN41187Qj/

  代码中获取天气的部分使用了城市编码，想要获取对应的城市天气只需要将天气api中的城市编码对应替换即可。
  TFT接线方式：  GND-GND  
                VCC-VCC  
                SCL-GPIO18  
                SDA-GPIO23  
                RES-GPIO26  
                DC-GPIO25  
                CS-GPIO27  
                BL-VCC  
              
