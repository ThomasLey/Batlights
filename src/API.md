# API Description

This document contains a brief summary of the containing projects and a brief API description. The project differ in WiFi support, either a wifi manager (batlights) or static wifi configuration (segmentlights).

Anyway, all projects support the zero conf protocol mDNS which provides a nice network name instead of an IP. In addition, the API is designed as an easy to integrate API for home automation and 3rd party apps.

__NIE__ tag is used as "not implemented exception, derived from C#, which means the function needs to be implemented, or easily spoken a __todo__.

## BatmanLight

The __BatLights__ were initially designed as DND lights. My __BatLights__ hang above the door of my home office to indicate, if I am in a meeting or not.

__NIE__ Change the direction using a increment/decrement value.

Set the animation for the lights. __All__ means a static color, single a few (__NIE__ a configuration) LEDS, inverted a few leds wich are turned off and kitt, which is basically "single" but with more leds and a fade out at the other LEDS.

    /animation/{all, single, invert, kitt}

To change the color, BatLights support some common colors, which are at the moment the 255/0 combination of RGB. __NIE__ to use a full RGB color.

    /colorCode/{red, green, blue, yellow, cyan, magenta, black, white}

The speed code defines a string for different speeds from turtle (2s per increment) to fast (1ms). __NIE__ to use a numerical value for the speed in ms. 

    /speedCode/{turtle, veryslow, slow, medium, fast, veryfast, max}

## Segmented Lights

Turn a segment (multiple leds) on with a specific color. Segments are hard coded and [num of segments] * [led per segment]will be calculated as max number of leds.

    /segment/{[id]}/color/{[RGB as int]}

Turn a segment exclusively on. All other segments are turned off.

    /xsegment/{[id]}/color/{[RGB as int]}

Turn all segments off

    /off

Turn all segments on with the given colour

    /on/{[RGB as int]}


## Spinning Lights

Number of highlighted segments are hard coded. 

Background Color (non active color) is hard coded.

Set speed to rounds per second.

    /speed/{[rps]}

Highlight a predefined segment (top x leds)

    /highlight/

Highlight a intersection of some leds e.g. to focus on a segment. Start and end are the pixels itself, not a segment.

    /section/{start}/{end}

Turn all segments off

    /off


The following set entpoints can be used

    /setBackground/{color}
    /setForeground/{color}
    /setPps/{pixels per segment}
    /setNos/{number of segments}
    /setDirection/{increment, should be 1 or -1 (backwards), but can be any int}
