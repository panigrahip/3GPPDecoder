# This file is part of 3GPP Decoder project.
# Copyright (C) 2015  Prashant Panigrahi prashant@3glteinfo.com
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.

# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.

# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.



#-------------------------------------------------
#
# Project created by QtCreator 2015-06-23T13:43:46
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = 3GPPDecoder
TEMPLATE = app
win32:RC_ICONS += decoder.ico


SOURCES += main.cpp\
        mainwindow.cpp \
    preferencedialog.cpp \
    tsharkdecoder.cpp \
    umtsrlcdecoder.cpp \
    aboutdialog.cpp

HEADERS  += mainwindow.h \
    preferencedialog.h \
    tsharkdecoder.h \
    umtsrlcdecoder.h \
    aboutdialog.h

FORMS    += mainwindow.ui \
    preferencedialog.ui \
    aboutdialog.ui

DISTFILES +=

RESOURCES += \
    images.qrc
