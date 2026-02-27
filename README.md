# BrainstormTally
Brainstorm Tally Injector for common Mixer / Tally Systems

![housing](https://github.com/Bavarialex/BrainstormTally/blob/main/pics/housing.jpg)

For use in TV production environments, with a vision-mixer providing dry close contacts for camera tally.

![mixer](https://github.com/Bavarialex/BrainstormTally/blob/main/pics/mixer.jpg)
Mixer is providing tally contacts for camera 1 - 3, but only when input is directly selected.
When using input 4 (BS - output Brainstorm Infinityset), no tally to camera is available.

Brainstorm Tally Injector provides dry closure contacts for camera tally, when input 4 (BS) is selected.
Attached in between tally output from mixer and tally inputs at cameras.
Normal funtionality when using direct buttons remains available.

Based on WT32 - ETH board, containing an ESP32 microcontroller with Ethernet functionality.
![wt32-eth](https://github.com/Bavarialex/BrainstormTally/blob/main/pics/wt32-eth.jpg)

and a 4-relay board.

![relay](https://github.com/Bavarialex/BrainstormTally/blob/main/pics/relay.jpg)

Brainstorm Tally Injector is providing a web frontend to show current status:
![http01](https://github.com/Bavarialex/BrainstormTally/blob/main/pics/http01.JPG)

Enabling OTA updates:

![http02](https://github.com/Bavarialex/BrainstormTally/blob/main/pics/http02.JPG)

and serial output for debugging purposes:

![http03](https://github.com/Bavarialex/BrainstormTally/blob/main/pics/http03.JPG)

Pcb in use:

![pcb](https://github.com/Bavarialex/BrainstormTally/blob/main/pics/pcb.jpg)

How to flash WT32 - ETH for first time use is described ![here](https://werner.rothschopf.net/microcontroller/202401_esp32_wt32_eth01_en.htm).
