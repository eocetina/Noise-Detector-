#ifndef HUB_TASK_H
#define HUB_TASK_H

#include <Ticker.h>

class HubTask
{
    private:
    Ticker tick_task;
    float time;
    Ticker::callback_t callback;

    public:
    HubTask (float time, Ticker::callback_t callback);
    void start();
    void stop();
};

#endif