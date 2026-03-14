#include "ble_controller.h"

#include "ble_configuration.h"

#include <BLEDevice.h>
#include <BLE2902.h>

BLEController::BLEController(const BLE_Configuration& configuration)
  : m_configuration(configuration)
{
}

void BLEController::init()
{
  BLEDevice::init(m_configuration.name);

  BLEServer* server = BLEDevice::createServer();
  server->setCallbacks(this);

  BLEService* service = server->createService(m_configuration.serviceUUID);

  m_configurationChannel = service->createCharacteristic(m_configuration.configurationChannelUUID, BLECharacteristic::PROPERTY_WRITE);
  m_configurationChannel->setCallbacks(this);

  BLEDescriptor* descriptor = new BLEDescriptor((uint16_t)0x2901);
  descriptor->setValue("Configuration JSON");
  m_configurationChannel->addDescriptor(descriptor);

  m_configurationChannel->addDescriptor(new BLE2902());

  service->start();
}

void BLEController::setConfigurationChunkCallback(std::function<void(const std::string&)> callback)
{
  m_onConfigurationChunkReceived = callback;
}

void BLEController::startAdvertising()
{
  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(m_configuration.serviceUUID);
  pAdvertising->setScanResponse(true);
  BLEDevice::startAdvertising();
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

void BLEController::onConnect(BLEServer* server)
{
  m_pendingConnect = true;
}

void BLEController::onDisconnect(BLEServer* server)
{
  m_pendingDisconnect = true;
}

void BLEController::onWrite(BLECharacteristic* characteristic)
{
  if (characteristic != m_configurationChannel)
    return;

  if (!m_onConfigurationChunkReceived)
    return;

  std::string value = m_configurationChannel->getValue();

  std::lock_guard<std::mutex> lock(m_dataMutex);
  m_dataQueue.push(value);
}