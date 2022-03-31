# API Description

Description of the API entpoints.

mDNS support.

Simple http get calls. No authentication.


## Batlights

__todo__

    /animation/{all, single, invert, kitt}


    /colorCode/{red, green, blue, yellow, cyan, magenta, black, white}


    /speedCode/{turtle, veryslow, slow, medium, fast, veryfast, max}

## Segmented Lights

Turn a segment (multiple leds) on with a specific color. Segments are hard coded and [num of segments] * [led per segment]will be calculated as max number of leds.

    /segment/{[id]}/color/{[RGB as int]}

Turn a segment excludivly on. All other segments are turned off.

    /xsegment/{[id]}/color/{[RGB as int]}

Turn all segments off

    /off


## Spinning Lights

Number of highlighted segments are hard coded. 

Background Color (non active color) is hard coded.

Set speed to rounds per second.

    /speed/{[rps]}

Highlight some leds e.g. to focus on a segment. Start and end is the pixels itself, not a segment.

    /highlight/{start}/{end}/color/{[RGB as int]}

Turn all segments off

    /off