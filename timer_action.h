#ifndef ARDUINO_POMODORO_TIMER_ACTION_H_
#define ARDUINO_POMODORO_TIMER_ACTION_H_

namespace detail {

class TimerActionImpl {
  public:
    TimerActionImpl(uint32_t timer)
      : m_timer(timer)
      , m_lastSnapshot(0)
      , m_isActive(true)
    {
    }

    void sync()
    {
      if (!m_isActive) return;
      if (millis() - m_lastSnapshot >= m_timer) {
        m_lastSnapshot = millis();
        callAction();
      }
    }

    void setActive(boolean isActive)
    {
      if (isActive && !m_isActive) {
        m_lastSnapshot = millis(); // prevent from immediate action call
      }
      m_isActive = isActive;
    }

  protected:
    virtual void callAction() = 0;

    uint32_t m_timer;
    uint32_t m_lastSnapshot;
    boolean m_isActive;
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
