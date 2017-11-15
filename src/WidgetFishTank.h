#ifndef WIDGET_FISH_TANK_H_
#define WIDGET_FISH_TANK_H_

#include "application.h"



class WidgetFishTank: public WidgetBaseClass
{
    public:
        WidgetFishTank(uint8_t ucItem = 0);
        ~WidgetFishTank();
        void begin(void (*UserCallBack)(void) = NULL);
        void displayTemperature(char temperature);
        void displayOxyStatus(uint8_t status);
        uint8_t getThermostatSwitch(void);
        uint8_t getPumpSwitch(void);
        uint8_t getOxySwitch(void);
        uint8_t getFeedSwitch(void);
        int getOxyTime(void);
        int getBrightness(void);
        uint32_t getRGBValue(void);
        void controlThermostat(uint8_t pin,uint8_t status);
        void controlPump(uint8_t pin,uint8_t status);
        void controlOxy(uint8_t pin,uint8_t status);
        void controlFeed(uint8_t pin,uint8_t status);

    private:
        char pDataTemperatureTopic[64];
        char pCmdTopic[64];
        char pDataOxyStatusTopic[64];

        uint8_t _thermostatSwitchKey = 0;
        uint8_t _pumpSwitchKey = 0;
        uint8_t _oxySwitchKey = 0;
        uint8_t _feedKey;
        uint8_t _Item=0;
        int _oxyTime;
        int _ledBrightnessValue;
        uint32_t _rgbValue;
        void (*_EventCb)(void);
        void widgetBaseCallBack(uint8_t *payload, uint32_t len);

};

#endif
