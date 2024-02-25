## Guitar Tuner on Teensy 4.0 - Signal Processing Project

### Overview
This project aims to develop a guitar tuner using signal processing techniques implemented in C++ on a Teensy 4.0 with its audio-shield. The tuner will analyze the input audio signal from a guitar and provide real-time feedback on the pitch accuracy.

### Features
- Real-time audio input processing
- Fast and accurate pitch detection
- User-friendly interface for visualizing detected pitch

### Installation
1. Download the pitch_tracking.ino file in the code directory from GitHub: [https://github.com/Herisson-kun/SON/].
2. Navigate to the code directory and open the pitch_tracking.ino file.
3. Transfer it to the teensy.(beware to configure correctly the port and card in the Arduino IDE)

### Usage
1. Play a string on your guitar.
2. The tuner will analyze the input signal and display if the detected pitch is higher or lower than the correct frequency.
3. Adjust the tuning of your guitar based on the feedback provided by the tuner.

### Configuration
Adjust the threshold around the correct frequency to adjust the accuracy.
