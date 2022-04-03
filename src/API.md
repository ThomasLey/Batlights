# API Description

This document contains a brief summary of the containing projects and a brief API description. The project differ in WiFi support, either a wifi manager (batlights) or static wifi configuration (segmentlights).

Anyway, all projects support the zero conf protocol mDNS which provides a nice network name instead of an IP. In addition, the API is designed as an easy to integrate API for home automation and 3rd party apps.

_NIE_ tag is used as "not implemented exception, derived from C#, which means the function needs to be implemented, or easily spoken a __todo__.

Colors are provided as a 32bit integer that can be calculated with ˋcolor = (65536 * r) + (256 * g) + bˋ or [here](https://www.checkyourmath.com/convert/color/rgb_decimal.php).

## BatmanLight

**Abstract**: The _BatLights_ were initially designed as DND lights. My _BatLights_ hang above the door of my home office to indicate, if I am in a meeting or not.

_NIE_ Change the direction using a increment/decrement value.

Set the animation for the lights. _All_ means a static color, _single_ a few (_NIE_ a configuration) LEDS, _inverted_ a few leds wich are turned off and kitt, which is basically "single" but with more leds and a fade out at the other LEDS.

    /animation/{all, single, invert, kitt}

To change the color, BatLights support some common colors, which are at the moment the 255/0 combination of RGB. _NIE_ to use a full RGB color.

    /colorCode/{red, green, blue, yellow, cyan, magenta, black, white}

The speed code defines a string for different speeds from turtle (2s per increment) to fast (1ms). _NIE_ to use a numerical value for the speed in ms. 

    /speedCode/{turtle, veryslow, slow, medium, fast, veryfast, max}

## Segmented Lights

**Abstract** The segmented lights are designed to enlighten shelves in a cabinet. The entire light strips are divided into similar-sized segments which can be turned on and off with a single api call. The number of pixels per segment are hard coded since the wireing is fixed as well.

Turn a segment (multiple leds) on with a specific color. 

    /segment/{[id]}/color/{[RGB as int]}

_NIE_ Turn a segment exclusively on. All other segments are turned off with the background color.

    /xsegment/{[id]}/color/{[RGB as int]}

Turn all segments off with the given background color.

    /off

Turn all segments on with the given color.

    /on/{[RGB as int]}
    
 _NIE_ Have some configuration settings as for ˋSegmented Lightsˋ.

## Spinning Lights

__Abstract__ The spinning lights are designed to show some spinning around a static wheel. The number and the width of spinning segments can be defined through configuration. The speed is provided as rounds per second and limited to chip and led speed. If a absolute direction larger one (1) is used, the animation appears fast hence an animation is skipped.

Set speed to rounds per second. The animation delay is calculated.

    /speed/{[rps]}

_NIE_ Highlight a predefined segment which has its center at led zero (0).

    /highlight/{width}

_NIE_ Highlight a intersection of some leds e.g. to focus on a segment. Start and end are the pixels itself, not a segment.

    /section/{start}/{end}

Turn all segments off to the background color.

    /off

The following set entpoints can be used to change configuration settings. These settings are not persisted and resetted to default after restart.

    /setBackground/{color}
    /setForeground/{color}
    /setPps/{pixels per segment}
    /setNos/{number of segments}
    /setDirection/{increment, should be 1 or -1 (backwards), but can be any int}
