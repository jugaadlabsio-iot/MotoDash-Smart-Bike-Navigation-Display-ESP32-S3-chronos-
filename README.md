🏍️ MotoDash — Smart Bike Navigation Display (ESP32-S3)

MotoDash is a custom-built smart bike dashboard using ESP32-S3 and a 1.69” display.
It provides real-time navigation, turn-by-turn directions, and a clean BMW-style UI.

🚀 Features
📍 Turn-by-turn navigation (via Chronos app)
⬅️ Dynamic turn arrows (left/right/straight)
📏 Distance to next turn (large display)
🛣️ Road name display (auto fallback logic)
⏱ ETA display
🎨 BMW-style centered UI
🔄 Stable BLE communication (patched Chronos library)
⚡ Crash-free performance on ESP32-S3
🛠 Hardware Used
Waveshare ESP32-S3 1.69" Display
ST7789 LCD (240x280)
Bluetooth Low Energy (BLE)
📱 Software Stack
Arduino IDE
Chronos Companion App
Custom modified ChronosESP32 library (for ESP32-S3 stability)
⚠️ Important (Custom Fixes)

This project includes a patched version of the ChronosESP32 library:

Fixes applied:
Prevent BLE crashes (Guru Meditation errors)
Safe packet parsing
Disabled unsafe BLE writes
Memory overflow protection
