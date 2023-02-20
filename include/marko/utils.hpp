#pragma once

#include <atomic>
#include <exception>
#include <signal.h>

// https://gist.github.com/jncornett/e449826d2a1bd6b481f818176be0d2de
// https://stackoverflow.com/questions/69385119/how-do-wait-and-notify-work-for-stdatomic-in-c20
// A version of python's Event class. Assume is_set/set of a bool is atomic and
// no need for a mutex.
class Event {
public:
  Event() {}
  Event(const Event &e) = delete; // don't allow this
  // ~Event() = delete;
  inline void set() noexcept { flag.test_and_set(); }
  inline void clear() noexcept { flag.clear(); }
  inline bool is_set() const { return flag.test(); }
  inline void wait(bool val) { flag.wait(val); } // block until value met
protected:
  std::atomic_flag flag{false}; // default to false
};

/*
https://stackoverflow.com/questions/1641182/how-can-i-catch-a-ctrl-c-event
https://www.geeksforgeeks.org/inheritance-in-c/
https://stackoverflow.com/questions/12662891/how-can-i-pass-a-member-function-where-a-free-function-is-expected
http://www.yolinux.com/TUTORIALS/C++Signals.html
*/
/*
kevin@Logan build $ kill -l
 1) SIGHUP	 2) SIGINT	 3) SIGQUIT	 4) SIGILL
 5) SIGTRAP	 6) SIGABRT	 7) SIGEMT	 8) SIGFPE
 9) SIGKILL	10) SIGBUS	11) SIGSEGV	12) SIGSYS
13) SIGPIPE	14) SIGALRM	15) SIGTERM	16) SIGURG
17) SIGSTOP	18) SIGTSTP	19) SIGCONT	20) SIGCHLD
21) SIGTTIN	22) SIGTTOU	23) SIGIO	24) SIGXCPU
25) SIGXFSZ	26) SIGVTALRM	27) SIGPROF	28) SIGWINCH
29) SIGINFO	30) SIGUSR1	31) SIGUSR2
*/

/**
 * Captures a signal and calls a callback function.
 *
 * ref: https://man7.org/linux/man-pages/man2/sigaction.2.html
 */
class SignalFunc {
public:
  SignalFunc() {}
  void enable(void (*f)(int), int signum = SIGINT) {
    if (enabled) return;

    if (signum == SIGKILL || signum == SIGSTOP) {
      throw std::invalid_argument(
          "** Cannot capture signals SIGKILL or SIGSTOP **");
    }

    struct sigaction sigIntHandler;
    sigIntHandler.sa_handler = f;
    sigemptyset(&sigIntHandler.sa_mask);
    sigIntHandler.sa_flags = 0;

    sigaction(signum, &sigIntHandler, NULL);

    enabled = true;
  }

protected:
  bool enabled{false};
};

/**
 * Combination of the Event and SignalFunc classes.
 */
class EventSignal : public Event, public SignalFunc {
public:
  EventSignal() {}
};
