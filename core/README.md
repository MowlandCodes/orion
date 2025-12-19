# ⚽ Project Orion: High-Level Robot Core ("The Brain")

> **Role:** Decision Making, Computer Vision, & Strategy  
> **Platform:** Raspberry Pi 5 (Ubuntu/Debian Bookworm)  
> **Target Hardware:** 3-Omni Wheel Robot with ESP32 Low-Level Controller

## 🧠 System Architecture

This Core module acts as the **Cortex** of the robot. It processes visual data, calculates navigation vectors relative to the field (Field Oriented Control), and sends kinematic commands to the spinal cord (ESP32).

**Data Flow:**
1.  **SENSE:** Camera 1 (Front/Webcam) & Camera 2 (Top/Fisheye) capture frames.
2.  **PERCEIVE:** `vision.py` processes frames using HSV Thresholding (MVP) or YOLO (Future).
3.  **ORIENT:** `compass.py` reads heading data from ESP32/HMC5883L to determine field orientation.
4.  **DECIDE:** `main_brain.py` executes a Finite State Machine (Search -> Chase -> Orbit -> Shoot).
5.  **ACT:** `comms.py` packs velocity vectors (`Vx`, `Vy`, `Omega`) into Byte Structs and transmits via Serial (USB).

---

## 📂 Project Structure

```bash
core/
├── main_brain.py       # Main entry point (Finite State Machine)
├── vision.py           # Object detection & distance calculation logic
├── comms.py            # Serial communication & Byte packing (The "Translator")
├── tuner.py            # GUI Tool for real-time HSV calibration
├── config.py           # Global constants (PID gains, Serial Ports, Resolutions)
├── utils.py            # Math helpers (Rotation matrices, coordinate transforms)
└── requirements.txt    # Python dependencies

```

---

## 🛠️ Hardware Prerequisites

* **Compute:** Raspberry Pi 5 (Active Cooler Required).
* **Vision:**
* **Cam 1 (Front):** High FPS USB Webcam (for ball locking & dribbling).
* **Cam 2 (Omni):** Fisheye/Mirror (for 360° situational awareness).


* **Power:** Dedicated 5V/3A+ via USB-C (Isolated from motor power).
* **Interface:** USB Serial connection to ESP32.

---

## 🚀 Setup & Installation

**1. System Dependencies**
Ensure your Pi 5 is up to date and has system-level libraries:

```bash
sudo apt update && sudo apt upgrade -y
sudo apt install libopencv-dev python3-opencv

```

**2. Virtual Environment (Mandatory on Pi 5)**
Don't break the system python. Use a venv.

```bash
cd ~/robot_soccer/core
python3 -m venv venv
source venv/bin/activate

```

**3. Install Python Packages**

```bash
pip install -r requirements.txt

```

*(Content of requirements.txt: `opencv-python`, `numpy`, `pyserial`)*

---

## 🎛️ Usage Guide

### Phase 1: Calibration (Do this FIRST at every new venue)

Lighting conditions change. Hardcoded values fail. Use the tuner.

```bash
python tuner.py

```

* **Goal:** Adjust sliders until the ball is **Solid White** and the background is **Solid Black**.
* **Action:** Save the `Lower` and `Upper` HSV values into `config.py`.

### Phase 2: Autonomous Mode

Run the main brain. Ensure ESP32 is connected via USB.

```bash
python main_brain.py

```

### Phase 3: Diagnostic/Debug

To test camera feeds without running logic:

```bash
python vision.py --debug

```

---

## 📡 Communication Protocol (The Contract)

We communicate with the ESP32 using a strict **14-Byte Packet Structure** (Little Endian).

**Pi 5 -> ESP32 (Command Packet):**
| Byte Index | Data Type | Description |
| :--- | :--- | :--- |
| 0 | `0xA5` | Header / Sync Byte |
| 1-4 | `float` | **Vx** (Maju/Mundur in m/s) |
| 5-8 | `float` | **Vy** (Geser Kiri/Kanan in m/s) |
| 9-12 | `float` | **Omega** (Putar CCW in rad/s) |
| 13 | `uint8` | Checksum (XOR of bytes 1-12) |

**ESP32 -> Pi 5 (Feedback Packet - Optional):**
| Byte Index | Data Type | Description |
| :--- | :--- | :--- |
| 0 | `0x5A` | Header |
| 1-4 | `float` | **Heading** (Compass 0-360°) |
| 5 | `uint8` | IR Sensor Status (Ball possession) |

---

## 🤖 Logic: Finite State Machine

The robot operates on the following priority logic:

1. **STATE: SEARCH**
* *Condition:* No ball detected by any camera.
* *Action:* Rotate in place (Scan). Use Fisheye as radar.


2. **STATE: CHASE**
* *Condition:* Ball detected (Distance > 30cm).
* *Action:* PID Control to center ball in frame + Forward velocity.


3. **STATE: ORBIT/DRIBBLE**
* *Condition:* Ball detected (Distance < 30cm).
* *Action:* Reduce speed, engage dribbler motor, align for shoot.



---

## 🔮 Future Roadmap

* [ ] **YOLOv8 Nano Integration:** Replace HSV with Deep Learning for robust detection in dynamic lighting.
* [ ] **Kalman Filter:** Implement tracking to predict ball trajectory when occluded.
* [ ] **Reinforcement Learning (PPO):** Train a policy for optimal striker behavior in simulation (PyBullet) then Sim2Real.

