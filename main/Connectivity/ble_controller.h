#ifndef BLE_CONTROLLER_H
#define BLE_CONTROLLER_H

#include "NimBLECharacteristic.h"
#include "iconnection_observer.h"

#include <Patterns/single_observable.h>

#include <NimBLEServer.h>

#include <atomic>
#include <mutex>
#include <string>
#include <queue>

class BLE_Configuration;

class BLEController : public NimBLEServerCallbacks,
                      public NimBLECharacteristicCallbacks,
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
  virtual void onConnect(NimBLEServer* server, NimBLEConnInfo& info) override;
  virtual void onDisconnect(NimBLEServer* server, NimBLEConnInfo& info, int reason) override;

  virtual void onWrite(NimBLECharacteristic *characteristic, NimBLEConnInfo& info) override;

private:
  const BLE_Configuration& m_configuration;

  NimBLECharacteristic* m_configurationChannel;

  std::function<void(const std::string&)> m_onConfigurationChunkReceived;

  std::atomic<bool> m_pendingConnect { false };
  std::atomic<bool> m_pendingDisconnect { false };

  std::mutex m_dataMutex;
  std::queue<std::string> m_dataQueue;
};

#endif // BLE_CONTROLLER_H