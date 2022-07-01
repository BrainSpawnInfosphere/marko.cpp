#pragma once

#include <atomic>

// https://gist.github.com/jncornett/e449826d2a1bd6b481f818176be0d2de
// https://stackoverflow.com/questions/69385119/how-do-wait-and-notify-work-for-stdatomic-in-c20
// A version of python's Event class. Assume is_set/set of a bool is atomic and
// no need for a mutex.
class Event {
public:
    Event() {}
    Event(const Event& e) = delete; // don't allow this
    // ~Event() = delete;
    inline void set() noexcept  {flag.test_and_set();}
    inline void clear() noexcept {flag.clear();}
    inline bool is_set() const {return flag.test();}
    inline void wait(bool val) {flag.wait(val);} // block until value met
protected:
    std::atomic_flag flag{false}; // default to false
};



#include <signal.h>

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

/*
 This class captures the SIGINT signal and sets ok to false. Since ok is
 static, any class that inherits this will see the status change and
 allow it close down cleanly.
 */
class SigCapture {
public:
    SigCapture(): enabled(false) {}
    static void my_handler(int s) { ok = false; }
    void on(){
        if (enabled) return;

        struct sigaction sigIntHandler;
        sigIntHandler.sa_handler = SigCapture::my_handler;
        sigemptyset(&sigIntHandler.sa_mask);
        sigIntHandler.sa_flags = 0;

        sigaction(SIGINT, &sigIntHandler, NULL);

        enabled = true;
    }
    void shutdown() { ok = false; }

// protected:
    static bool ok;     // global status on if a SIGINT has occured
    bool enabled;
};

bool SigCapture::ok = true;
