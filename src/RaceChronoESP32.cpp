#ifdef ARDUINO_ARCH_ESP32
#include "RaceChronoESP32.h"

#define MAIN_SERVICE_NAME           (uint16_t) 0x1ff8
#define PID_CHARACTERISTIC          (uint16_t) 0x02
#define CANBUS_CHARACTERISTIC       (uint16_t) 0x01


RaceChronoESP32 RaceChronoBle;
// RaceChronoBleCanHandler *handler = nullptr;

class handleRaceChronoFilterRequestClass: public NimBLECharacteristicCallbacks {

  RaceChronoBleCanHandler handler;

  handleRaceChronoFilterRequestClass(RaceChronoBleCanHandler handler) 
  {
    handler = handler;
  }

  void onWrite(BLECharacteristic* pCharacteristic) {
    std::string data = pCharacteristic->getValue();
    uint8_t* dataint = (uint8_t*)data.data();
    int len = pCharacteristic->getValue().length();
    RaceChronoBle.handleFilterRequest(&handler, dataint, len);
  }
};



RaceChronoESP32::RaceChronoESP32() :
  RaceChronoBleAgent(),
  BTName(),
  pServer(),
  pMainService(),
  _pidRequestsCharacteristic(),
  _canBusDataCharacteristic()
{
}

RaceChronoESP32::~RaceChronoESP32()
{
}



void RaceChronoESP32::setUp(
    const char *bluetoothName, RaceChronoBleCanHandler *canhandler) {
  // handler = handler;
  static handleRaceChronoFilterRequestClass cbhandle_racechrono_filter_request(*canhandler);
  BTName = bluetoothName;
  BLEDevice::init(BTName);
  BLEDevice::setSecurityIOCap(BLE_HS_IO_NO_INPUT_OUTPUT);
  pServer = BLEDevice::createServer();
  pMainService = pServer->createService(MAIN_SERVICE_NAME);

  _pidRequestsCharacteristic = pMainService->createCharacteristic( 
                PID_CHARACTERISTIC, 
                NIMBLE_PROPERTY::WRITE
                );
  _pidRequestsCharacteristic->setCallbacks(&cbhandle_racechrono_filter_request);

  _canBusDataCharacteristic = pMainService->createCharacteristic( 
                CANBUS_CHARACTERISTIC, 
                NIMBLE_PROPERTY::READ |
                NIMBLE_PROPERTY::NOTIFY
                );
  pMainService->start();
  
}

void RaceChronoESP32::startAdvertising() {
  
  NimBLEAdvertising* pAdvertising = NimBLEDevice::getAdvertising();
  pAdvertising->setName(BTName);
  pAdvertising->setMinInterval(32);
  pAdvertising->setMaxInterval(160);
  pAdvertising->addServiceUUID(pMainService->getUUID());
  pAdvertising->setScanResponse(false);
  pAdvertising->start();
}

bool RaceChronoESP32::isConnected() const {
  int32_t connectedCount;
  connectedCount = pServer->getConnectedCount();
  return (connectedCount > 0);
}

void RaceChronoESP32::notify(unsigned char *buffer, uint8_t len) {
  _canBusDataCharacteristic->setValue(buffer, 4 + len);
  _canBusDataCharacteristic->notify();
}

#endif