#ifdef ARDUINO_ARCH_NRF52
#include "RaceChronoNRF52.h"

#define MAIN_SERVICE_NAME           (uint16_t) 0x1ff8
#define PID_CHARACTERISTIC          (uint16_t) 0x02
#define CANBUS_CHARACTERISTIC       (uint16_t) 0x01

// namespace {
// RaceChronoBleCanHandler RaceChronoBleAgent::*handler = nullptr;

// void RaceChronoBleAgent::handle_racechrono_filter_request(
//     uint16_t conn_hdl, BLECharacteristic *chr, uint8_t *data, uint16_t len) {
//   if (len < 1) {
//     // TODO: figure out how to report errors.
//     return;
//   }

//   switch (data[0]) {
//     case 1:  // Allow all CAN PIDs.
//       if (len == 3) {
//         uint16_t updateIntervalMs = data[1] << 8 | data[2];
//         handler->allowAllPids(updateIntervalMs);
//         return;
//       }
//       break;

//     case 0:  // Deny all CAN PIDs.
//       if (len == 1) {
//         handler->denyAllPids();
//         return;
//       }
//       break;

//     case 2:  // Allow one more CAN PID.
//       if (len == 7) {
//         uint16_t updateIntervalMs = data[1] << 8 | data[2];
//         uint32_t pid = data[3] << 24 | data[4] << 16 | data[5] << 8 | data[6];
//         handler->allowPid(pid, updateIntervalMs);
//         return;
//       }
//       break;
//   }
//   // TODO: figure out how to report errors.
// }
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

RaceChronoNRF52::RaceChronoNRF52() :
  _service(/* uuid= */ MAIN_SERVICE_NAME),
  _pidRequestsCharacteristic(PID_CHARACTERISTIC),
  _canBusDataCharacteristic(CANBUS_CHARACTERISTIC)
{
}

RaceChronoNRF52::~RaceChronoNRF52()
{  
}

// static handleRaceChronoFilterRequestClass cbhandle_racechrono_filter_request;


void RaceChronoNRF52::setUp(
    const char *bluetoothName, RaceChronoBleCanHandler *handler) {
  // ::handler = handler;

  Bluefruit.configPrphBandwidth(BANDWIDTH_MAX);
  Bluefruit.begin();
  Bluefruit.setName(bluetoothName);

  _service.begin();

  _pidRequestsCharacteristic.setProperties(CHR_PROPS_WRITE);
  _pidRequestsCharacteristic.setPermission(SECMODE_NO_ACCESS, SECMODE_OPEN);
  _pidRequestsCharacteristic.setWriteCallback(filterRequestHandler);
  _pidRequestsCharacteristic.begin();

  _canBusDataCharacteristic.setProperties(CHR_PROPS_NOTIFY | CHR_PROPS_READ);
  _canBusDataCharacteristic.setPermission(SECMODE_OPEN, SECMODE_NO_ACCESS);
  _canBusDataCharacteristic.begin();

  Bluefruit.setTxPower(+4);
}


void RaceChronoNF52::startAdvertising() {
  Bluefruit.Advertising.addFlags(BLE_GAP_ADV_FLAGS_LE_ONLY_GENERAL_DISC_MODE);
  Bluefruit.Advertising.addTxPower();
  Bluefruit.Advertising.addService(_service);
  Bluefruit.Advertising.addName();
  Bluefruit.Advertising.restartOnDisconnect(true);

  // Fast mode interval: 20 ms, slow mode interval: 152.5 ms.
  Bluefruit.Advertising.setInterval(/* fast= */ 32, /* slow= */ 244); // x0.625 ms

  // Timeout for fast mode is 30 seconds.
  Bluefruit.Advertising.setFastTimeout(30);

  // Start advertising forever.
  Bluefruit.Advertising.start(/* timeout= */ 0);
}


bool RaceChronoNRF52::isConnected() const {
  return Bluefruit.connected();
}


void RaceChronoNRF52::notify(unsigned char buffer[20], uint8_t len) 
{
  _canBusDataCharacteristic.notify(buffer, 4 + len);
}
#endif