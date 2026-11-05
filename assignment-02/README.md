# Advanced Networking — Assignment 2

Solutions and project files for the three tasks in FAU Advanced Networking Assignment 2.

## Repository layout

```text
AdN-Assignment-2/
├── task1-ip-display/
│   ├── src/main.cpp
│   └── platformio.ini
├── task2-lux-socket/
│   ├── src/main.cpp
│   └── platformio.ini
├── task3-port-forwarding/
│   ├── README.md
│   └── scripts/port-forward.sh
├── .gitignore
└── README.md
```

## Hardware and environment

- Board: WEMOS/LOLIN D1 Mini ESP32 (`wemos_d1_mini32` PlatformIO board ID).
- Task 1: OLED is not attached; the assigned IP address is printed to the Serial Monitor.
- Task 2: ESP32 with a TSL2561 illuminance sensor and the course smart socket.
- Task 3: macOS/Linux OpenSSH workflow.

## Build and upload

Open the relevant task folder as a PlatformIO project in VS Code:

```bash
pio run
pio run --target upload
pio device monitor --baud 115200
```

## Task 2 configuration note

The course assignment describes the required illuminance-based ON/OFF logic but does not provide the smart socket's HTTP endpoint or request format. Replace `SOCKET_ON_URL` and `SOCKET_OFF_URL` directly in `task2-lux-socket/src/main.cpp` with the actual endpoints provided for your socket. The code uses explicit ON/OFF requests, not a TOGGLE command.

The example thresholds (100 lx / 300 lx) are starting values. Measure the environment and adjust them if necessary.

## Git

From the repository root:

```bash
git init
git add .
git commit -m "Add AdN Assignment 2 solutions"
```

See each task folder for task-specific instructions.
