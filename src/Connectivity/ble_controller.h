#ifndef BLE_CONTROLLER_H
#define BLE_CONTROLLER_H

#include "iconnection_observer.h"

#include <Patterns/single_observable.h>

#include <BLEServer.h>

#include <atomic>
#include <mutex>
#include <string>
#include <queue>

class BLEServerCallbacks;
class BLECharacteristicCallbacks;
class BLE_Configuration;

class BLEController : public BLEServerCallbacks,
                      public BLECharacteristicCallbacks,
                      public SingleObservable<IConnectionObserver>
{
public:
  BLEController(const BLE_Configuration& configuration);

  void init();

  void setConfigurationChunkCallback(std::function<void(const std::string&)> callback);

  void startAdvertising();

  /*
   * Processes BLE events queued by the hardware stack.
   * Warning: The ESP32 BLE stack runs on a separate FreeRTOS task (Core 0).
   * To ensure thread safety, this function must be called continuously from
   * the main thread (Core 1) to safely transfer data to the application state.
   */
  void process();

private:
  virtual void onConnect(BLEServer* server) override;
  virtual void onDisconnect(BLEServer* server) override;

  virtual void onWrite(BLECharacteristic *characteristic) override;

private:
  const BLE_Configuration& m_configuration;

  BLECharacteristic* m_configurationChannel;

  std::function<void(const std::string&)> m_onConfigurationChunkReceived;

  std::atomic<bool> m_pendingConnect { false };
  std::atomic<bool> m_pendingDisconnect { false };

  std::mutex m_dataMutex;
  std::queue<std::string> m_dataQueue;
};

#endif // BLE_CONTROLLER_H