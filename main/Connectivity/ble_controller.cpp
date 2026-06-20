#include "ble_controller.h"

#include "ble_configuration.h"

#include <NimBLEDevice.h>

BLEController::BLEController(const BLE_Configuration& configuration)
  : m_configuration(configuration)
{
}

void BLEController::init()
{
  NimBLEDevice::init(m_configuration.name.data());

  NimBLEServer* server = NimBLEDevice::createServer();
  server->setCallbacks(this);

  NimBLEService* service = server->createService(m_configuration.serviceUUID.data());

  m_configurationChannel = service->createCharacteristic(m_configuration.configurationChannelUUID.data(), NIMBLE_PROPERTY::WRITE);
  m_configurationChannel->setCallbacks(this);

  NimBLEDescriptor* descriptor = m_configurationChannel->createDescriptor
  (
    "2901",
    NIMBLE_PROPERTY::READ
  );
  descriptor->setValue("Configuration JSON");

  m_configurationChannel->createDescriptor
  (
    "2902",
    NIMBLE_PROPERTY::READ | NIMBLE_PROPERTY::WRITE
  );
}

void BLEController::setConfigurationChunkCallback(std::function<void(const std::string&)> callback)
{
  m_onConfigurationChunkReceived = callback;
}

void BLEController::startAdvertising()
{
  NimBLEAdvertising *pAdvertising = NimBLEDevice::getAdvertising();

  NimBLEAdvertisementData advertisingData;
  advertisingData.setFlags(BLE_HS_ADV_F_DISC_GEN | BLE_HS_ADV_F_BREDR_UNSUP);
  advertisingData.addServiceUUID(NimBLEUUID(m_configuration.serviceUUID.data()));
  pAdvertising->setAdvertisementData(advertisingData);

  NimBLEAdvertisementData scanData;
  scanData.setName(m_configuration.name.data());
  pAdvertising->setScanResponseData(scanData);

  NimBLEDevice::startAdvertising();
}

void BLEController::process()
{
  if (m_pendingConnect.exchange(false))
  {
    notify(&IConnectionObserver::onConnected);
  }

  if (m_pendingDisconnect.exchange(false))
  {
    notify(&IConnectionObserver::onDisconnected);
  }

  std::queue<std::string> localQueue;

  {
    std::lock_guard<std::mutex> lock(m_dataMutex);
    std::swap(m_dataQueue, localQueue);
  }

  while (!localQueue.empty())
  {
    std::string currentData = localQueue.front();
    localQueue.pop();

    if (m_onConfigurationChunkReceived)
    {
      m_onConfigurationChunkReceived(currentData);
    }
  }
}

void BLEController::onConnect(NimBLEServer* server, NimBLEConnInfo& info)
{
  (void)server;
  (void)info;

  m_pendingConnect = true;
}

void BLEController::onDisconnect(NimBLEServer* server, NimBLEConnInfo& info, int reason)
{
  (void)server;
  (void)info;
  (void)reason;

  m_pendingDisconnect = true;
}

void BLEController::onWrite(NimBLECharacteristic* characteristic, NimBLEConnInfo& info)
{
  (void)info;

  if (characteristic != m_configurationChannel)
    return;

  if (!m_onConfigurationChunkReceived)
    return;

  std::string value = m_configurationChannel->getValue();

  std::lock_guard<std::mutex> lock(m_dataMutex);
  m_dataQueue.push(value);
}