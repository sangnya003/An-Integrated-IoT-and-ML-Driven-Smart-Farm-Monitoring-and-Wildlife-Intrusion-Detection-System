IoT and ML Based Smart Farm Monitoring & Wildlife Intrusion Detection

This project uses a combination of IoT hardware and a CNN-based machine learning model to automate irrigation and detect wildlife entering farmland. The goal is to reduce crop damage and help farmers monitor field conditions without being physically present all the time.

This system helps farmers by detecting animals early to prevent crop damage, automating irrigation based on soil moisture to save water, and reducing the need for constant manual monitoring. It also allows farmers to view live field data and control the pump remotely through the Blynk app, making daily work easier and more efficient. The main advantages include water conservation, improved crop safety, less labour, and affordable hardware, while the drawbacks are its dependence on internet/SMS networks, reduced accuracy in low-light conditions, and the need for occasional sensor maintenance and proper camera placement.

Project Overview
The system has two major parts:

1. Wildlife Intrusion Detection
A CNN model is trained to identify animals such as wild boar, monkeys, and elephants.
When an intrusion is detected:
A relay triggers a sound alarm
An SMS alert is sent to the farmer
The event can be logged for reference
The model files (best_model.h5 and balanced_model.h5) are used by the Python script for prediction.

2. Smart Irrigation
Soil moisture and humidity sensors continuously monitor field conditions.

If the soil becomes too dry:
The water pump is switched on automatically using a relay
Pump turns off once moisture reaches a safe level
Farmers can also see live data through the Blynk IoT app and manually control the pump if needed.

Technologies Used

Python (for the ML model)
TensorFlow/Keras
OpenCV
Arduino IDE (C++ for ESP32/NodeMCU)
Blynk IoT App
Soil moisture and humidity sensors
Two-channel relay module
Transformer
Regulated Power supply
LCD
ESP8266
Water pump
USB to Serial Converter
LM393
Speaker

File Structure

datasets/
smart_farm_monitoring.ino
static/
templates/
app1.py
train_model.py
best_model.h5
balanced_model.h5
README.md

How the System Works

Animal Detection:
Images from the camera are processed by the CNN model. If an animal is detected, the alarm is activated and an SMS is sent to the farmer.

Irrigation:
Sensor readings decide whether the pump needs to be turned on. All data is updated on Blynk for remote monitoring.

Running the Project

Machine Learning:
python train_model.py
python app1.py

IoT Part:
Upload smart_farm_monitoring.ino using Arduino IDE after setting WiFi and Blynk credentials.
