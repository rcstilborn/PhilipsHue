# PhilipsHue Monitor

The challenge is to create a program which implements a simple integration with the Philips Hue platform.  It should be a simple terminal based program that will print out text based on the state of the lights.  When the program starts up, it should print out all lights and their state (we will restrict to only whether the lights are on/off, brightness, name, and ID).  The output should be formatted as JSON in the format of the following example:

```
[  
   {  
      "name":"Red Lamp",
      "id":"1",
      "on":true,
      "brightness":45
   },
   {  
      "name":"Green Lamp",
      "id":"2",
      "on":false,
      "brightness":100
   }
]
```
Your output does not need to be pretty printed like this, it can be a single line, however printing tab and newline formattedJSON will probably make debugging easier for yourself.  The “on” property is simply a boolean of whether the light is on or not.  The “brightness” property should be an integer (from 0 to 100) representing the % brightness of the light.

After printing out the initial state, your program should print out any changes in the lights’ state.  For example, if the Red Lamp turns off, you should print:
```
{  
   "id":"1",
   "on":false
}
```
If the Green Lamp is then turned on, and dimmed to 75%, you would print two changes:
```
{  
   "id":"2",
   "on":true
}
{  
   "id":"2",
   "brightness":75
}
```

## Dependencies
boost, ssl and gmock for testing


## Usage
'''
PhilipsHueMonitor -h | --help    : Print usage
PhilipsHueMonitor <IP address>   : Uses provided address to connect to bridge.
                                   Saves in config.ini if successful
PhilipsHueMonitor                : Uses address in config.ini if provided and 
                                   successful otherwise tries to discover bridge via:
                                   1. uPnP (note: not yet implemented!!)
                                   2. www.meethue.com/api/nupnp
                                      
Once connected it prints the current lights and polls the bridge every second to detect changes.
'''

## Limitations
1. The tool does not implement the uPnP discovery mechanism
2. This has only been tested against Bridge API version 1.21.0
3. The default (long) timeout is used for socket connections