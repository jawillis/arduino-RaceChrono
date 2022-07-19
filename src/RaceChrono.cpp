#include "RaceChrono.h"

#define MAIN_SERVICE_NAME           (uint16_t) 0x1ff8
#define PID_CHARACTERISTIC          (uint16_t) 0x02
#define CANBUS_CHARACTERISTIC       (uint16_t) 0x01

// namespace {
RaceChronoBleCanHandler RaceChronoBleAgent::*handler = nullptr;

void RaceChronoBleAgent::handleFilterRequest(RaceChronoBleCanHandler *handler, uint8_t *data, uint16_t len) {
  if (len < 1) {
    // TODO: figure out how to report errors.
    return;
  }

  switch (data[0]) {
    case 1:  // Allow all CAN PIDs.
      if (len == 3) {
        uint16_t updateIntervalMs = data[1] << 8 | data[2];
        handler->allowAllPids(updateIntervalMs);
        return;
      }
      break;

    case 0:  // Deny all CAN PIDs.
      if (len == 1) {
        handler->denyAllPids();
        return;
      }
      break;

    case 2:  // Allow one more CAN PID.
      if (len == 7) {
        uint16_t updateIntervalMs = data[1] << 8 | data[2];
        uint32_t pid = data[3] << 24 | data[4] << 16 | data[5] << 8 | data[6];
        handler->allowPid(pid, updateIntervalMs);
        return;
      }
      break;
  }
  // TODO: figure out how to report errors.
}
// }

// class handleRaceChronoFilterRequestClass: public NimBLECharacteristicCallbacks {

//   void onWrite(BLECharacteristic* pCharacteristic) {
//     std::string data = pCharacteristic->getValue();
//     uint8_t* dataint = (uint8_t*)data.data();
//     int len = pCharacteristic->getValue().length();
//     handle_racechrono_filter_request(NULL, pCharacteristic, dataint, len);
//   }
// };


// RaceChronoBleAgent RaceChronoBle;


RaceChronoBleAgent::RaceChronoBleAgent() 
{
}

RaceChronoBleAgent::~RaceChronoBleAgent()
{  
}


// #if defined(ARDUINO_ARCH_NRF52)
// void RaceChronoBleAgent::setUp(
//     const char *bluetoothName, RaceChronoBleCanHandler *handler) {
//   // ::handler = handler;

//   // Bluefruit.configPrphBandwidth(BANDWIDTH_MAX);
//   // Bluefruit.begin();
//   // Bluefruit.setName(bluetoothName);

//   // _service.begin();

//   // _pidRequestsCharacteristic.setProperties(CHR_PROPS_WRITE);
//   // _pidRequestsCharacteristic.setPermission(SECMODE_NO_ACCESS, SECMODE_OPEN);
//   // _pidRequestsCharacteristic.setWriteCallback(handle_racechrono_filter_request);
//   // _pidRequestsCharacteristic.begin();

//   // _canBusDataCharacteristic.setProperties(CHR_PROPS_NOTIFY | CHR_PROPS_READ);
//   // _canBusDataCharacteristic.setPermission(SECMODE_OPEN, SECMODE_NO_ACCESS);
//   // _canBusDataCharacteristic.begin();

//   // Bluefruit.setTxPower(+4);
// }
// #elif defined(ARDUINO_ARCH_ESP32)
// void RaceChronoBleAgent::setUp(
//     const char *bluetoothName, RaceChronoBleCanHandler *handler) {
// //   ::handler = handler;
// //   BTName = bluetoothName;
// //   BLEDevice::init(BTName);
// //   BLEDevice::setSecurityIOCap(BLE_HS_IO_NO_INPUT_OUTPUT);
// //   pServer = BLEDevice::createServer();
// //   pMainService = pServer->createService(MAIN_SERVICE_NAME);

// //   _pidRequestsCharacteristic = pMainService->createCharacteristic( 
// //                 PID_CHARACTERISTIC, 
// //                 NIMBLE_PROPERTY::WRITE
// //                 );
// //   _pidRequestsCharacteristic->setCallbacks(&cbhandle_racechrono_filter_request);

// //   _canBusDataCharacteristic = pMainService->createCharacteristic( 
// //                 CANBUS_CHARACTERISTIC, 
// //                 NIMBLE_PROPERTY::READ |
// //                 NIMBLE_PROPERTY::NOTIFY
// //                 );
// //   pMainService->start();
  
// }
// #endif


bool RaceChronoBleAgent::waitForConnection(uint32_t timeoutMs) {
  uint32_t startTimeMs = millis();
  while (!isConnected()) {
    if (millis() - startTimeMs >= timeoutMs) {
      return false;
    }
    delay(100);
  }

  return true;
}


void RaceChronoBleAgent::sendCanData(
    uint32_t pid, const uint8_t *data, uint8_t len) {
  if (len > 8) {
    len = 8;
  }

  unsigned char buffer[20];
  buffer[0] = pid & 0xFF;
  buffer[1] = (pid >> 8) & 0xFF;
  buffer[2] = (pid >> 16) & 0xFF;
  buffer[3] = (pid >> 24) & 0xFF;
  memcpy(buffer + 4, data, len);
// #if defined(ARDUINO_ARCH_NRF52)
//   _canBusDataCharacteristic.notify(buffer, 4 + len);
// #elif defined(ARDUINO_ARCH_ESP32)
//   _canBusDataCharacteristic->setValue(buffer, 4 + len);
//   _canBusDataCharacteristic->notify();
// #endif
  notify(buffer, 4 + len);
}
