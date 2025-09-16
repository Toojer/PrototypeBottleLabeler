# PrototypeBottleLabeler
Initial design for the honey bottle label dispenser.  

Need to install Arduino IDE (2.3.6 used in the original build)
  Install the following packages inside of the Arduino IDE: 
    * AccelStepper
    * ezButton 
    Install the packages exactly how they are spelled.  There are other packages that may be similar in name, but the packages are case sensitive.

Using Arduino UNO R3 from about 2013.  Works fine in initial build.  Can't imagine a newer Arduino R3 would be any different.

Using 28BYJ-48 Stepper Motor with a ULN2003 Motor Driver
https://www.amazon.com/dp/B01CP18J4A?ref=ppx_yo2ov_dt_b_fed_asin_title
Using Pins 4,5,6,7 for conveyor motor and Pins 8,9,10,11 for sticker dispenser motor.   See the link inside of the code for reference to wiring used. 

Shopping list for project is located in this spreadsheet: 
https://docs.google.com/spreadsheets/d/1KZ6SiuTt13xs5TYWtGyumSnYDdGxF8kruBy6Y83toKw/edit?usp=drive_link
