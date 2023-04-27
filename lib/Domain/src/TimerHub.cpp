#include "TimerHub.h"
#include <Hub.h>

bool TimerHub::updateSensorValues = false;
bool TimerHub::updateAverageValue = false;
bool TimerHub::sendSensorValues = false;
bool TimerHub::sendAverageValues = false;
const float TimerHub::UPDATE_SOUND_VALUE_SEC  = 5.0;     // 5sec
const float TimerHub::UPDATE_SOUND_AVERAGE_SEC = 60.0;     // 60sec
const float TimerHub::NORMAL_MODE_SEND_SOUND_SEC  = 5.0;     // 5sec
const float TimerHub::NORMAL_MODE_SEND_AVERAGE_SEC = 60.0;     // 60sec
unsigned int TimerHub::eco_count = 0;

Ticker TimerHub::valueUpdateTicker;
Ticker TimerHub::averageUpdateTicker;

TimerHub::Mode TimerHub::mode = TimerHub::Mode::NORMAL;

TimerHub::Mode TimerHub::getMode()
{
  return mode;
}

/**
 * 
*/
TimerHub::TimerHub()
{
  updateSensorValues = false;
  updateAverageValue = false;
  sendSensorValues = false;
  sendAverageValues = false;
}

void TimerHub::setup(){
  updateSensorValues = false;
  updateAverageValue = false;
  sendSensorValues = false;
  sendAverageValues = false;

  valueUpdateTicker.attach((float)UPDATE_SOUND_VALUE_SEC, TimerHub::updateValuesCallback);
  averageUpdateTicker.attach((float)UPDATE_SOUND_AVERAGE_SEC, TimerHub::updateAverageCallback);

  mode = Mode::NORMAL;
}

/**
 * 
 */
void TimerHub::updateValuesCallback(){
  updateSensorValues = true;
  if (mode == Mode::NORMAL)
  {
    sendSensorValues = true;
  }
}

void TimerHub::updateAverageCallback(){
  updateAverageValue = true;
  if (mode == Mode::NORMAL)
  {
    sendAverageValues = true;
  }
  else
  {
    if(++eco_count >= Hub::getECOInterval())
    {
      sendAverageValues = true;
      eco_count = 0;
    }
  }
  
}

/**
 * 
 */
bool TimerHub::isTimeUpdateValues(){
  return updateSensorValues;
}

bool TimerHub::isTimeUpdateAverage(){
  return updateAverageValue;
}

bool TimerHub::isTimeSendValues(){
  return sendSensorValues;
}

bool TimerHub::isTimeSendAverage(){
  return sendAverageValues;
}
/**
 * 
 */
void TimerHub::valuesUpdated(){
  updateSensorValues = false;
}

void TimerHub::averageUpdated(){
  updateAverageValue = false;
}

void TimerHub::valuesSent(){
  sendSensorValues = false;
}

void TimerHub::averageSent(){
  sendAverageValues = false;
}

/**
 * 
 */

void TimerHub::changeToEcoMode()
{
  auto hub = Hub::getInstance();
  valueUpdateTicker.detach();
  averageUpdateTicker.detach();
  hub->clearSoundAverageQueue();
  eco_count = 0;

  valueUpdateTicker.attach((float)UPDATE_SOUND_VALUE_SEC, TimerHub::updateValuesCallback);
  averageUpdateTicker.attach((float)UPDATE_SOUND_AVERAGE_SEC, TimerHub::updateAverageCallback);

  mode = Mode::ECO;

  updateSensorValues = false;
  updateAverageValue = false;
  sendSensorValues = false;
  sendAverageValues = false;

}

void TimerHub::changeToNormalMode()
{
  auto hub = Hub::getInstance();

  valueUpdateTicker.detach();
  averageUpdateTicker.detach();

  hub->clearSoundAverageQueue();

  updateSensorValues = false;
  updateAverageValue = false;
  sendSensorValues = false;
  sendAverageValues = false;

  valueUpdateTicker.attach((float)UPDATE_SOUND_VALUE_SEC, TimerHub::updateValuesCallback);
  averageUpdateTicker.attach((float)UPDATE_SOUND_AVERAGE_SEC, TimerHub::updateAverageCallback);

  mode = Mode::NORMAL;
}