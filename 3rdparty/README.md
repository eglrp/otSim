# 3rd Party Libraries

Required 3rd party libraries go here

## Outerra API
The [Outerra API](https://github.com/Outerra/anteworld) must be installed 
here with the folder name "ot-api"
The reason for not redistributing the Outerra API in this package 
is that it is expected to be updated often via its github page, so
redistributing it again here for each update would be unnecessary.

## Independent Solutions (Shared Libraries)
These libraries have their own VisualStudio solution and 
must be built independently prior to building of otSim.
* [GeographicLib](https://geographiclib.sourceforge.io/) -> GeographicLib\windows\GeographicLib\GeographicLib.sln
* [jsoncpp](https://github.com/open-source-parsers/jsoncpp) -> jsoncpp\jsoncpp\jsoncpp.sln

## Integrated Libraries (Static Libraries)
These libraries are integrated into one or more of otSim's 
projects and will be built automatically by otSim compilation.
* [Splines](https://github.com/ebertolazzi/Splines)
* [tinymath](https://sourceforge.net/projects/tinymath/)