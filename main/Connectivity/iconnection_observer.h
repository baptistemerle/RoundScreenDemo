#ifndef ICONNECTION_OBSERVER_H
#define ICONNECTION_OBSERVER_H

class IConnectionObserver
{
public:
  virtual void onConnected() = 0;
  virtual void onDisconnected() = 0;
};

#endif // ICONNECTION_OBSERVER_H