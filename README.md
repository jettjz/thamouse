# Thamouse
EECS149 Project
Members: Avinash Nandakumar, Ryan Thornton, Jack Zhang, Zac Patel

## Project Goal:
The overall project is to create a glove that acts as a mouse to a computer. In addition, this glove will provide basic multi-finger gestures (such as those on the macOS touchpad or on iPads). We plan to demonstrate that we can accurately control the mouse by pointing and do gesture controls fluidly using the glove.

While the overall goal would be to control the screen like a normal mouse while having features of multifinger gestures on Windows, we may not have enough time to implement drivers for implementing the multitouch gestures. If this proves too difficult, we will create a simple application that uses the gestures.

## Project Approach:
The project has 2 main parts:

1. Sensing where the mouse is pointing:

 We currently plan on achieving this similar to a wii-mote using infrared sensors. The reason for this being that we believe that it will be easier to keep a more absolute position for the pointer (thus, removing the need to deal with drift especially due to gestures)
This means that we would need to have an additional infrared light that we put at the base of the display

2. Detecting gestures (includes basic left/right clicks):

We will detect gestures through a combination of strain gauges on each finger and an accelerometer. The strain gauge (tension sensors) will detect whether the finger is extended or folded (such as in a fist). The accelerometer will then detect whether the motion is moving up, down, left, or right for sweeping motions
Basic left clicks will be simply controlled by the squeezing of the thumb
* We will use the Arduino micro for processing and a bluetooth module to transmit
* We will need to decide where to do processing, the microcontroller or the computer. 
* Ideally, we would use the board to transmit less data
* These also create a nice division of responsibility. Both parts can be worked on independently without reliance on the other part
* Other functionality may need to be included (such as a button/switch to turn the glove on and off)

## Resources: 
see https://docs.google.com/document/d/15LqMYDxss_5ZKtvKQVmt7ELVTU_zIfMgg3QwwFb_BEE/edit
* Sensors Needed:
* Buttons / strain sensors on fingers
* Accelerometers (sparkfun accelerometer)
* Gyroscope?
* Light sensor? Wiimote uses 2 UV lights as reference.
* Glove / gauntlet
* Microcontroller with bluetooth / wireless capability (buckler would work, but is large)
* Battery / portable power supply for controller

## Schedule:
* October 25: 
Project charter (this document)
* November 8 (Milestone 1):
Show significant progress on each task individually. We expect that we should be able to detect pointing and squeezing
* November 17 (Milestone 2): 
Show that we can accurately detect where the glove is pointing. Independently, show that we can detect individual squeezing of each finger as well as basic gestures (left, right, up, down)
Ideally, we hope to have both parts integrated together by this point for the project demonstrations
* Thanksgiving Break: 
Hardware finalized and combined into glove, software well underway
* December 8: 
Application Demo created. This would be either implementing the mouse with windows (perhaps using PyAutoGUI). If this package proves too difficult to implement gesture macros, we will create an application such as a drawing app instead. 
* Dead Week: 
project finished or with minor software tweaks
* Finals Week:
Finish report


## Risks:
* Once built, if the sensors are not chosen properly and of good quality we could be unable to measure enough gestures to create a usable device
* On the gesture side, there could easily be a lot of heuristics for the glove. Depending on thresholds, this may result in incorrect detection of clicks or gestures
* The gestures or pointing can be very jumpy. We will likely need to smooth these actions out in software, but risk missing different things or overfitting certain actions
* There are many additional features or UX possibilities. While important, we should make sure to choose and act quickly on these and choose time-efficient features
* Flex sensors may be difficult to match the glove without breaking
* If we are able to assemble all of the infinity stones, we may destroy half of humanity
