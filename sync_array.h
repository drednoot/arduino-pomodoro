#ifndef ARDUINO_POMODORO_SYNC_ARRAY_H_
#define ARDUINO_POMODORO_SYNC_ARRAY_H_

#include "move.h"

template<class Syncable, uint8_t size>
class SyncArray {
  public:
    template<class ...Args>
    SyncArray(Args&& ...args)
    {
      static_assert(sizeof...(args) == size);
      initArray(pomo::move(args)...);
    }

    void sync() 
    {
      for (int i = 0; i < size; ++i) {
        m_data[i].sync();
      }
    }

  private:
    template<class ...Args>
    initArray(Syncable&& first, Args&& ...rest)
    {
      m_data[size - sizeof...(rest) - 1] = first;
      initArray(pomo::move(rest)...);
    }
    initArray(Syncable&& last)
    {
      m_data[size - 1] = last;
    }

    Syncable m_data[size];
};

#endif // ARDUINO_POMODORO_SYNC_ARRAY_H_
