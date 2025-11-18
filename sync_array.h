#ifndef ARDUINO_POMODORO_SYNC_ARRAY_H_
#define ARDUINO_POMODORO_SYNC_ARRAY_H_

template<class Syncable, uint8_t size>
class SyncArray {
  public:
    template<class ...Args>
    SyncArray(Args ...args)
    {
      static_assert(sizeof...(args) == size);
      initArray(args...);
    }

    void sync() 
    {
      for (int i = 0; i < size; ++i) {
        m_data[i].sync();
      }
    }

  private:
    template<class ...Args>
    void initArray(Syncable first, Args ...rest)
    {
      m_data[size - sizeof...(rest) - 1] = first;
      initArray(rest...);
    }
    void initArray(Syncable last)
    {
      m_data[size - 1] = last;
    }

    Syncable m_data[size];
};

#endif // ARDUINO_POMODORO_SYNC_ARRAY_H_
