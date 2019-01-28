# demo-Smart-Heart-rate-detector

Basic Information:

A Demo of Qualcomm MDM9206 

Maintainer: Kou Zhiwu,JiangYuan,Wang Jie,Yan Shaohui,YangRong.

Start date: December 17th, 2018

Function description:

Use the pluseSensor to detect heart rate, when the heart rate value exceeds a certain threshold, led lights up.

Document introduction:

===> Gizwits: it mainly include Gizwits cloud protocol related files.

===> Utils: It mainly includes some common data processing algorithms.

===> demo-Smart-Motion-detector-picture: include some pictures about this demo.

===> driver: mainly include some driver code, about gpio, adxl345, spi etc.

===> gagent: file containing the Gizwits Cloud Device Connection Protocol Library.

===> main: code start runing from this directory.

===> objs: Store some link files.

===> sdk: Store head files.

===> target: Store image file.

===> demo-Smart-Motion-detector.doc: detailed description of the demo;

===> llvm_build.bat: Script to compile source code.

Usage Instructions:
1. Downloading code from github according to the repository in “https://github.com/ThunderSoft-XA/demo-Smart-Heart-rate-detector” sheet.
2. Compile the code and flash the image to Gokit4 development kit.
3. Connect the heart rate sensor to the ADC1 channel of the Gokit development board.
4. Connect one pin of the led to the D9 pin of the development board, and the other pin is connected to the vcc.
5. Turn on the oscilloscope, adjust to the appropriate gear position, connect the heart rate sensor's pulse signal output pin and GND to the oscilloscope.
6. USB  data cable to connect PC and Gokit development board.
7. Touch your finger to the signal acquisition surface of the heart rate sensor.
8. Open the serial debugging assistant, you can see the collected data in real time.
9. When the detected data exceeds the threshold, you can see that the led is lit.
