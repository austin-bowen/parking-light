#include <FlashStorage.h>

#include "DistanceSensor.h"
#include "Light.h"
#include "Panel.h"


// Pin assignments
const int CLOSE_ENOUGH_BUTTON_PIN = 6;
const int TOO_CLOSE_BUTTON_PIN = 5;
const int ULTRASONIC_TRIGGER_PIN = 9;
const int ULTRASONIC_ECHO_PIN = 10;
const int LIGHT_PIN = 7;


// Distance sensor
// - [ms]
const unsigned long ACTIVE_POLLING_PERIOD = 200;
const unsigned long INACTIVE_POLLING_PERIOD = 3000;
const unsigned long ACTIVE_TIMEOUT = 30000;
DistanceSensor distance_sensor(
  ULTRASONIC_TRIGGER_PIN,
  ULTRASONIC_ECHO_PIN,
  ACTIVE_POLLING_PERIOD,
  INACTIVE_POLLING_PERIOD,
  // Active variance [meters]
  0.05,
  ACTIVE_TIMEOUT
);


// Light
const unsigned long BLINK_PERIOD = 500;
Light light(LIGHT_PIN, BLINK_PERIOD);


// Panel
Panel panel(CLOSE_ENOUGH_BUTTON_PIN, TOO_CLOSE_BUTTON_PIN);


// Settings
struct Settings {
  float close_enough_dist;
  float too_close_dist;
} settings;
FlashStorage(settings_store, Settings);


void setup() {
  Serial.begin(9600);

  // Retrieve settings from memory
  settings = settings_store.read();

  // Setup button panel
  panel.begin();
}

void loop() {
  handle_panel();

  check_dist();

  print_debug();
}

void print_debug() {
  const float dist = distance_sensor.get_dist();

  Serial.println("state, dist, close_enough_dist, too_close_dist");

  Serial.print(distance_sensor.is_active() ? "ACTIVE," : "INACTIVE,");

  Serial.print(dist);
  Serial.print(',');

  Serial.print(settings.close_enough_dist);
  Serial.print(',');

  Serial.print(settings.too_close_dist);
  Serial.println();
}

void handle_panel() {
  const float buffer = 0.05;

  const float dist = distance_sensor.get_dist();

  bool changed_settings = false;
  if (panel.close_enough_pressed()) {
    settings.close_enough_dist = dist + buffer;
    changed_settings = true;
  }
  if (panel.too_close_pressed()) {
    settings.too_close_dist = dist - buffer;
    changed_settings = true;
  }

  if (changed_settings) {
    save_settings();
  }
}

void check_dist() {
  if (!distance_sensor.is_active()) {
    light.turnOff();
    return;
  }

  float dist = distance_sensor.get_dist();

  if (dist <= settings.too_close_dist) {
    light.blink();
  } else if (dist <= settings.close_enough_dist) {
    light.turnOn();
  } else {
    light.turnOff();
  }
}

void save_settings() {
  settings_store.write(settings);
}
