# IR LED Stripe Upgrade with esp01 for WiFI Usage

# Disclaimer
I got the idea from [this website](https://www.hackster.io/ashish_8284/making-smart-led-strip-from-boring-ir-controller-led-strip-22b0f9), but i did not like the way he did some things. So i changed some things.
You can not use the IR Remote after this modification!

# Components
- Buck Converter
- ESP01 Module
- LED Stripe
- LED Controller

# Usage
The code is just a base, it could have a lot more functions, but it is enough for the start.
- Flash the code to the esp01 module. (There are more than enough tutorials on Youtube)
- Wire everything as shown on the image below.
- You can acces the module by connecting to it via wifi from any device, get the ip and enter your home WiFi SSID and password.
- You can now enter the ip in a browser and change the colors from ther.
- I made an IOS app to change the colors, it is just a POST request to the ip with the RGBA arguments:
  - Example: http://123.123.123.123?Red=100&Green=100&Blue=100&Brightness=100

[<img style="width: 40%" src="https://github.com/tamasgara04/WIFI-LED-Upgrade/blob/main/wiring.jpg?raw=true">](wiring)

