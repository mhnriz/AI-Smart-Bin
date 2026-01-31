# **AI Smart Bin: Automated Waste Classifier**
<div align="center">

![Last Commit](https://img.shields.io/github/last-commit/mhnriz/ai-smart-bin?style=for-the-badge&color=3498DB)
![Repo Size](https://img.shields.io/github/repo-size/mhnriz/ai-smart-bin?style=for-the-badge&color=2ECC71)
<br />
![Platform](https://img.shields.io/badge/Platform-ESP32--CAM-E74C3C?style=for-the-badge)
![Language](https://img.shields.io/badge/Language-C++%20%26%20Python-3498DB?style=for-the-badge)
![Framework](https://img.shields.io/badge/Framework-Flask%20%26%20YOLOv8-FFCC00?style=for-the-badge)

</div>

## 📖 **Overview**

The **AI Smart Bin** is an IoT-based waste management system developed during a Hackathon x Entrepreneurship Camp 2025 (June 2025). It leverages computer vision to identify waste types and automate the sorting process. 

The system uses an **ESP32-CAM** to capture images, which are then sent to a **Flask-based Python server**. The server performs inference using a **YOLOv8** model and sends a signal back to the hardware to trigger a sorting mechanism (Servo Motor).

## ⚙️ **Features**

- 📸 **Edge Capture**: Real-time image acquisition via ESP32-CAM.
- 🧠 **AI Detection**: YOLOv8 model trained to detect specific waste classes (e.g., Metal).
- 📡 **Wireless Integration**: Seamless HTTP communication between hardware and backend.
- 🦾 **Automated Sorting**: 180° Servo motor control based on AI classification.
- 📊 **Training Pipeline**: Includes scripts for initial training and resuming model training.

## 🧩 **Project Structure**

```
ai-smart-bin/
├── firmware/                # ESP32-CAM Arduino Source Code
│   ├── firmware.ino         # Main logic (WiFi, Camera, HTTP)
│   ├── camera_pins.h        # AI-Thinker Pin Mapping
├── server/                  # Python Backend
│   ├── app.py               # Flask Inference Server
│   └── models/              # Saved YOLOv8 .pt files
├── training/                # AI Development
│   ├── train.py             # YOLOv8 Training Script
├── README.md                # Project Documentation
└── requirements.txt         # Python Dependencies
```
## 🔧 **Hardware Requirements**

| Component | Specification |
|----------|-------|
| **MCU** | ESP32-CAM (AI-Thinker) |
| **Actuator** | MG90S Servo Motor |
| **Power** | 5V 2A Power Supply |
| **Connectivity** | 2.4GHz WiFi |

## 🚀 **Getting Started**

### 1. Server Setup
```bash
pip install -r requirements.txt
python server/app.py
```
### 2. Firmware Configuration
1. Open firmware/firmware.ino in Arduino IDE.
2. Update ssid, password, and server IP.
3. Upload to ESP32-CAM.



## 🛠 **Technologies Used**

- **Embedded**: C++, Arduino ESP32 Core.
- **AI/ML**: Ultralytics YOLOv8, PyTorch.
- **Backend**: Flask (Python).
- **Computer Vision**: OpenCV.

## 📜 **Hackathon Timeline**
This project was developed between **June 13 – June 16, 2025**.
- **Day 1**: Hardware initialization & Camera driver setup.
- **Day 2**: Dataset preparation & YOLOv8 Model Training.
- **Day 3**: Integration of Flask API & Servo sorting logic.
- **Day 4**: Final testing & Documentation.
