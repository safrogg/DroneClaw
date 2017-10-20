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
#include <EventLoop.h>

#define LED 13

EventLoop scheduler;

void setup() {
  pinMode(LED, OUTPUT);
  // Turn on the LED every 2 secs
  scheduler.repeat([] () {
    digitalWrite(LED, HIGH);
    // Turn off the LED off after 1 sec
    scheduler.execute([] () {
      digitalWrite(LED, LOW);
    }, 1, SECONDS);
  }, 2, SECONDS);
}

void loop() {
  // Must be added to the loop function
  scheduler.process();
}
