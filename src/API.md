# API Description

Description of the API endpoints.

mDNS support.

Simple http get calls. No authentication.


## BatmanLight


    /animation/{all, single, invert, kitt}


    /colorCode/{red, green, blue, yellow, cyan, magenta, black, white}


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

    /highlight/{[RGB as int]}

Highlight a intersection of some leds e.g. to focus on a segment. Start and end are the pixels itself, not a segment.

    /section/{start}/{end}/color/{[RGB as int]}

Turn all segments off

    /off

