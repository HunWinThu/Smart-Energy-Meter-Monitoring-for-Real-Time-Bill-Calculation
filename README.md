# Smart-Energy-Meter-Monitoring-for-Real-Time-Bill-Calculation 

Objective- Created an Internet of Things system from the ground up, consisting of device nodes capable of monitoring the electrical energy usage of various appliances linked to them and offering real-time smart metering to users via a responsive web interface and also including the billing calculation of Thailand.

Technology Stack: Arduino, ESP3 Library,LCD Display library, EEPROM library, RTC library, ThingSpeak Cloud library, etc

Source Code Explaination-

Line 1 - 7 => Import Libraries
Line 9 - 10=> Declearing variables
Line 13- 14=> Connecting to Wi-Fi
Line 15- 18=> Counting the measurement and setting the real-time clock
Line 21- 22=> Putting the API keys to connect with ThingSpeak Cloud
Line 25- 30=> Setting measurement counting delay
Line 37    => Strat working EEPRom
Line 39-55 => Setting LED Built In notification when the unit is reset to zero and idetifying the characters when the system is started.
Line 57-65 => To adjust the real-time on RTC module when the system is power off
Line 71-96 => Whenever the time passes 10s, the resultant value from voltage and current sensors are being taken max value and going to next step.
Line 98-101=> The output value from two sensors are taken to process the calculation of the power and the power value will be going to next step to process the unit 			   calculation
Line 103-111=>The measure values will be store in the EEPROM every 10s.
Line 115-143=>Billing Calculation algorithm according 163to the Thailand's Government.
Line 151-162=>Setting up the Wi-Fi connection and upload the measurement values to the ThingSpeak platform every 30s.
Line 163-180=>Configuration to upload the four values to the ThingSpeak cloud with Write API keys
Line 181-188=>Configuration of the time to reset the data stored in the EEPROM
Line 191-204=>just monitoring to see the real-time easily.
