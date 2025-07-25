# Remote Thermometer Prototype: Raspberry Pi Pico + DS18B20 + nRF24L01+

This project implements a **remote temperature monitoring system** using two Raspberry Pi Pico boards:

- **Sensor Node**: Pico A with up to two DS18B20 sensors and an nRF24L01+ radio module.  
- **Base Station**: Pico B (to be extended later to Pico W) receiving temperature readings over nRF24 and forwarding data over USB serial.

---

## Current State

- **Sensor Node** (`src/sensor_node.c`):  
  - Reads up to two DS18B20 sensors on GPIO pins GP4 and GP5 via FreeRTOS tasks.  
  - Packages each temperature reading into a simple payload (sensor ID + raw value).  
  - Transmits payloads every second via nRF24L01+ on SPI0 (GPIO 10=MOSI, 11=SCK, 12=CSN, 13=CE).

- **Base Station** (`src/base_station.c`):  
  - Initializes nRF24L01+ on SPI1 (GPIO 2=MOSI, 3=SCK, 4=CSN, 5=CE) under FreeRTOS.  
  - Listens continuously for incoming payloads.  
  - Prints received temperatures to USB serial (stdio USB CDC).

- **onewire.c / onewire.h**:  
  - Robust bit-banged 1-Wire implementation with shared `ow_pin`.  
  - Timings conform to DS18B20 datasheet (reset, write, read slots).  

- **ds18b20.c / ds18b20.h**:  
  - High-level API for DS18B20 commands (READ ROM, CONVERT T, READ SCRATCHPAD).  

- **Radio driver** (`src/nrf24l01.c` / `nrf24l01.h`):  
  - Basic TX/RX functions using the Pico SDK’s hardware SPI.  
  - Configuration for 2.4 GHz channel, 32-byte payload, auto-ack disabled.

- **FreeRTOS** support:  
  - Two tasks on sensor node: `vTempReadTask` and `vRadioTxTask`.  
  - Two tasks on base station: `vRadioRxTask` and `vSerialTxTask`.

---

## Running the Prototype

### Hardware Setup

1. **Sensor Node** (Pico A):  
   - DS18B20 #1 → GP4 (pin 6)  
   - DS18B20 #2 → GP5 (pin 7)  
   - Each sensor has a 4.7 kΩ pull-up to 3.3 V.  
   - nRF24L01+ connections:  
     - CE → GP13  
     - CSN → GP12  
     - SCK → GP11  
     - MOSI → GP10  
     - MISO → GP9   
     - VCC → 3V3, GND → GND

2. **Base Station** (Pico B):  
   - nRF24L01+ wired similarly on SPI1 pins:  
     - CE → GP5  
     - CSN → GP4  
     - SCK → GP3  
     - MOSI → GP2  
     - MISO → GP1  
     - VCC → 3V3, GND → GND  
   - Connect Pico B to PC via USB for serial output.

### Build & Flash

From the project root:
```bash
mkdir build && cd build
cmake ..
make -j$(nproc)
```

- **Sensor Node firmware**: outputs `sensor_node.uf2`; copy onto Pico A.  
- **Base Station firmware**: outputs `base_station.uf2`; copy onto Pico B.

### Serial Monitor

1. Open a terminal at **115200 baud** on the Pico B’s USB CDC port.  
2. Reset Pico B; you should see lines like:
   ```
   Received sensor 1: 23.50 °C
   Received sensor 2: 24.12 °C
   ```
3. Observe updates every second.

---

## Next Steps

- **Add encryption** or CRC checks on radio payloads.  
- **Extend Base Station** to Pico W for Wi‑Fi streaming or HTTP API.  
- **Implement alarm thresholds** and bidirectional commands.

---

*End of current prototype documentation.*
