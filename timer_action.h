#ifndef ARDUINO_POMODORO_TIMER_ACTION_H_
#define ARDUINO_POMODORO_TIMER_ACTION_H_

namespace detail {

class TimerActionImpl {
  public:
    TimerActionImpl(uint32_t timer)
      : m_timer(timer)
      , m_lastSnapshot(0)
    {
    }

    void sync()
    {
      if (millis() - m_lastSnapshot >= m_timer) {
        m_lastSnapshot = millis();
        callAction();
      }
    }

  protected:
    virtual void callAction() = 0;

    uint32_t m_timer;
    uint32_t m_lastSnapshot;
};

} // namespace detail

template<typename Action, typename Caller = void>
class TimerAction : public detail::TimerActionImpl {
  public:
  TimerAction(uint32_t timer, Caller caller, Action action)
    : detail::TimerActionImpl(timer)
    , m_caller(caller)
    , m_action(action)
  {
  }

  size_t getCaller() {
    return (size_t)m_caller;
  }

  private:
  virtual void callAction() override
  {
    (m_caller->*m_action)();
  }

  Caller m_caller;
  Action m_action;
};

template<typename Action>
class TimerAction<Action, void> : public detail::TimerActionImpl {
  public:
  TimerAction(uint32_t timer, Action action) 
    : detail::TimerActionImpl(timer)
    , m_action(action)
  {
  }

  private:
  virtual void callAction() override
  {
    m_action();
  }

  Action m_action;
};

#endif // ARDUINO_POMODORO_TIMER_ACTION_H_
