#ifndef ARDUINO_POMODORO_ONCE_H_
#define ARDUINO_POMODORO_ONCE_H_

template<typename T>
class Once {
  public:
    Once()
    {
    }

    Once(T val)
      : m_val(val)
    {
    }

    boolean set(T val)
    {
      if (m_val != val) {
        m_val = val;
        return true;
      }
      return false;
    }

    T operator*() const { return m_val; }
    T operator->() const { return m_val; }
    T operator=(T other)
    {
      m_val = other.m_val; 
      return m_val;
    }

  private:
    T m_val;
};

#endif // ARDUINO_POMODORO_ONCE_H_
