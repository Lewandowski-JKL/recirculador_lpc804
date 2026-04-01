#ifndef RTC_H
#define RTC_H

void RTC_begin(unsigned int *ptrTimestamp);
void RTC_loop(void *arg);
void RTC_set_resynchronization_time(unsigned long time);
void RTC_resynchronization();
void RTC_setValue(unsigned int timestampRef);

#endif