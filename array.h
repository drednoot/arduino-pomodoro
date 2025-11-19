#ifndef ARDUINO_POMODORO_ARRAY_H_
#define ARDUINO_POMODORO_ARRAY_H_

/**
 * @brief Array class for simple types
 */
template<class T, uint8_t arrayCapacity>
class Array {
  public:
    Array()
      : m_size { 0 }
    {
    }

    uint8_t size() const
    {
      return m_size;
    }

    uint8_t capacity() const
    {
      return arrayCapacity;
    }

    // always copy as T is simple
    T operator[](uint8_t i) const
    {
      return m_data[i];
    }

    void push(T val)
    {
      m_data[m_size] = val;
      ++m_size;
    }

    void clear()
    {
      for (int i = 0; i < m_size; ++i) {
        m_data[i] = T {};
      }
      m_size = 0;
    }

  private:
    T m_data[arrayCapacity];
    uint8_t m_size;
};

#endif // ARDUINO_POMODORO_ARRAY_H_
