/**
 * @file    relayControl.ino
 * @brief   Basic control of the DevLab I2C relay module: ON, OFF and TOGGLE
 *          every 2 seconds, reading back and printing the relay state after
 *          each command over Serial (115200 baud).
 * @author  Jonathan Mejorado
 * @note    Supported boards: ESP32, RP2040/RP2350, STM32 and AVR.
 */

#include <Arduino.h>
#include <Wire.h>
#include <DevLab_Relay.h>

#if defined(ARDUINO_ARCH_RP2040)
  /* Both pin pairs below use I2C0, exposed as Wire by these variants. */
  #if defined(PIN_WIRE1_SDA) && defined(PIN_WIRE1_SCL) && \
      PIN_WIRE1_SDA == 24U && PIN_WIRE1_SCL == 25U
    /* Pulsar connector pins. Core 3.1.0 incorrectly assigns them to Wire1. */
    constexpr uint8_t I2C_SDA = 24U, I2C_SCL = 25U;
  #else
    /* Generic RP2040/RP2350 wiring. */
    constexpr uint8_t I2C_SDA = 12U, I2C_SCL = 13U;
  #endif
  constexpr uint32_t I2C_CLOCK_HZ = 100000U;
#elif defined(ARDUINO_ARCH_ESP32)
  constexpr uint8_t I2C_SDA = 6U, I2C_SCL = 7U;
  constexpr uint32_t I2C_CLOCK_HZ = 400000U;
#elif defined(ARDUINO_ARCH_STM32)
  // STM32duino: default I2C pins of the selected board.
  constexpr uint8_t I2C_SDA = SDA, I2C_SCL = SCL;
  constexpr uint32_t I2C_CLOCK_HZ = 400000U;
#elif defined(ARDUINO_ARCH_AVR)
  // AVR has fixed I2C pins (Uno/Nano: A4/A5, Mega: 20/21, Leonardo: 2/3);
  // SDA/SCL come from the board variant and begin() ignores the pin numbers.
  constexpr uint8_t I2C_SDA = SDA, I2C_SCL = SCL;
  constexpr uint32_t I2C_CLOCK_HZ = 100000U;  // 400 kHz falla con el level shifter en UNO
#else
  #error "Use ESP32, RP2040/RP2350, STM32 or AVR"
#endif

constexpr uint32_t SWITCH_INTERVAL_MS = 2000U;

DevLab_Relay relay(Wire, DevLab_Relay::DEFAULT_ADDRESS, I2C_CLOCK_HZ);

void report(const char *action) {
  bool state = false;
  Serial.print(action);
  if (!relay.readState(state)) {
    Serial.println(": ERROR reading relay state");
    return;
  }
  Serial.print(": relay is ");
  Serial.println(state ? "ON" : "OFF");
}

void setup() {
  Serial.begin(115200);
  delay(500);

  if (!relay.beginRecovered(I2C_SDA, I2C_SCL, 20000, false)) {
    Serial.println("Relay module not found; check wiring, power and address");
    return;
  }
  relay.printInfo(Serial);

  relay.off();
  report("Initial state");
}

void loop() {
  if (!relay.isConnected()) return;

  relay.on();
  report("ON");
  delay(SWITCH_INTERVAL_MS);

  relay.off();
  report("OFF");
  delay(SWITCH_INTERVAL_MS);

  relay.toggle();
  report("TOGGLE");
  delay(SWITCH_INTERVAL_MS);

  relay.off();
  report("OFF");
  delay(SWITCH_INTERVAL_MS);
}
