# __WireWayContact__
![Static Badge](https://img.shields.io/badge/C%2B%2B-v20-red?labelColor=gray&color=orange) ![Static Badge](https://img.shields.io/badge/Qt-v6.5.2-blue?color=green)

__WireWayContact__  - this application is part of the WireWay software system that allows the user to give access to control his own computercontacts

<br/>
Important! This version of the application is developed only for Windows

<br/>

## __Build__
---
__Before building, make sure you have the right version of the language and Qt__ 

```cmd
mkdir build 
cd build
cmake ..
cmake --build .
```

<br/>
<br/>

## __Specification__
---
The application allows the user to give access to their own computer. B connects to the WireWay server. There the user is listed as a contact to which you can connect at any time

When connected, the program generates several SCREEN packets every 5 milliseconds, which contain parts of the computer screen. Before that, the program saves a screenshot of the screen in JPEG format. The program also processes incoming COMMAND packets and executes their contents.

<br/>

___P.s. I note that the contact always knows whether they have connected to it, for this a special informing window appears indicating this___

## __Extra__
---
Description of package functions can be found [___here___]()