#ifdef ARDUINO_ARCH_ESP32
#ifndef _RACECHRONOESP32_H
#define _RACECHRONOESP32_H

#include <RaceChrono.h>
#include <NimBLEDevice.h>
#include "RaceChronoPidMap.h"

// class RaceChronoBleCanHandler {
// public:
//   virtual ~RaceChronoBleCanHandler() {}

//   virtual void allowAllPids(uint16_t updateIntervalMs);
//   virtual void denyAllPids();
//   virtual void allowPid(uint32_t pid, uint16_t updateIntervalMs);
// };

// class handleRaceChronoFilterRequestClass: public NimBLECharacteristicCallbacks {
// public:
//     void onWrite(BLECharacteristic* pCharacteristic);
// };

class RaceChronoESP32 : public RaceChronoBleAgent {
public:
  RaceChronoESP32();
  virtual ~RaceChronoESP32();
  using RaceChronoBleAgent::handler;
  using RaceChronoBleAgent::handleFilterRequest;
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
  
  BLEServer* pServer;
  BLEService* pMainService;
  std::string BTName;
  NimBLECharacteristic* _pidRequestsCharacteristic;
  NimBLECharacteristic* _canBusDataCharacteristic;
  
};

extern RaceChronoESP32 RaceChronoBle;

#endif
#endif