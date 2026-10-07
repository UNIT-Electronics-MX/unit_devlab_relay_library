# DevLab_Relay

Arduino library for the I2C relay module using the DevLab Device Protocol
(DDP) over I2C.

The module exposes relay on, off and toggle commands and reports the relay
state back. This library wraps the shared
[`DevLabDDP`](https://github.com/UNIT-Electronics-MX/unit_devlab_ddp_library)
master and the
[`DevLab_Interface`](https://github.com/UNIT-Electronics-MX/unit_devlab_interface_library)
`DevLab_I2C_Orchestrator` bus class into a single `DevLab_Relay` object.

Compatible with ESP32, RP2040/RP2350, STM32, AVR and Arduino-compatible platforms.

---

# Features

- Device identification (Device ID `0x0108`) before any command
- `on()`, `off()`, `toggle()` and `set(bool)` with the resulting state returned
- `readState()` to query the relay at any time
- I2C address scan and reassignment (`0x08` to `0x77`), default address `0x28`
- Bus recovery (`beginRecovered`) for a slave left mid-transaction
- Supports custom I2C pins and clock speed

---

# Supported Interfaces

| Interface | Support Status |
|---|---|
| I2C | Supported |

---

# Installation

1. Open Arduino IDE
2. Go to `Sketch -> Library Manager -> Search DevLab_Relay...`
3. Click on Install (the `DevLabDDP` and `DevLab_Interface` dependencies are
   installed with it; otherwise install them manually)
4. Compile and upload the examples

---

# Quick Start Example

```cpp
#include <Arduino.h>
#include <Wire.h>
#include <DevLab_Relay.h>

DevLab_Relay relay(Wire, DevLab_Relay::DEFAULT_ADDRESS, 400000);

void setup() {
  Serial.begin(115200);
  // ESP32: 6/7. RP2040/RP2350: 24/25 (Pulsar) or 12/13.
  if (!relay.beginRecovered(6, 7)) {
    Serial.println("Relay module not found");
    return;
  }
  relay.on();
}

void loop() {
  bool state;
  if (relay.toggle(&state)) {
    Serial.println(state ? "ON" : "OFF");
  }
  delay(2000);
}
```

---

# API

| Method | Description |
|---|---|
| `DevLab_Relay(wire, address, clock)` | Create the object (default `Wire`, `0x28`, 400 kHz). |
| `begin()` / `begin(sda, scl, clock)` | Start the bus and verify the device. |
| `beginRecovered(sda, scl, timeoutUs, restart)` | Same, clearing a stuck bus first. |
| `on(&state)` / `off(&state)` / `toggle(&state)` | Send the command; `state` is optional. |
| `set(enabled, &state)` | `on` or `off` depending on `enabled`. |
| `readState(state)` | Read the current relay state (`true` = on). |
| `isConnected()` / `busReady()` | Result of the last `begin`. |
| `deviceInfo()` / `printInfo(out)` | Identity reported by the module. |
| `protocol()` / `bus()` | Access the underlying DDP master and I2C bus. |

All commands return `false` on an I2C error or if `begin` has not verified the
device.

---

# License

MIT, see [LICENSE](LICENSE).
