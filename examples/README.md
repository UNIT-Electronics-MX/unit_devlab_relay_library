# DevLab_Relay examples

| Example | Purpose |
|---|---|
| `relay/relayControl` | Switch the relay on, off and toggle it, reading the state back after each command. |
| `i2c/changeAddress` | Scan the bus and change the I2C address of a relay module (default `0x28`). |

All examples verify Device ID `0x0108` before sending any command.
