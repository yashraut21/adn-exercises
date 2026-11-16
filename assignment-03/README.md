# AdN Assignment 3

Minimal PlatformIO project for Part 3: toggle the MQTT light switch when the ESP32 button is pressed.

## Setup

1. Open this folder in VS Code with PlatformIO.
2. In `src/main.cpp`, replace `REPLACE_WITH_SWITCH_ID` with the switch identifier you discovered in Part 1.
3. Check the MQTT broker address. The Exercise 3 slides show `192.168.0.254`; Part 1 uses `192.168.0.1` for the command-line broker. Change `MQTT_HOST` if your lab uses a different address.
4. Build and upload to the ESP32.
5. Open Serial Monitor at 115200 baud and press the button (GPIO 17).

The sketch connects to WiFi, connects to MQTT, and publishes `TOGGLE` to `cmnd/<switch-id>/POWER` when the button is pressed.

## Part 1 commands

Subscribe to all topics:

```bash
mosquitto_sub -h 192.168.0.1 -t '#' -v
```

Query switch status:

```bash
mosquitto_sub -h 192.168.0.1 -t 'stat/#' -v
mosquitto_pub -h 192.168.0.1 -t 'cmnd/<switch-id>/POWER' -n
```

Turn the light on:

```bash
mosquitto_pub -h 192.168.0.1 -t 'cmnd/<switch-id>/POWER' -m 'ON'
```

## Part 2 QoS results

| Subscription QoS | Publication QoS | Received QoS |
|---:|---:|---:|
| 0 | 0 | 0 |
| 0 | 2 | 0 |
| 2 | 0 | 0 |
| 2 | 2 | 2 |

Publisher-to-broker packet types:

- QoS 0: PUBLISH
- QoS 1: PUBLISH, PUBACK
- QoS 2: PUBLISH, PUBREC, PUBREL, PUBCOMP
