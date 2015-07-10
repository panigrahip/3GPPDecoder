What is 3GPPDecoder?
=====================

3GPPDecoder is a free open source decoder to decode wireless messages and PDUs(Protocol Data Units). The program currently supports Windows OS and Linux.

You can use 3GPPDecoder to decode 3GPP LTE, UMTS and GSM messages for RRC and NAS. Also it supports RLC (Radio Link Control) protocol PDU header decoding.

## How to build 3GPPDecoder

3GPPDecoder is developed using QT and main programming language is C++. To build the decoder program open the 3GPP Decoder.pro using QT Create and build it inside the environment.

On Debian (apt based package systems) install qmake and qt development files:

$> sudo apt-get install qt5-qmake libqt4-dev

run qmake to create a Makefile:

$> qmake

run make:

$> make

## How to get the Setup file

You can also download the 3GPPDecoder executable from 3GLteInfo.com website.


