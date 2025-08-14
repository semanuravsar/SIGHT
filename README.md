# SIGHT
A repository for the Ad-Hoc Comm. Project of SIGHT company.

## **Overview**  
This project implements an **autonomous multi-unit robotic system** for searching a **9×9 tiled field** to locate a designated target.  

- Each tile has an **RFID tag** encoding its coordinates and whether it contains the target.  
- The system includes multiple **Mobile Units (MUs)** and one **Base Unit (BU)**.  
- **MUs**: Search assigned regions, detect targets, share findings **with each other** via narrow-angle communication links, and converge when a target is found.  
- **BU**: Broadcasts target location to all units once detected.  

**Search Strategy – Predetermined Area Assignment**  
- The field is divided into three **3×9 regions**, one per MU.  
- Units scan their assigned regions independently.  
- Upon detecting the target, the discovering MU shares its location, prompting all units to converge.  

---

## **Video Demonstration**  
Watch the project in action here:  
[![Project Video](https://img.youtube.com/vi/MVDLl5eAIYU/0.jpg)](https://www.youtube.com/watch?v=MVDLl5eAIYU)  

---
## Poster
- [Project Poster (PDF)](SIGHT/Final%20Demo%20Poster/Sight_poster_30_05_2024_05-17.pdf)

## Final Report
[Project Final Report (PDF)](SIGHT/reports/Final-Report.pdf)

---

## **Subsystems**  

### **1. Communication Unit**  
- **Hardware:** 8 × IR transmitter/receiver pairs at 45° spacing.  
- **Operation:** Sequentially activates pairs to determine direction of incoming pings.  
- **Communication Type:** **Narrow-angle IR**, enabling targeted, directional data exchange between MUs and reducing interference.  
- **Protocol:**  
  - Custom **ping/ack** messages: 3-byte payload + 1-byte CRC.  
  - Payload includes: next position, unit ID, search state, target position.  
  - CRC ensures data integrity.  
- **Search States:** 6 defined (moving to area, searching, target not found, target found, moving to target, target reached). Ack = state + 9.  
- **Performance:** ≥95% success within ±22.5° at 0.5 m; ≥90% at 1 m; stable for 4-, 8-, and 12-byte packets at 1 m.  

---

### **2. Decision Unit**  
- Implements the **Predetermined Area Assignment** algorithm.  
- Maintains latest states/positions of other MUs to prevent collisions.  
- Assigns movement priorities based on defined search states.  
---

### **3. Sensing Unit**  
- **Method:** RFID-based localization.  
- **Hardware:**  
  - Passive **13.56 MHz RFID tags** on each tile.  
  - Target tiles: coordinate values incremented by +9.  
  - Each MU has an RFID reader mounted underneath.  
- **Operation:**  
  - Reads tags in milliseconds while moving at 10 cm/s (no stop needed).  
  - On read, MU pauses 500 ms to process.  

---

### **4. Motion Unit**  
- **Submodules:** Mechanical, driving, and feedback.  
- **Driving:**  
  - **L293N H-bridge** motor driver.  
  - Direction via IN pins, speed via PWM on EN pins.  
  - Gradual PWM ramp to avoid surges and resets.  
- **Feedback & Navigation:**  
  - Line follower on black-on-white grid.  
  - Ensures accurate RFID positioning.  
- **Recovery:** Back-and-forth routine if tag read is missed.  

---

### **5. Power Unit**  
- **Consumption:** ~4.6 W average (motors ~3.45 W).  
- **Supply:** Two 18650 Li-ion cells in series (7.4 V, 3.5 A peak).  
- **Runtime:** >5 hours at worst-case 7.5 W.  
- **Regulation:** Linear 5 V; remove batteries for charging.  

---

## **System Workflow**  
1. MUs start at the base and move toward their assigned regions.  
2. Each MU scans tiles, reading RFID tags to determine position and target status.  
3. MUs exchange status and target information with each other via **narrow-angle IR communication**.  
4. If a target is detected, the discovering MU broadcasts location, and all MUs converge on it.  

---
