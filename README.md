# FormatUSB v1.0.0.3

[![License: GPL v3](https://img.shields.io/badge/License-GPLv3-blue.svg)](https://www.gnu.org/licenses/gpl-3.0)
[![Version](https://img.shields.io/badge/Version-1.0.0.3-green.svg)](https://github.com/WHO-AM-I-404/formatusb)
[![Build Status](https://img.shields.io/badge/Build-Passing-brightgreen.svg)](https://github.com/WHO-AM-I-404/formatusb)

**FormatUSB** is a modern graphical tool for formatting USB drives and removable storage devices on Linux. Built with Qt5/6, it provides an intuitive and safe interface for formatting USB drives with various filesystem options.

![FormatUSB](images/FORMAT.png)

> **Note**: This is a **Linux desktop application** that requires X11/Wayland display server, root privileges, and physical USB hardware access. While the code can be developed and compiled in cloud environments like Replit, the application must be run on a local Linux system with physical USB devices.

---

## 📋 Table of Contents

- [Features](#-features)
- [System Requirements](#-system-requirements)
- [Installation](#-installation)
- [Usage](#-usage)
- [Building from Source](#-building-from-source)
- [Troubleshooting](#-troubleshooting)
- [Project Structure](#-project-structure)
- [Changelog](#-changelog)
- [Contributing](#-contributing)
- [License](#-license)

---

## ✨ Features

- **🎨 Modern GUI**: Clean, contemporary design for easy use
- **💾 Multiple Filesystem Support**: 
  - **FAT32** - Universal compatibility (Windows/Mac/Linux)
  - **NTFS** - Windows native, large file support
  - **ext4** - Linux native, optimal performance
  - **exFAT** - Cross-platform with large file support
- **🔍 Automatic Device Detection**: Safely detects removable USB devices
- **🛡️ Safety Features**: Protection to prevent formatting system drives
- **⚙️ Partition Table Options**: Support for GPT, MBR/MSDOS, and defaults
- **🏷️ Custom Volume Labels**: Set custom names for your drives
- **🌍 Internationalization**: Supports 50+ languages
- **📦 Embedded Resources**: CHANGELOG and assets bundled in binary

---

## 🖥️ System Requirements

### Supported Operating Systems

| Distribution | Minimum Version | Status |
|--------------|----------------|--------|
| Ubuntu | 18.04 LTS | ✅ Tested |
| Debian | 9 (Stretch) | ✅ Tested |
| Linux Mint | 19 | ✅ Compatible |
| Elementary OS | 5.0 | ✅ Compatible |
| Pop!_OS | 20.04 | ✅ Compatible |
| Other Debian-based | - | ⚠️ Should work |

### Hardware Requirements

- **RAM**: Minimum 512 MB (Recommended: 1 GB+)
- **Disk Space**: 10 MB for application
- **Display**: X11 or Wayland display server
- **USB Port**: For devices to be formatted

### Software Dependencies

**Required:**
- Qt5 ≥ 5.9 (Core, GUI, Widgets)
- C++ Compiler with C++20 support
- X11 or Wayland display server
- Root/sudo privileges (for formatting operations)

**System Tools:**
- `parted` - Partition table manipulation
- `lsblk` - Block device listing
- `udevadm` - Device information (optional, fallback)

**Filesystem Tools (as needed):**
- `mkfs.vfat` (dosfstools) - For FAT32
- `mkfs.ntfs` (ntfs-3g) - For NTFS
- `mkfs.ext4` (e2fsprogs) - For ext4
- `mkfs.exfat` (exfatprogs or exfat-utils) - For exFAT

---

## 📦 Installation

### Method 1: Pre-built Binary (Recommended)

**Coming soon** - .deb packages will be available on GitHub Releases

### Method 2: Build from Source

#### Step 1: Install Build Dependencies

**Ubuntu/Debian/Linux Mint:**
```bash
# Update package lists
sudo apt update

# Install build tools and Qt5
sudo apt install -y \
    build-essential \
    gcc \
    g++ \
    make \
    qtbase5-dev \
    qtchooser \
    qt5-qmake \
    qtbase5-dev-tools \
    pkg-config

# Install system utilities
sudo apt install -y \
    parted \
    dosfstools \
    ntfs-3g \
    e2fsprogs

# Install exFAT support (choose one)
# For modern systems (Ubuntu 20.04+, Debian 11+):
sudo apt install -y exfatprogs

# Or for older systems (Ubuntu 18.04, Debian 9-10):
sudo apt install -y exfat-fuse exfat-utils
```

**Verify Dependencies:**
```bash
# Check Qt5 installation
qmake --version
# Expected: QMake version 3.x, Using Qt version 5.x

# Check compiler
g++ --version
# Expected: g++ (Ubuntu/Debian) 7.5.0 or newer

# Check system tools
parted --version
lsblk --version
```

#### Step 2: Clone Repository

```bash
# Clone from GitHub
git clone https://github.com/WHO-AM-I-404/FormatUSB.git

# Enter project directory
cd FormatUSB

# (Optional) Checkout specific version tag
git checkout v1.0.0.3
```

#### Step 3: Build Application

```bash
# Generate Makefile with qmake
qmake src.pro

# Compile with make (use multiple cores for speed)
make -j$(nproc)

# Result: 'formatusb' executable will be created in current directory
```

**Expected Output:**
```
...
g++ -Wl,-O1 -o formatusb main.o mainwindow.o about.o cmd.o ...
```

#### Step 4: Verify Build

```bash
# Check executable file
ls -lh formatusb
# Expected: -rwxr-xr-x ... 160K ... formatusb

# Check version
./formatusb --version
# Expected output: Version: 1.0.0.3

# Run health check script
bash check_formatusb.sh
# Expected: ✓ FormatUSB is in excellent condition!
```

#### Step 5: Install System-Wide (Optional)

```bash
# Copy binary to system path
sudo cp formatusb /usr/local/bin/formatusb

# Copy library
sudo mkdir -p /usr/lib/formatusb
sudo cp lib/formatusb_lib /usr/lib/formatusb/
sudo chmod +x /usr/lib/formatusb/formatusb_lib

# Copy library
sudo mkdir -p /usr/local/lib/formatusb/
sudo cp lib/formatusb_lib /usr/local/lib/formatusb/
sudo chmod 755 /usr/local/lib/formatusb/formatusb_lib


# Copy icon
sudo mkdir -p /usr/share/icons/hicolor/scalable/apps/
sudo cp images/logo.svg /usr/share/icons/hicolor/scalable/apps/formatusb.svg


# Create desktop entry
sudo tee /usr/share/applications/formatusb.desktop << 'EOF'
[Desktop Entry]
Name=FormatUSB
GenericName=USB Formatter
Comment=Format USB drives and removable storage devices
Exec=/usr/local/bin/formatusb
Icon=drive-removable-media-usb
Terminal=false
Type=Application
Categories=System;Utility;HardwareSettings;
Keywords=USB;format;drive;storage;disk;partition;
EOF

# Update desktop database
sudo update-desktop-database

# (Optional) Copy icon
sudo cp formatusb.png /usr/share/pixmaps/formatusb.png
```

**Verify Installation:**
```bash
# Check installation
which formatusb
# Expected: /usr/local/bin/formatusb

# Run from anywhere
formatusb --version
# Expected: Version: 1.0.0.3
```

---

## 🚀 Usage

### Running the Application

**From Source Directory:**
```bash
# Run directly from build folder
./formatusb
```

**If Installed System-Wide:**
```bash
# Run from terminal
formatusb

# Or find in application menu: System → FormatUSB
```

**Important Notes:**
- ⚠️ Application requires **root privileges** for formatting operations
- If prompted for sudo password, enter your user password
- Use polkit/pkexec for automatic privilege escalation

### Step-by-Step Guide

#### 1️⃣ Preparation
- Connect the USB drive you want to format
- **Backup all important data** from the USB drive
- Launch FormatUSB application

#### 2️⃣ Select Target Device
- In "Select Target USB Device" dropdown, choose the correct device
- **Check carefully** - ensure you select the right USB!
- Tip: Check capacity and device name to verify

**Additional Options:**
- ☑️ **Show all devices** - Display all storage (⚠️ **Caution!** Can format HDD/SSD)
- ☑️ **Show partitions** - Display individual partitions

#### 3️⃣ Choose Filesystem Format

| Format | Max File Size | Max Volume Size | Compatibility | Use Case |
|--------|---------------|-----------------|---------------|----------|
| **FAT32** | 4 GB | 2 TB | Windows/Mac/Linux/Android | USB for small files, maximum compatibility |
| **NTFS** | 16 TB | 256 TB | Windows/Linux (read) | Windows USB, large files (>4GB) |
| **ext4** | 16 TB | 1 EB | Linux only | Linux-only USB, best performance |
| **exFAT** | 16 EB | 128 PB | Modern Windows/Mac/Linux | Modern USB, large files, cross-platform |

**Recommendations:**
- 💾 **General purpose**: FAT32 (if files < 4GB)
- 🎥 **Large videos/ISOs**: exFAT or NTFS
- 🐧 **Linux only**: ext4
- 🪟 **Windows only**: NTFS

#### 4️⃣ Set Partition Options

- **Defaults** - Let system choose (recommended for beginners)
- **MBR/msdos** - Compatible with old BIOS and legacy systems
- **GPT** - Modern, recommended for drives > 2TB

#### 5️⃣ Customize Volume Label
- Enter a name for your USB drive
- Use alphanumeric characters (A-Z, 0-9, _, -)
- Avoid spaces and special characters
- Example: `MYUSB`, `Backup_2025`, `Data_Drive`

#### 6️⃣ Format the Drive
1. Click **"Next"** button
2. **Confirm** operation in the dialog
3. Wait for formatting process to complete
4. Check output log for errors
5. **Safely eject USB** after completion

---

## ⚠️ Important Safety Information

### Data Loss Warning
- ❌ **Formatting will DELETE ALL data** on the selected drive
- ❌ **Deleted data CANNOT be recovered**
- ✅ **Always backup important data** before formatting
- ✅ **Double-check device selection** before formatting

### Built-in Safety Features
- 🛡️ **System Drive Protection** - Prevents formatting system drives
- 🛡️ **Confirmation Dialog** - Confirms before destructive operations
- 🛡️ **Device Type Verification** - Filters for removable devices only
- 🛡️ **Process Monitoring** - Logs all operations for debugging

### Best Practices
1. ✅ Always safely eject USB after formatting
2. ✅ Test USB after formatting by copying test files
3. ✅ Never unplug USB during formatting process
4. ✅ Use "Show all devices" only when absolutely necessary

---

## 🛠️ Troubleshooting

### Problem 1: Permission Denied

**Error:**
```
Error: Permission denied
Cannot access /dev/sdb
```

**Solution:**
```bash
# Run with sudo
sudo ./formatusb

# Or if installed system-wide
sudo formatusb
```

### Problem 2: Qt Library Not Found

**Error:**
```
error while loading shared libraries: libQt5Core.so.5
```

**Solution:**
```bash
# Install Qt5 runtime libraries
sudo apt install -y qtbase5-dev libqt5widgets5 libqt5gui5 libqt5core5a

# Or install Qt5 full
sudo apt install -y qt5-default
```

### Problem 3: Device Not Detected

**Symptoms:** Device dropdown is empty or USB doesn't appear

**Solution:**
1. **Check USB Connection:**
   ```bash
   # List all block devices
   lsblk
   
   # Check USB devices
   lsusb
   ```

2. **Try Different USB Port**

3. **Enable "Show all devices"** (⚠️ careful!)

4. **Check dmesg logs:**
   ```bash
   sudo dmesg | tail -20
   ```

### Problem 4: Formatting Fails

**Error:**
```
Error: Failed to create filesystem
```

**Solution:**

1. **Unmount device first:**
   ```bash
   # Check mounted partitions
   mount | grep sdb
   
   # Unmount all partitions
   sudo umount /dev/sdb*
   ```

2. **Verify filesystem tools installed:**
   ```bash
   # Check available mkfs tools
   ls /sbin/mkfs.*
   
   # Install if missing
   sudo apt install dosfstools ntfs-3g e2fsprogs exfatprogs
   ```

3. **Check device for errors:**
   ```bash
   # Check device status
   sudo parted /dev/sdb print
   
   # Check bad blocks (warning: slow!)
   sudo badblocks -v /dev/sdb
   ```

### Problem 5: Build Errors

**Error during compilation:**

**Solution:**
```bash
# Clean build artifacts
make clean
rm -f Makefile moc_* ui_* qrc_*

# Regenerate Makefile
qmake src.pro

# Rebuild
make -j$(nproc)
```

**Missing Qt headers:**
```bash
# Install all Qt5 development packages
sudo apt install -y qtbase5-dev qtbase5-dev-tools qt5-qmake
```

### Log Files

Application logs are saved to:
```
/tmp/formatusb.log
```

For debugging, check log file:
```bash
tail -f /tmp/formatusb.log
```

---

## 🗂️ Project Structure

```
FormatUSB/
├── src.pro                 # Qt project configuration file
├── Makefile               # Generated by qmake (after build)
├── formatusb              # Compiled executable (after build)
│
├── main.cpp               # Application entry point
├── mainwindow.cpp         # Main window logic
├── mainwindow.h           # Main window header
├── mainwindow.ui          # Qt Designer UI file (GUI layout)
├── about.cpp              # About dialog implementation
├── about.h                # About dialog header
├── cmd.cpp                # Command execution wrapper
├── cmd.h                  # Command execution header
├── version.h              # Version definitions (1.0.0.3)
│
├── images.qrc             # Qt resource file (embedded assets)
├── CHANGELOG.txt          # Application changelog (embedded in binary)
├── README.md              # This file - User documentation
├── README1.md             # Technical documentation
├── LICENSE                # GPL v3 License
│
├── check_formatusb.sh     # Health check script
│
├── lib/
│   └── formatusb_lib      # Core formatting library (Bash script)
│
├── images/
│   ├── FORMAT.png         # Screenshot
│   └── logo.svg           # Application logo
│
├── translations/          # Internationalization files
│   ├── formatusb_en.ts    # English
│   ├── formatusb_id.ts    # Indonesian
│   ├── formatusb_de.ts    # German
│   ├── formatusb_fr.ts    # French
│   ├── formatusb_es.ts    # Spanish
│   ├── formatusb_zh_CN.ts # Chinese (Simplified)
│   ├── formatusb_ja.ts    # Japanese
│   └── ... (50+ languages)
│
├── debian/                # Debian packaging files
│   ├── changelog          # Debian changelog
│   ├── control            # Package dependencies
│   ├── copyright          # Copyright information
│   ├── install            # Installation rules
│   ├── rules              # Build rules
│   └── source/
│       └── format         # Source format
│
└── polkit-actions/        # PolicyKit rules
    └── org.usbformat.pkexec.formatusb.policy
```

---

## 📝 Changelog

### Version 1.0.0.3 (October 03, 2025)

**Changes:**
- ➕ **Added** comprehensive health check script
- 📚 **Updated** documentation with detailed installation guide
- 🔧 **Improved** README with troubleshooting section
- 🌐 **Converted** all documentation to English

**Technical:**
- Enhanced build verification with `check_formatusb.sh`
- Updated all version references across codebase
- Improved documentation clarity and completeness

### Version 1.0.0.2 (October 03, 2025)

**Changes:**
- ➖ **Removed** license button from about dialog (cleaner UI)
- 📦 **Embedded** CHANGELOG.txt into application resources
- 🚀 **Improved** resource management and portability
- ✨ **Enhanced** build system with proper resource embedding

**Technical:**
- Modified `about.cpp` to load CHANGELOG from `:/docs/CHANGELOG.txt`
- Updated `images.qrc` to embed CHANGELOG as Qt resource
- Changed `displayAboutMsgBox()` signature to remove license parameters
- No longer depends on external CHANGELOG.txt file

### Version 1.0.0.1 (September 25, 2025)

**Changes:**
- 🎉 Initial release after fork from MX Linux FormatUSB
- 🔍 Enhanced USB detection (multiple methods: lsblk, udevadm, /sys/block)
- 🎨 Modern GUI with compact 600x285 design
- 📝 Updated copyright to FormatUSB Team
- 🛠️ Improved error handling and device detection

---

## 🧪 Development & Testing

### Health Check Script

Use the health check script to verify project integrity:

```bash
# Run health check
bash check_formatusb.sh
```

**Expected output:**
```
═══════════════════════════════════════════════════════════
  Summary
═══════════════════════════════════════════════════════════
  Total checks: 47
  Passed: 47
  Warnings: 0
  Failed: 0

✓ FormatUSB is in excellent condition!
```

### Testing Checklist

Before release, ensure:

- [ ] Build succeeds without errors
- [ ] Health check script passes 100%
- [ ] Version numbers consistent across all files
- [ ] Executable runs without crashes
- [ ] USB detection works
- [ ] Formatting works for all filesystem types
- [ ] About dialog displays changelog
- [ ] No segfaults or memory leaks

### Development Dependencies

```bash
# Install development tools
sudo apt install -y \
    git \
    valgrind \
    gdb \
    qtcreator
```

---

## 🤝 Contributing

Contributions are welcome! Here's how to contribute:

### Reporting Bugs

1. Open [GitHub Issues](https://github.com/WHO-AM-I-404/FormatUSB/issues)
2. Check if bug already reported
3. Create new issue with template:
   - **OS & Version**: Ubuntu 22.04, Debian 12, etc.
   - **Qt Version**: Output from `qmake --version`
   - **Steps to Reproduce**: Steps to trigger bug
   - **Expected Behavior**: What should happen
   - **Actual Behavior**: What actually happens
   - **Logs**: Output from `/tmp/formatusb.log`
   - **Screenshots**: If applicable

### Contributing Code

1. **Fork** the repository
2. **Clone** your fork:
   ```bash
   git clone https://github.com/YOUR_USERNAME/FormatUSB.git
   ```
3. **Create feature branch**:
   ```bash
   git checkout -b feature/amazing-feature
   ```
4. **Make changes** and test thoroughly
5. **Commit** with descriptive messages:
   ```bash
   git commit -m "Add: Support for F2FS filesystem"
   ```
6. **Push** to fork:
   ```bash
   git push origin feature/amazing-feature
   ```
7. **Create Pull Request** on GitHub

### Coding Standards

- Follow Qt coding conventions
- Use modern C++20 features where appropriate
- Add comments for complex logic
- Test on multiple Linux distributions
- Update documentation for changes

---

## 📄 License

This project is licensed under **GNU General Public License v3.0**.

```
FormatUSB - USB Drive Formatting Tool
Copyright (C) 2025 FormatUSB Team

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.
```

See [LICENSE](LICENSE) file for complete details.

---

## 👥 Credits

### Development Team
- **WHO-AM-I-404** - Lead Developer & Maintainer
- **FormatUSB Team** - Contributors

### Original Project
- **MX Linux Team** - Original FormatUSB implementation
- **James Bowlin (BitJam)** - live-usb-maker utilities

### Frameworks & Libraries
- **Qt Framework** - GUI toolkit
- **GNU Project** - Core utilities

---

## 📞 Support & Contact

- **GitHub Repository**: [https://github.com/WHO-AM-I-404/FormatUSB](https://github.com/WHO-AM-I-404/FormatUSB)
- **Issues & Bug Reports**: [GitHub Issues](https://github.com/WHO-AM-I-404/FormatUSB/issues)
- **Email**: formatusb1122@gmail.com

---

## ⭐ Show Your Support

If this project is useful to you, give it a ⭐ on GitHub!

[![GitHub stars](https://img.shields.io/github/stars/WHO-AM-I-404/FormatUSB?style=social)](https://github.com/WHO-AM-I-404/FormatUSB)
[![GitHub forks](https://img.shields.io/github/forks/WHO-AM-I-404/FormatUSB?style=social)](https://github.com/WHO-AM-I-404/FormatUSB/fork)

---

**Made with ❤️ for the Linux community**

*FormatUSB is an open source tool developed to make formatting USB drives on Linux safe, easy, and intuitive.*
