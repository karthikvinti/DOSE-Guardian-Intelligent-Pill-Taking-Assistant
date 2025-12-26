# 💊 DoseGuardian – Intelligent Pill-Taking Assistant

DoseGuardian is an embedded-system–based smart medication reminder designed to help patients take medicines on time. It is built using the LPC2148 (ARM7) microcontroller and provides real-time monitoring, alerts, and user acknowledgment to reduce missed doses.

## 📌 Project 

Many patients, especially elderly individuals and those under long-term treatment, often forget or skip their medication. DoseGuardian addresses this issue by integrating a Real-Time Clock (RTC) with visual and audible alert mechanisms to ensure timely medicine intake.

## 🎯 Objective

- Display real-time date and time using RTC  
- Allow users to edit RTC information using a 4×4 keypad  
- Enable medicine reminder scheduling  
- Trigger alerts when medicine time matches current time  
- Confirm whether medicine is taken or missed  

## 🧠Working Principle

1️⃣Setting the Medicine Schedule: The user presses Switch-1 and enters the medicine time using the keypad. The schedule is stored in the microcontroller and displayed on the LCD along with RTC     information.

2️⃣Real-Time Monitoring: The microcontroller continuously checks the RTC time and compares it with the stored medicine schedule.

3️⃣Alert Mechanism: When the current time matches the medicine time, the LCD displays “TIME FOR MEDICINE” and the buzzer turns ON at fixed intervals.

4️⃣User Acknowledgment: The user presses Switch-2 to confirm medicine intake. If the switch is not pressed within a defined time, the LED turns ON and the LCD displays “MEDICINE NOT TAKEN”.

## 🧩Block Diagram (Logical View)

RTC, Keypad, and Switches provide inputs to the LPC2148 microcontroller. The controller drives the LCD, Buzzer, and LED for output indications.

## 🛠️Hardware Requirements

- LPC2148 Microcontroller  
- 16×2 LCD Display  
- 4×4 Matrix Keypad  
- Buzzer  
- Push Buttons (Switch-1 and Switch-2)  
- LED  
- USB-UART Converter or DB-9 Cable  

## 💻Software Requirements

- Embedded C  
- Keil µVision IDE  
- Flash Magic  

## ✨Features

- 🕰️Real-time clock display  
- ✏️Time, date, and day editing  
- ⏰Medicine reminder scheduling  
- 🔔Audible alerts using buzzer  
- 📺Visual alerts using LCD and LED  
- ☑️Confirmation-based medicine intake  
- ⚠️Missed-dose indication  

## 📸LCD Display 
### 🖥️ Full Proteus Setup
<img width="1282" height="905" alt="image" src="https://github.com/user-attachments/assets/418937d8-673c-49d0-a0d0-2fbfb58ca6c0" />

### ⌚ Main Menu 
<img width="857" height="350" alt="image" src="https://github.com/user-attachments/assets/3d92371f-d09a-4b0c-a3d7-d1415a804342" />

### ✏️ Editing Menu (Time Configuration)
> **Right ( > )** → Move between HH • MM • SS  
>  
> **Left ( < )** → Move backward  
>  
> **↑** → Increase value  
>  
> **↓** → Decrease value
<img width="1278" height="520" alt="image" src="https://github.com/user-attachments/assets/217174b3-7a57-4635-b7b9-48f7659a0b6f" />

## ⚠️ Known Issues
- Setting ON/OFF Time at **00:00** behaves differently:  
  - As **Day Start** for ON Time  
  - As **Day End** for OFF Time  

- Year field supports **1–4095**, but only **2025** is accepted for correct operation  
  (Warning message alerts the user during editing).

## 🔑Keypad Controls

| Key | Function |
|-----|----------|
| A   | Move selection forward |
| B   | Move selection backward |
| C   | Decrement value |
| D   | Increment value |
| =   | Save / Confirm |

## 🧭 User Guide

1. Power on the system — the LCD shows **Date & Time**.  
2. Press the configured **menu switch** to enter settings.  
3. Use the keypad to edit:  
   - RTC Time (HH:MM:SS)  
   - RTC Date (DD/MM/YYYY)  
   - Device ON Time  
   - Device OFF Time  
4. Navigation controls:  
   - **“>”** → Move Right  
   - **“<”** → Move Left  
   - **↑** → Increment  
   - **↓** → Decrement  
5. Confirm inputs — invalid entries trigger warnings.  
6. Exit to return to normal running mode with updated parameters.

## 💡Future Enhancements

- Multiple medicine schedules  
- EEPROM or Flash memory storage  
- GSM or SMS alert system  
- Mobile application integration  
- Voice reminders  
- Low-power sleep mode  

## 👤Developed By

**Karthik Vinti** 
Electronics and Communication Engineering
Developer of *DoseGuardian – Intelligent Pill-Taking Assistant*

## 📜License

This project is developed for academic and learning purposes. You are free to modify and enhance it with proper credit.

⭐ If you find this project useful, consider giving it a star on GitHub.
