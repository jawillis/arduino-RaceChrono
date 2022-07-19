#ifdef ARDUINO_ARCH_NRF52
#ifndef __RACECHRONONRF52_H
#define __RACECHRONONRF52_H

#include <RaceChrono.h>
#include <bluefruit.h>
#include "RaceChronoPidMap.h"


// void handle_racechrono_filter_request(uint16_t conn_hdl, BLECharacteristic *chr, uint8_t *data, uint16_t len);

// class RaceChronoBleCanHandler {
// public:
//   virtual ~RaceChronoBleCanHandler() {}

//   virtual void allowAllPids(uint16_t updateIntervalMs);
//   virtual void denyAllPids();
//   virtual void allowPid(uint32_t pid, uint16_t updateIntervalMs);
// };

// RaceChronoBleCanHandler *handler;

class RaceChronoNRF52: public RaceChronoBleAgent {
public:
  RaceChronoNRF52();
  virtual ~RaceChronoNRF52();
  using RaceChronoBleAgent::handler;
  using RaceChronoBleAgent::handleFilterRequest;
  void filterRequestHandler(uint16_t conn_hdl, BLECharacteristic *chr, uint8_t *data, uint16_t len);

  // Seems like the length limit for 'bluetoothName' is 19 visible characters.
  void setUp(const char *bluetoothName, RaceChronoBleCanHandler *handler);

  void startAdvertising();

  // Returns true on success, false on failure.
  using RaceChronoBleAgent::waitForConnection;

  bool isConnected() const;

  void notify(unsigned char buffer[20], uint8_t len);

  using RaceChronoBleAgent::sendCanData;

protected:
  
  // The protocol implemented in this file is based on
  // https://github.com/aollin/racechrono-ble-diy-device

  // BLEService docs: https://learn.adafruit.com/bluefruit-nrf52-feather-learning-guide/bleservice
  // BLECharacteristic docs: https://learn.adafruit.com/bluefruit-nrf52-feather-learning-guide/blecharacteristic

  BLEService _service;

  // RaceChrono uses two BLE characteristics:
  // 1) 0x02 to request which PIDs to send, and how frequently
  // 2) 0x01 to be notified of data received for those PIDs
  
  BLECharacteristic _pidRequestsCharacteristic;
  BLECharacteristic _canBusDataCharacteristic;

};

extern RaceChronoNRF52 RaceChronoBle;

#endif
#endif  // __RACECHRONONRF52_H

