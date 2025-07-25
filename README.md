# Raspberry Pi Pico + FreeRTOS Project

This document provides step-by-step instructions for integrating the FreeRTOS kernel into a Raspberry Pi Pico project using Visual Studio Code.

---

## Prerequisites

- **Hardware**  
  - Raspberry Pi Pico

- **Software**  
  - Visual Studio Code (version 1.60 or later)  
  - [Raspberry Pi Pico VS Code Extension](https://marketplace.visualstudio.com/items?itemName=raspberry-pi.raspberry-pi-pico)  
  - CMake (version 3.13 or later)  
  - GNU Arm Embedded Toolchain  
  - Git  

---

## 1. Scaffold the Project

1. Open **Visual Studio Code**  
2. Install the **Raspberry Pi Pico** extension  
3. Open the **Command Palette** (⇧⌘P / Ctrl+Shift+P)  
4. Select **Pico: Create New Project**  
5. Choose the **"Hello World"** template, and name the project folder **`<PROJECT_NAME>`**  
6. Confirm to generate `CMakeLists.txt`, `.vscode/`, and `main.c`  

---

## 2. Add an `external/` Directory

```bash
cd ~/projects/<PROJECT_NAME>
mkdir external
```

---

## 3. Clone the FreeRTOS Kernel

```bash
cd external
git clone https://github.com/FreeRTOS/FreeRTOS-Kernel.git
cd ..
```

This places the kernel at `external/FreeRTOS-Kernel/`

---

## 4. Update `CMakeLists.txt`

1. Open the root `CMakeLists.txt`  
2. Below the line:  
   ```cmake
   include(pico_sdk_import.cmake)
   ```  
   insert:  
   ```cmake
   # Import FreeRTOS for RP2040 (GCC)
   include(external/FreeRTOS-Kernel/portable/ThirdParty/GCC/RP2040/FreeRTOS_Kernel_import.cmake)
   ```

3. In the `target_link_libraries(<PROJECT_NAME> ...)` section, append:  
   ```cmake
       # FreeRTOS kernel and Heap_4 allocator
       FreeRTOS-Kernel
       FreeRTOS-Kernel-Heap4
   ```

---

## 5. Provide `FreeRTOSConfig.h`

1. Download the RP2040 config file:

   ```bash
   curl -L -o FreeRTOSConfig.h \
     https://raw.githubusercontent.com/FreeRTOS/FreeRTOS-Community-Supported-Demos/main/CORTEX_M0%2B_RP2040/Standard_smp/FreeRTOSConfig.h
   ```

2. Edit `FreeRTOSConfig.h`:

    ```diff
   /* Scheduler Related */
   -#define configUSE_TICK_HOOK                     1
   +#define configUSE_TICK_HOOK                     0
   ```

---

## 6. Build and Flash

### Option A: CMake + Ninja + VS Code

1. Run:
   ```bash
   mkdir build && cd build
   cmake -G Ninja ..
   ninja
   ```

2. In VS Code:  
   - Open the **Raspberry Pi Pico** Project tab  
   - Expand the **Project** section  
   - Click **Run Project (USB)** to build and flash automatically

### Option B: CMake + Make + Picotool

1. Run:
   ```bash
   mkdir build && cd build
   cmake ..
   make -j$(nproc)
   ```

2. Put the Pico in bootloader mode (hold **BOOTSEL** while connecting USB)  
3. Run:
   ```bash
   picotool load <PROJECT_NAME>.uf2 -v
   picotool reboot
   ```

---

## References

- [Pico C/C++ SDK Documentation](https://raspberrypi.github.io/pico-sdk-doxygen/)  
- [FreeRTOS Official Site](https://www.freertos.org/)  
- [FreeRTOS Community Demos for RP2040](https://github.com/FreeRTOS/FreeRTOS-Community-Supported-Demos)
