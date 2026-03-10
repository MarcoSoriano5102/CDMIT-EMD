# CDMIT-EMD

## Table of Contents

- [Prerequisites](#prerequisites)
- [Repository Structure](#repository-structure)
- [Getting Started](#getting-started)
  - [1. Clone the Repository](#1-clone-the-repository)
  - [2. Set Up the Pico SDK](#2-set-up-the-pico-sdk)
  - [3. Configure Environment Variables](#3-configure-environment-variables)
  - [4. Build the Project](#4-build-the-project)
  - [5. Flash the Firmware](#5-flash-the-firmware)
- [CMake Configuration](#cmake-configuration)
- [Build Options](#build-options)
- [Troubleshooting](#troubleshooting)

---

## Prerequisites

The following tools must be installed on your development machine before building the project.

### Required Tools

| Tool | Minimum Version | Notes |
|---|---|---|
| CMake | 3.13 | Build system generator |
| GNU Arm Embedded Toolchain | 10.3-2021.10 | `arm-none-eabi-gcc` |
| Raspberry Pi Pico SDK | 1.5.0 | See setup instructions below |
| Python 3 | 3.9+ | Required by the Pico SDK build system |
| Ninja or Make | Latest | Recommended: Ninja for faster builds |

### Platform-Specific Installation

**Ubuntu / Debian**
```bash
sudo apt update
sudo apt install cmake gcc-arm-none-eabi libnewlib-arm-none-eabi \
     build-essential libstdc++-arm-none-eabi-newlib python3 git ninja-build
```

---

## Repository Structure

```
CDMIT-EMD/
├── CMakeLists.txt          # Top-level CMake configuration
├── pico_sdk_import.cmake   # SDK import helper (copied from SDK)
├── src/
│   ├── main.cpp              # Application entry point
├── include/
│   └── ...                 # Project header files
└── README.md
```

---

## Getting Started

### 1. Clone the Repository

```bash
git clone https://github.com/MarcoSoriano5102/CDMIT-EMD.git
cd CDMIT-EMD
```

### 2. Set Up the Pico SDK

The Pico SDK can be used either as a Git submodule within the project or installed separately on the host system.

**External SDK installation (for multi-project setups)**

```bash
git clone https://github.com/raspberrypi/pico-sdk.git ~/pico-sdk
cd ~/pico-sdk
git submodule update --init
```

### 3. Configure Environment Variables

When using an external SDK installation, export the `PICO_SDK_PATH` environment variable so CMake can locate it.

**Linux / macOS**
```bash
export PICO_SDK_PATH=$HOME/pico-sdk
```

To make this persistent, add the line above to your `~/.bashrc`, `~/.zshrc`, or equivalent shell configuration file.

### 4. Build the Project

Create a dedicated build directory to keep generated files separate from source files.

```bash
mkdir build
cd build
cmake .. -G Ninja
ninja
```

Alternatively, using Make:

```bash
mkdir build
cd build
cmake ..
make -j$(nproc)
```

Upon a successful build, the following output files will be present in the `build/` directory:

| File | Description |
|---|---|
| `project_name.uf2` | Drag-and-drop firmware image for the Pico bootloader |
| `project_name.elf` | ELF binary for use with a debugger (e.g., OpenOCD, picoprobe) |
| `project_name.bin` | Raw binary image |
| `project_name.hex` | Intel HEX format |

### 5. Flash the Firmware

**Via USB Mass Storage (BOOTSEL mode)**

1. Hold the **BOOTSEL** button on the Pico while connecting it to your computer via USB.
2. The device will mount as a USB mass storage drive named `RPI-RP2`.
3. Copy the `.uf2` file to the drive:

```bash
cp build/project_name.uf2 /media/$USER/RPI-RP2/
```

---

## CMake Configuration

The top-level `CMakeLists.txt` follows the standard structure required by the Pico SDK.

```cmake
cmake_minimum_required(VERSION 3.13)

# Import the Pico SDK before the project() call
include(pico_sdk_import.cmake)

project(project_name C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# Initialise the Pico SDK
pico_sdk_init()

add_executable(project_name
    src/main.c
)

# Pull in required Pico libraries
target_link_libraries(project_name
    pico_stdlib
    hardware_gpio
    hardware_uart
)

# Enable USB and UART output
pico_enable_stdio_usb(project_name 1)
pico_enable_stdio_uart(project_name 0)

# Generate UF2, BIN, HEX, and MAP output files
pico_add_extra_outputs(project_name)
```

The file `pico_sdk_import.cmake` must be copied from the SDK into the project root:

```bash
cp $PICO_SDK_PATH/external/pico_sdk_import.cmake .
```

---

## Build Options

The following CMake variables can be passed at configuration time to control build behaviour.

| Variable | Default | Description |
|---|---|---|
| `PICO_BOARD` | `pico` | Target board (`pico`, `pico_w`, etc.) |
| `CMAKE_BUILD_TYPE` | `Release` | Build type (`Release`, `Debug`, `MinSizeRel`) |
| `PICO_STDIO_USB` | `1` | Enable USB serial output |
| `PICO_STDIO_UART` | `0` | Enable UART serial output |

**Example — building for Pico W in Debug mode**

```bash
cmake .. -G Ninja -DPICO_BOARD=pico_w -DCMAKE_BUILD_TYPE=Debug
ninja
```

---

## Troubleshooting

**CMake cannot find the Pico SDK**

Ensure `PICO_SDK_PATH` is exported correctly and points to the root of a valid SDK clone that has had its submodules initialised. Verify with:

```bash
echo $PICO_SDK_PATH
ls $PICO_SDK_PATH/CMakeLists.txt
```

**`arm-none-eabi-gcc` not found**

Confirm the toolchain is installed and on your `PATH`:

```bash
arm-none-eabi-gcc --version
```

If the command is not found, revisit the toolchain installation steps for your platform.

**Build fails with TinyUSB or mbedTLS errors**

These components are included as submodules within the Pico SDK. Run the following from the SDK directory:

```bash
cd $PICO_SDK_PATH
git submodule update --init --recursive
```

**Device not recognised in BOOTSEL mode**

Try a different USB cable — many cables are charge-only and do not carry data. Ensure the BOOTSEL button is held before the cable is connected, not after.