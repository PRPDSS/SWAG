#include <Arduino.h>
#include <animationmanager.h>
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>

// Перед использованием необходимо отдельно инициализировать SPIFFS
// #define FORMAT_SPIFFS_IF_FAILED false

// Название устройства
#define DEVICE_NAME "ProtoBrain v0.1"
// Задаем случайные UUID
#define SERVICE_UUID "3a5516a4-816e-4852-a6fb-0552984f6ce8"
#define CHARACTERISTIC_UUID "76422dba-046a-4e40-b6f3-f40fe04f0e8f"

void setup()
{
    Serial.begin(115200);
    BLEDevice::init(DEVICE_NAME);
    BLEServer *pServer = BLEDevice::createServer();
    BLEService *pService = pServer->createService(SERVICE_UUID);
    BLECharacteristic *pCharacteristics = pService->createCharacteristic(CHARACTERISTIC_UUID,
                                                                         BLECharacteristic::PROPERTY_READ |
                                                                             BLECharacteristic::PROPERTY_WRITE);
    pCharacteristics->setValue("Cocal??");
    pService->start();
    BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
    pAdvertising->addServiceUUID(SERVICE_UUID);
    pAdvertising->setScanResponse(true);
    pAdvertising->setMinPreferred(0x06); // functions that help with iPhone connections issue
    pAdvertising->setMinPreferred(0x12);
    BLEDevice::startAdvertising();
    while (!Serial)
    {
        ;
    }
    Serial.println("Characteristic defined! Now you can read it in your phone!");
}

void loop()
{
}