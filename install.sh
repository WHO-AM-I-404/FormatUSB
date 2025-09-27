#!/bin/bash

# 1. Copy binary
sudo cp formatusb /usr/local/bin/
sudo chmod 755 /usr/local/bin/formatusb

# 2. Copy library
sudo mkdir -p /usr/local/lib/formatusb/
sudo cp lib/formatusb_lib /usr/local/lib/formatusb/
sudo chmod 755 /usr/local/lib/formatusb/formatusb_lib

# 3. Copy changelog
sudo mkdir -p /usr/share/doc/formatusb/
sudo cp CHANGELOG.txt /usr/share/doc/formatusb/changelog.txt

# 4. Copy icon
sudo mkdir -p /usr/share/icons/hicolor/scalable/apps/
sudo cp images/logo.svg /usr/share/icons/hicolor/scalable/apps/formatusb.svg

# 5. Create desktop entry
sudo tee /usr/share/applications/formatusb.desktop > /dev/null << EOF
[Desktop Entry]
Name=FORMAT USB
Comment=Format USB drives and removable storage devices
Exec=/usr/local/bin/formatusb
Icon=formatusb
Type=Application
Categories=System;Utility;
Keywords=USB;format;drive;storage;disk;
EOF

# 6. Update desktop database
sudo update-desktop-database

echo "Install selesai! Binary dapat dijalankan dari mana saja."
