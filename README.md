# Google Curtain Control
ESP8266 curtain controller with Google Assistant and Adafruit IO

The original tutorial for this application using Alexa can be found here:
* Alexa Curtain Control System - 3d Printable And Low Cost
  * https://www.electromaker.io/project/view/alexa-curtain-control-system-3d-printable-and-low-cost
* Youtube tutorial - How to build Alexa controlled curtains (Echo, 3D Printable & Adafruit Feather Huzzah!):
  * https://www.youtube.com/watch?time_continue=31&v=1X6pu14eNrc
* And the original code:
  * https://github.com/DIY-Machines/alexa-cutrain-control

In order to convert this to use the Google assistant I used the following tutorial:
* ESP8266 Voice Control With Google Assistant and Adafruit IO
  * https://www.instructables.com/id/Control-Your-Projects-With-Google-Assistant-and-Ad/
  
 ## How to get it working ##
 #### 1. Follow the "Step 1: Setting Up Adafruit IO" portion of the [above tutorial](https://www.instructables.com/id/Control-Your-Projects-With-Google-Assistant-and-Ad/) ####
 > In my code I named my feed "openclose", the name of the dashboard does not effect the code. If you named your feed something else, just CTRL+F in the Arduino IDE and replace with your feed name.
 
#### 2. Install the Adafruit libraries ####
Install the Adafruit MQTT Client library, which can be found under the Arduino Library Manager (Sketch > Include Library > Library Manager...)
 
#### 3. Fill in missing information ####
  i. At lines 5 and 6 enter your wifi's SSID and your wifi password
  
  ii. At lines 10 and 11 enter Adafruit.io username and key
  > Your Adafruit IO Key is a string of characters that can be found by pressing the gold key button on your dashboard, or "View AIO Key" on the left-hand menu of Adafruit IO.

#### 4. Finally, follow "Step 3: Connecting to Google Assistant Through IFTTT" portion of the [above tutorial](https://www.instructables.com/id/Control-Your-Projects-With-Google-Assistant-and-Ad/) ####
Instead of putting the data as "ON" I use the word "trigger" in my code. This allows me to make a single IFTTT rule with the following setup:

<img src="https://github.com/parrycj/google-curtain-control/blob/master/IFTTTConfig.png" width="400"> <img src="https://github.com/parrycj/google-curtain-control/blob/master/IFTTTConfig2.png" width="400">

> Orignally I had intended to use a text field in my rule as "$ the curtains" so I could say either "Open the curtains" or "Close the curtains" sending open and close as my trigger words, however, google picked up more than one word when the text field was not the last word in the command. Instead I configured the IFTTT rule this way since the code is already checking whether the curtains are opened or closed, it's irrelevant for the user to say open or close. Essentially the user is just saying "Trigger the curtains" but you can set this up however you see fit.

#### 5. Upload to your ESP8266 and try it out! ####

> **This code assumes that your curtains are closed to start**

> STEPS_PER_ROTATION on line 27 is defined as 1200, you can increase or decrease this number according to how far you need the motor to turn


*Special thank you to https://github.com/DIY-Machines and https://github.com/jzheadley for the help on making this possible!*
