# FormatUSB

## Overview

FormatUSB is a Qt-based graphical application for formatting USB drives and removable storage devices on Linux systems. The application provides a user-friendly interface for selecting USB devices and formatting them with various filesystem types (FAT32, NTFS, ext4, exFAT) and partition table options (GPT, MBR/MSDOS). Built with Qt5/Qt6 and C++20, it emphasizes safety by protecting against accidental formatting of system drives and offers comprehensive internationalization support for 50+ languages.

## User Preferences

Preferred communication style: Simple, everyday language.

## System Architecture

### Frontend Architecture

**UI Framework**: Qt5/Qt6 Widgets-based desktop application
- **UI Design**: Compact GUI with fixed dimensions (650x280) for a solid, modern appearance
- **UI Definition**: Interface layout defined in `mainwindow.ui` XML format, loaded at runtime
- **Translation System**: Qt Linguist-based internationalization with 50+ language translations stored in `translations/` directory as `.ts` files
- **Resource Management**: Embedded resources including CHANGELOG stored in application binary via Qt Resource System (`.qrc` files)
- **Main Window**: Single-window application (`MainWindow` class) containing all user interactions

**Key UI Components**:
- Device selection dropdown with USB device auto-detection
- Filesystem format selector (FAT32, NTFS, ext4, exFAT)
- Partition table options (Defaults, MSDOS, GPT)
- Volume label input field
- Device refresh and format action buttons
- Toggle options for showing all devices vs. USB-only and showing partitions

### Backend Architecture

**Core Technology**:
- **Language**: C++20
- **Build System**: qmake project files (`.pro`) with make compilation
- **Application Entry Point**: `main.cpp` initializes Qt application and MainWindow

**Device Detection Strategy**:
- Multiple detection methods for robust USB device identification
- Likely uses system calls to enumerate block devices (typical pattern: `/dev/sd*`, `/sys/block/`)
- Safety filtering to prevent system drive selection
- Support for showing/hiding partitions and all devices vs. removable-only

**Formatting Logic**:
- Executes formatting via external shell script (`lib/formatusb_lib`) made executable by default
- Shell script wraps system utilities (likely `mkfs.*`, `parted`, etc.) with elevated privileges
- Root/sudo privilege handling for device operations
- Progress feedback and error handling through Qt signals/slots

**Resource Architecture**:
- CHANGELOG embedded as internal Qt resource (no external file dependency)
- Images and UI resources bundled for portability
- Copyright attribution: "FormatUSB Team"

### External Dependencies

**System Requirements**:
- Linux operating system with X11/Wayland display server
- Qt5 or Qt6 libraries (QtWidgets, QtCore, QtGui modules)
- Root/sudo privileges for device formatting operations
- Physical USB hardware access

**External System Utilities** (invoked by shell script):
- Filesystem creation tools: `mkfs.vfat`, `mkfs.ntfs`, `mkfs.ext4`, `mkfs.exfat`
- Partition table management: `parted` or similar
- Block device utilities for device enumeration

**Build Dependencies**:
- Qt5/Qt6 development packages
- C++20 compatible compiler (g++/clang)
- qmake build tool
- make

**Runtime Dependencies**:
- Qt5/Qt6 runtime libraries
- Filesystem utilities matching supported formats
- System device access permissions