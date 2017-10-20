/*
 * Copyright 2015 Joshua Rodriguez.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef _EVENT_LOOP
#define _EVENT_LOOP

#include <Arduino.h>
#include "utilities/LinkedQueue.h"

/** The unit of time to make scheduling better */
enum Unit {
  MILLIS = 1,
  SECONDS = 1000,
  MINUTES = 60000,
  HOURS = 36000000,
  DAYS = 8640000000,
};

/** A singlton EventLoop that provides a scheduler */
class EventLoop {
  private:
    struct Task {
      void (*_function)(); // The function to be ran
      bool _repeat; // Should this event be looped
      unsigned long _caculate_time; // the caculated to eslapsed
      unsigned long _delay; // the delay what will be muliplied with Unit
      /** Grab the current time of when the event was created */
      inline Task() {
        _caculate_time = millis();
      };
    };
    LinkedQueue<Task> _queue;
    /** Add the function event to the queue */
    inline void add(void (*function)(), const long delay, Unit unit, const bool repeat) {
      add(function, delay * unit, repeat);
    };
    /** Add the function event to the queue */
    inline void add(void (*function)(), const long delay, const bool repeat) {
      Task task;
      task._function = function;
      task._caculate_time += task._delay = delay;
      task._repeat = repeat;
      _queue.push(task);
    };
  public:
    /** Run the function */
    inline void execute(void (*function)()) {
      add(function, 0, MILLIS, false);
    };
    /** Run the function with a delay */
    inline void execute(void (*function)(), const long delay, const Unit unit) {
      add(function, delay, unit, false);
    };
    /** Repeat the function with a delay */
    inline void repeat(void (*function)()) {
      add(function, 1, MILLIS, true);
    };
    /** Repeat the function with a delay */
    inline void repeat(void (*function)(), const long delay, const Unit unit) {
      add(function, delay, unit, true);
    };
    /** Get the current size of the event */
    inline int size() const {
      return _queue.size();
    };
    /** Process the event */
    inline void process() {
      if (_queue.size() > 0) {
        Task task = _queue.pop();
        // Has enuf time passed to execute the function
        if (millis() >= task._caculate_time) {
          // We want to repeat, recreate event
          if (task._repeat) {
            add(task._function, task._delay, true);
          }
          // Execute the function
          task._function();
        } else { // Time has not eslapsed yet add to queue
          _queue.push(task);
        }
      }
    };
};

#endif
