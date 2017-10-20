/*
    DroneClaw copyright 2016
*/

#include <EEPROM.h>
#include <EventLoop.h>
#include <SoftwareSerial.h>

#define PIN 2
#define CHANNELS 5
#define VERSION 0
#define STREAM_RX 4
#define STREAM_TX 3

typedef unsigned long uint_t;

struct channel_t {
  uint_t min;
  uint_t mid;
  uint_t max;
};

struct {
  channel_t channels[CHANNELS] = {
    {187, 757, 1197}, // Channel A - throttle
    {187, 699, 1197}, // Channel B - pitch
    {189, 698, 1197}, // Channel C - roll
    {191, 690, 1197}, // Channel D - yaw
    {191, 690, 1197}, // Channel D - claw
  };
} config_eeprom;


SoftwareSerial stream(STREAM_RX, STREAM_TX);
uint_t channels[CHANNELS];

void setup() {
  stream.begin(115200);
  Serial.begin(9600);
  pinMode(13, OUTPUT);
  pinMode(PIN, INPUT);
  // Connect the bluetooth to the drone
  stream.print("$$$");
  delay(1000);
  stream.print("C,201602225787\r");
  delay(5000);
  stream.flush();
  ping_packet();
  delay(1000);
  stream.write(byte(1));
  stream.flush();
  dump_eeprom();
}

/** The ping packet */
void ping_packet() {
  stream.write(byte(0));
  stream.flush();
}

/** The claw packet */
void claw_packet() {
  stream.write(byte(2));
  stream.write(byte(map(channels[4], config_eeprom.channels[4].min, config_eeprom.channels[4].max, 45, 120)));
  stream.flush();
}

/** The data packet */
void data_packet() {
  int* channels = mapped_channels();
  stream.write(byte(3));
  for (unsigned int i = 0 ; i < 4 ; i++) {
    stream.write(highByte(channels[i]));
    stream.write(lowByte(channels[i]));
  }
  stream.flush();
  delete channels;
}

/** The data debug */
void data_debug() {
  if (Serial) {
    int* channel_map = mapped_channels();
    for (unsigned int i = 0 ; i < CHANNELS; i++) {
      Serial.print(channel_map[i]);
      Serial.print(",\t");
    }
    delete channel_map;
    for (unsigned int i = 0 ; i < CHANNELS; i++) {
      Serial.print(channels[i]);
      Serial.print(",\t");
    }
    Serial.println();
  }
}

void loop() {
  digitalWrite(13, !digitalRead(13));
  read_channels();
  //data_debug();
  data_packet();
  claw_packet();
  ping_packet();
  delayMicroseconds(700); // takes about 100 microseconds to complete the rest
}

/** Dump the values from the eeprom */
void dump_eeprom() {
  if (Serial) {
    for (unsigned int i = 0 ; i < CHANNELS ; i++) {
      Serial.print("Channel - ");
      Serial.println(char('A' + i));
      Serial.println(config_eeprom.channels[i].min);
      Serial.println(config_eeprom.channels[i].mid);
      Serial.println(config_eeprom.channels[i].max);
    }
  }
}

/** Remap the channels to match what we need */
int* mapped_channels() {
  int* data = new int[CHANNELS];
  data[0] = map(channels[0], config_eeprom.channels[0].min, config_eeprom.channels[0].max, 999, 1800);
  for (unsigned int i = 1 ; i < CHANNELS ; i++) {
    data[i] = data[i] > config_eeprom.channels[i].mid - 8 && data[i] < config_eeprom.channels[i].mid + 4 ? config_eeprom.channels[i].mid : data[i];
    data[i] = map(channels[i], config_eeprom.channels[i].min, config_eeprom.channels[i].max, -15, 15);
  }
  return data;
}

/** Read the channels and set the global var */
void read_channels() {
  for (unsigned int i = 0 ; i < CHANNELS ; i++) {
    uint_t tmp = pulseIn(PIN, HIGH);
    if (tmp <= config_eeprom.channels[i].max && tmp >= config_eeprom.channels[i].min) {
      channels[i] = (0.5 * channels[i] + 0.5 * tmp);
    }
  }
}

