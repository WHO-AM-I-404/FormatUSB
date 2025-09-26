# **********************************************************************
# * Copyright (C) 2025 FormatUSB Team
# *
# * Authors: FormatUSB Development Team
# *          FormatUSB v1.0.0.1 - Cross-platform USB formatting tool
# *          Enhanced compatibility with Debian/Ubuntu and derivatives
# *
# * This is free software: you can redistribute it and/or modify
# * it under the terms of the GNU General Public License as published by
# * the Free Software Foundation, either version 3 of the License, or
# * (at your option) any later version.
# *
# * This program is distributed in the hope that it will be useful,
# * but WITHOUT ANY WARRANTY; without even the implied warranty of
# * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# * GNU General Public License for more details.
# *
# * You should have received a copy of the GNU General Public License
# * along with this package. If not, see <http://www.gnu.org/licenses/>.
# **********************************************************************/

QT       += core gui
CONFIG   += c++20

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = formatusb
TEMPLATE = app


SOURCES += main.cpp\
    mainwindow.cpp \
    about.cpp \
    cmd.cpp

HEADERS  += \
    mainwindow.h \
    version.h \
    about.h \
    cmd.h

FORMS    += \
    mainwindow.ui

TRANSLATIONS += translations/formatusb_en.ts \
                

RESOURCES += \
    images.qrc
