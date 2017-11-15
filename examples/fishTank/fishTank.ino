/*
 ************************************************************************
 * 作者:  IntoRobot Team
 * 版本:  V1.0.0
 * 日期:  05-11-16
 ************************************************************************
 功能描述：
检测鱼缸水温，控制其恒温、增氧、水泵、喂食、灯光。

 所需器件:


 接线说明:
 DS18B20传感器               核心板
 1.VCC                       3.3V
 2.GND                       GND
 3.DATA                      D0

 RGB灯条模块
 1.VCC                       +5V
 2.GND                       GND
 3.DATA                      D2

恒温器                       D1
抽水泵                       D7
喂食器                       D3
增氧器                       D5
 */

#include <Dallas_Temperature.h>
#include <OneWire.h>
#include <Adafruit_NeoPixel.h>
#include <WidgetFishTank.h>

#define ONE_WIRE_BUS  D0 //DS18B20 温度传感器
#define RGB_PIN     D2
#define TEMP_PIN    D1 //恒温开关
#define PUMP_PIN    D7 //水泵开关
#define FEED_PIN    D3 //喂食开关
#define O2_PIN      D5 //增氧开关

uint32_t oxyStartTime;
uint8_t incOxyTime;
uint8_t incOxyFlag;


// 设置oneWire线连接
OneWire oneWire(ONE_WIRE_BUS);

// 把oneWire线赋给温度传感器
Dallas_Temperature ds18b20(&oneWire);

Adafruit_NeoPixel strip = Adafruit_NeoPixel(60, RGB_PIN, WS2812B); // RGB灯条

WidgetFishTank fishTank;

//设置灯条亮度
void set_bright(uint8_t level)
{
    static unsigned s_level_old = 0x00;

    if(level != s_level_old)
    {
        strip.setBrightness(level);
        strip.show();
    }

    s_level_old = level;
}

//设置灯条颜色
void set_color(uint32_t c, uint8_t wait)
{
    uint16_t i;

    for (i = 0; i < strip.numPixels(); i++)
    {
        strip.setPixelColor(i, c);
    }

    strip.show();
    delay(wait);
}

//灯条控制回调 {"brightness":1,"rgb":991264}
void neopixelControlCb(void)
{
    int brightnessValue;
    int rgbValue;

    brightnessValue = fishTank.getBrightness();
    set_bright((25*brightnessValue)/10);
    
    rgbValue = fishTank.getRGBValue();
    set_color(rgbValue, 20);
}

void TankCb(void)
{
    if(fishTank.getThermostatSwitch()) //恒温开关
    {
        fishTank.controlThermostat(TEMP_PIN,1);
    } 
    else 
    {
        fishTank.controlThermostat(TEMP_PIN,0);
    }
    
    if(fishTank.getPumpSwitch())//水泵
    {
        fishTank.controlPump(PUMP_PIN,1);
    } 
    else 
    {
        fishTank.controlPump(PUMP_PIN,0);
    }
    
    if(fishTank.getFeedSwitch())//喂食
    {
        fishTank.controlFeed(FEED_PIN,1);
    }
    else
    {
        fishTank.controlFeed(FEED_PIN,0);
    }
    
    
    incOxyTime = fishTank.getOxyTime();//增氧时间
    
    if(fishTank.getOxySwitch()) //增氧开关
    {
        fishTank.controlOxy(O2_PIN,1);
        
        if(incOxyTime != 0)
        {
            incOxyFlag = 1;
            oxyStartTime = timerGetId();
        }
        else
        {
            incOxyFlag = 0;
        }
    } 
    else 
    {
        fishTank.controlOxy(O2_PIN,0);
        incOxyFlag = 0;
    }
    
    neopixelControlCb();
}



void setup() 
{
    oneWire.begin();
    ds18b20.begin();
    
    //灯条初始化
    strip.begin();
    set_bright(255);
    strip.show();

    fishTank.begin(TankCb);
}

void loop() 
{

    ds18b20.requestTemperatures();
    float temp = ds18b20.getTempCByIndex(0);
    
    if(temp != -127.00)
    {
        fishTank.displayTemperature(temp); //上送温度
    }
    
    if(incOxyFlag && (timerIsEnd(oxyStartTime,incOxyTime*60000)))
    {
        fishTank.controlOxy(O2_PIN,0);
        incOxyFlag = 0;
    }
    
    delay(3000);
}
