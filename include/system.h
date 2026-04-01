#ifndef SYSTEM_H
#define SYSTEM_H

#include <stdio.h>
#include "board_defs.h"

void SysConfigClock18M();
void SysConfigClock24M();
void SysConfigClock30M();
void watchdog_init(int time);
void watchdog_feed(void);

#endif