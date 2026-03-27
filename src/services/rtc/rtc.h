#ifndef RTC_H
#define RTC_H

void RTC_begin(void *arg);
void RTC_loop(void *arg);
void RTC_set_resynchronization_time(unsigned long time);

#endif