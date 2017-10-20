# EventLoopScheduler

A scheduler system that uses an event loop to schedule tasks,
to be ran in a non blocking system. It uses native functions and lambdas
to create tasks. The key part of making it simple is the use of singleton.
Anything related to the `EventLoop` can be used from `EventLoop::get()`.

## Usage

Examples can be found in `examples` folder.

```c++
// Turn on the LED every 2 secs
EventLoop::get().repeat([] () {
  digitalWrite(13, HIGH);
  // Turn off the LED off after 1 sec
  EventLoop::get().execute([] () {
    digitalWrite(13, LOW);
  }, 1, SECONDS);
}, 2, SECONDS);
```

Must be added to the loop function

```c++
EventLoop::get().process();
```
