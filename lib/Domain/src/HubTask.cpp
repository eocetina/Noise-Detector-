#include "HubTask.h"

HubTask::HubTask(float time, Ticker::callback_t callback)
{
    this->time = time;
    this->callback = callback;
}

void HubTask::start()
{
    tick_task.attach(time,callback);
}

void HubTask::stop()
{
    tick_task.detach();
}