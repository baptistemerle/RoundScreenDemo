#ifndef OBSERVABLE_VALUE_H
#define OBSERVABLE_VALUE_H

#include <functional>

template <typename T>
class ObservableValue
{
public:
    ObservableValue(T initialValue = T{})
      : m_value(initialValue)
    {}

    void attach(std::function<void(const T&)> callback)
    {
      m_callback = callback;
    }

    operator T() const
    {
      return m_value;
    }

    ObservableValue& operator=(const T& newValue)
    {
      if (m_value != newValue)
      {
        m_value = newValue;

        if (m_callback)
        {
          m_callback(m_value);
        }
      }

      return *this;
    }

private:
    T m_value;
    std::function<void(const T&)> m_callback;
};

#endif // OBSERVABLE_VALUE_H