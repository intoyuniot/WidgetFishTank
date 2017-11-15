#include "WidgetFishTank.h"

WidgetFishTank::WidgetFishTank(uint8_t ucItem)
{
	_Item=ucItem;

	memset(pDataTemperatureTopic,0,sizeof(pDataTemperatureTopic));
	memset(pCmdTopic,0,sizeof(pCmdTopic));
	memset(pDataOxyStatusTopic,0,sizeof(pDataOxyStatusTopic));
	sprintf(pDataTemperatureTopic,"channel/fishTank_%d/data/temperature",_Item);
	sprintf(pCmdTopic,"channel/fishTank_%d/cmd/control",_Item);
	sprintf(pDataOxyStatusTopic,"channel/fishTank_%d/data/oxyStatus",_Item);
}

WidgetFishTank::~WidgetFishTank()
{
}

void WidgetFishTank::begin(void (*UserCallBack)(void))
{
	_EventCb=UserCallBack;
	IntoRobot.subscribe(pCmdTopic, NULL, this);
}

void WidgetFishTank::displayTemperature(char temperature)
{
	IntoRobot.publish(pDataTemperatureTopic,temperature);
}

void WidgetFishTank::displayOxyStatus(uint8_t status)
{
	IntoRobot.publish(pDataOxyStatusTopic,status);
}

uint8_t WidgetFishTank::getThermostatSwitch(void)
{
	return _thermostatSwitchKey;
}

uint8_t WidgetFishTank::getPumpSwitch(void)
{
	return _pumpSwitchKey;
}

uint8_t WidgetFishTank::getOxySwitch(void)
{
	return _oxySwitchKey;
}

uint8_t WidgetFishTank::getFeedSwitch(void)
{
    return _feedKey;
}

int WidgetFishTank::getOxyTime(void)
{
	return _oxyTime;
}

int WidgetFishTank::getBrightness(void)
{
	return _ledBrightnessValue;
}

uint32_t WidgetFishTank::getRGBValue(void)
{
	return _rgbValue;
}

void WidgetFishTank::controlThermostat(uint8_t pin,uint8_t status)
{
    pinMode(pin,OUTPUT);
	digitalWrite(pin,status);
}

void WidgetFishTank::controlPump(uint8_t pin,uint8_t status)
{
    pinMode(pin,OUTPUT);
	digitalWrite(pin,status);
}

void WidgetFishTank::controlOxy(uint8_t pin,uint8_t status)
{
    pinMode(pin,OUTPUT);
	digitalWrite(pin,status);
	displayOxyStatus(status);
	_oxySwitchKey = 0;
}

void WidgetFishTank::controlFeed(uint8_t pin,uint8_t status)
{
    pinMode(pin,OUTPUT);
    digitalWrite(pin,status);
}


void WidgetFishTank::widgetBaseCallBack(uint8_t *payload, uint32_t len)
{
	aJsonClass aJson;

	aJsonObject *root = aJson.parse((char *)payload);
	if(root == NULL)
	{
	    aJson.deleteItem(root);
		return;
	}
	
	aJsonObject *thermostat = aJson.getObjectItem(root, "thermostat");

	if(thermostat != NULL)
	{
		_thermostatSwitchKey = atoi(thermostat->valuestring);

		aJsonObject *pumpSwitch = aJson.getObjectItem(root, "pump");
		if(pumpSwitch ==NULL)
		{
		    aJson.deleteItem(root);
			return;
		}
		_pumpSwitchKey = atoi(pumpSwitch->valuestring);

		aJsonObject *ledLight = aJson.getObjectItem(root, "light");
		if(ledLight == NULL)
		{
		    aJson.deleteItem(root);
			return;
		}


		aJsonObject *ledBrightness = aJson.getObjectItem(ledLight,"brightness");
		if(ledBrightness == NULL)
		{
		    aJson.deleteItem(root);
			return;
		}
		_ledBrightnessValue = ledBrightness->valueint;

		aJsonObject *ledRgb = aJson.getObjectItem(ledLight,"rgb");
		if(ledRgb == NULL)
		{
		    aJson.deleteItem(root);
			return;
		}
		_rgbValue = ledRgb->valueint;

	}
	else
	{
	    aJsonObject *feed = aJson.getObjectItem(root, "feed");
	    
	    if(feed != NULL)
	    {
	        _feedKey = atoi(feed->valuestring);
	    }
	    else
	    {
    		aJsonObject *oxy = aJson.getObjectItem(root, "oxy");
    		if(oxy == NULL)
    		{
    		    aJson.deleteItem(root);
    			return;
    		}
    
    
    		aJsonObject *oxystatus = aJson.getObjectItem(oxy,"status");
    		if(oxystatus == NULL)
    		{
    		    aJson.deleteItem(root);
    			return;
    		}
    		_oxySwitchKey = oxystatus->valueint;

    		aJsonObject *oxyTime = aJson.getObjectItem(oxy,"time");
    		if(oxyTime == NULL)
    		{
    		    aJson.deleteItem(root);
    			return;
    		}
    	    _oxyTime = oxyTime->valueint;
	    }

	}

	aJson.deleteItem(root);
	_EventCb();
}
