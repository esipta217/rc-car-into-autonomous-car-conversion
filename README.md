# rc-car-into-autonomous-car-conversion
# Autonomous RC Car with Steering-Based Navigation

## Project Overview
This project converts a commercially available RC car chassis into an autonomous obstacle avoidance vehicle using Arduino Uno and sensor-based navigation.

Unlike conventional robotic vehicles that use differential drive, this RC chassis uses a **car-like steering system**:

- One motor controls forward and reverse motion  
- One motor controls front wheel steering  
- L298N motor driver controls both motors  
- Ultrasonic sensor mounted on a servo performs environmental scanning  
- Rear IR sensors detect obstacles during reversing  

---

## Engineering Challenges

### RC Chassis Analysis
The chassis was originally designed for manual remote control. Understanding the internal motor layout and steering mechanism was necessary before implementing autonomous control.

### High-Speed Motion Control
The drive motor produced high speed and unstable turns. To solve this:

- Pulse-based motion control was implemented  
- Delay-based braking ensures complete stopping  
- PWM speed control is handled through the motor driver  

### Motor Driver Integration
The L298N motor driver acts as an interface between Arduino and motors. It enables:

- Bidirectional motor control  
- Speed control using PWM  
- Stable current supply for motors  

### Environmental Awareness
- Servo-mounted ultrasonic sensor scans surroundings  
- Rear IR sensors prevent unsafe reversing  
- Multiple sensors improve navigation accuracy  

---

## Hardware Components

- Arduino Uno  
- L298N Motor Driver  
- RC Car Chassis  
- DC Drive Motor  
- Steering Motor  
- Ultrasonic Sensor (HC-SR04)  
- Servo Motor  
- IR Sensors (Rear Left and Right)  
- 18650 Li-ion Battery Pack  

---

## Pin Configuration

### Drive Motor
- ENA → Pin 10  
- IN1 → Pin 3  
- IN2 → Pin 4  

### Steering Motor
- ENB → Pin 11  
- IN3 → Pin 5  
- IN4 → Pin 6  

### Sensors
- Ultrasonic TRIG → Pin 9  
- Ultrasonic ECHO → Pin 12  
- Servo Motor → Pin 13  
- IR Left → Pin 7  
- IR Right → Pin 8  

---

## System Working Principle

### Forward Navigation
The vehicle moves forward using pulse-controlled motor signals while the ultrasonic sensor scans surroundings using servo rotation.

### Front Obstacle Detection
When an object is detected within 20 cm:

1. Vehicle stops  
2. Servo scans left, center, and right  
3. Vehicle reverses safely  
4. Steering motor turns vehicle toward the clearer path  

### Rear Obstacle Monitoring
Rear IR sensors detect obstacles while reversing. If both sensors detect blockage, reversing is restricted.

### Dual Obstacle Condition
If both front and rear obstacles exist, ultrasonic scanning determines the safest turning direction.

---

## Control Strategy

- PWM speed control using L298N motor driver  
- Pulse-based motion stabilization  
- Servo-based directional scanning  
- Multi-sensor fusion decision making  

---

## Hardware Build

![Assembling the parts](images/car_build.jpeg)

---

## How To Run

1. Assemble hardware using pin configuration  
2. Upload Arduino code using Arduino IDE  
3. Power the system using battery pack  
4. Vehicle starts navigation after initialization delay  

---

## Serial Monitoring

The system outputs real-time debugging data including:

- Distance readings  
- IR sensor status  
- Navigation decisions  

---

## Future Improvements

- Computer vision-based navigation  
- SLAM mapping  
- PID steering stabilization  
- Wireless telemetry monitoring  
- Autonomous path planning  

---

## Learning Outcomes

- Embedded systems programming  
- Motor driver interfacing  
- Sensor fusion implementation  
- Autonomous navigation design  
- Real-world robotics troubleshooting  

