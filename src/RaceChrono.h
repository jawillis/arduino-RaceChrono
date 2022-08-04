#ifndef RACECHRONO_BLE_H
#define RACECHRONO_BLE_H

#if defined(ARDUINO_ARCH_NRF52)
#include "RaceChronoNRF52.h"
#else
#include "RaceChronoESP32.h"
#endif

#endif