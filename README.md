 Semi-Autonomous Trash Collecting Rover

An ESP32-based semi-autonomous rover designed to detect lightweight trash, avoid obstacles, and collect detected waste using a servo-operated collection mechanism.

The system combines sensor-based autonomous movement with Wi-Fi-based manual control and LCD status feedback.

---

## Project Overview

EcoBot is an ESP32-powered robotic rover developed as a prototype for automated trash collection.

The rover uses:

* **IR sensor** for trash detection
* **Ultrasonic sensor** for obstacle detection
* **Servo motor** for the trash collection mechanism
* **DC motors** for rover movement
* **16×2 I2C LCD** for system status
* **ESP32 Wi-Fi** for remote manual control

The main controller continuously processes sensor inputs and controls the rover's motors and collection mechanism accordingly.

---

## Features

* Semi-autonomous rover movement
* IR-based trash detection
* Ultrasonic obstacle detection
* Automatic obstacle avoidance
* Servo-based trash collection
* 16×2 LCD status display
* Wi-Fi-based manual control
* Autonomous mode ON/OFF control
* ESP32-based central control

---

## System Architecture

![ Block Diagram](docs/block_diagram.png)

### Main Components

| Component         | Function                     |
| ----------------- | ---------------------------- |
| ESP32 WROOM       | Main controller              |
| IR Sensor         | Trash detection              |
| Ultrasonic Sensor | Obstacle detection           |
| Servo Motor       | Trash collection mechanism   |
| DC Motors         | Rover movement               |
| Motor Driver      | Drives the DC motors         |
| 16×2 I2C LCD      | Displays system status       |
| Power Supply      | Provides power to the system |

---

## How It Works

![Flowchart](docs/flow_chart.png)

### Autonomous Operation

1. The ESP32 initializes the sensors, motors, servo and LCD.
2. The LCD displays the system status.
3. The rover starts moving forward.
4. The ultrasonic sensor checks for obstacles.
5. If an obstacle is detected within the defined distance, the rover stops and changes direction.
6. If no obstacle is detected, the IR sensor checks for trash.
7. When trash is detected, the rover stops.
8. The servo-operated collection mechanism is activated.
9. The LCD displays the collection status.
10. After the collection sequence, autonomous operation continues.

### Manual Operation

The ESP32 provides a Wi-Fi web interface that allows the user to control the rover remotely.

Available controls:

* Forward
* Backward
* Left
* Right
* Stop
* Autonomous Mode ON
* Autonomous Mode OFF

---

## Sensor Logic

### IR Sensor

The IR sensor is configured as **active LOW**.

A LOW signal is interpreted by the ESP32 as trash detection.

### Ultrasonic Sensor

The ultrasonic sensor measures the distance between the rover and an obstacle.

The current software uses:

**Obstacle threshold: 10 cm**

If an object is detected closer than this threshold, the rover stops and performs an obstacle-avoidance turn.

---

## Trash Collection Mechanism

When the IR sensor detects trash:

```text
Trash Detected
      ↓
Rover Stops
      ↓
Servo Activated
      ↓
Collection Sequence
      ↓
Trash Collected
```

The servo performs a predefined movement sequence to operate the collection mechanism.

---

## LCD Feedback

The 16×2 I2C LCD provides real-time system status messages such as:

* `ESP32 WROOM`
* `Initializing`
* `Connecting WiFi`
* `WiFi Connected`
* `Searching Trash`
* `Auto Mode ON`
* `Obstacle Ahead`
* `Avoiding...`
* `Trash Detected`
* `Collecting...`
* `Trash Collected`
* `MANUAL MODE`

---

## Hardware Pin Configuration

| Component       | ESP32 Pin |
| --------------- | --------: |
| Left Motor IN1  |   GPIO 27 |
| Left Motor IN2  |   GPIO 14 |
| Right Motor IN1 |   GPIO 12 |
| Right Motor IN2 |   GPIO 13 |
| Ultrasonic TRIG |   GPIO 33 |
| Ultrasonic ECHO |   GPIO 25 |
| IR Sensor       |   GPIO 34 |
| Servo Signal    |   GPIO 26 |
| LCD SDA         |   GPIO 21 |
| LCD SCL         |   GPIO 22 |

---

## Project Wiring

![EcoBot Wiring Diagram](connection-diagram.png)

---

## Software

The rover firmware is written for the **ESP32** using the Arduino framework.

### Main Libraries

* WiFi
* WebServer
* ESP32Servo
* Wire
* LiquidCrystal_I2C

The complete firmware is available here:

[`src/EcoBot.ino`](src/rover.ino)

---

## Wi-Fi Control

The ESP32 hosts a simple web interface for controlling the rover.

The interface provides:

| Command  | Function                |
| -------- | ----------------------- |
| FORWARD  | Move forward            |
| BACK     | Move backward           |
| LEFT     | Turn left               |
| RIGHT    | Turn right              |
| STOP     | Stop the rover          |
| AUTO ON  | Enable autonomous mode  |
| AUTO OFF | Disable autonomous mode |

After connecting to Wi-Fi, the ESP32 displays its local IP address on the LCD.

The user can enter this IP address in a browser connected to the same network to access the control interface.

---

## Project Status

| Feature                       | Status   |
| ----------------------------- | -------- |
| ESP32 control                 | Complete |
| DC motor control              | Complete |
| Ultrasonic obstacle detection | Complete |
| IR trash detection            | Complete |
| Obstacle avoidance            | Complete |
| Servo collection mechanism    | Complete |
| LCD status display            | Complete |
| Wi-Fi manual control          | Complete |
| Autonomous mode               | Complete |

---

## Prototype

Project photographs and demonstration media are available in the [`media`](media/) folder.

---

## Future Scope

The current prototype can be further developed through:

* Camera-based trash identification
* IoT-based system monitoring
* Enhanced mechanical design for larger waste collection
* Solar-powered operation

These features are considered future improvements and are not part of the current prototype.

---

## License

This project is intended for educational and project demonstration purposes.
