# Gravity Models
These files give the data for various gravity models.  It's 
necessary to download and install at least one of these files if you 
want to use the GravityModel class in GeographicLib.

## Earth Gravity Model (EGM)
See [Gravity Models](https://geographiclib.sourceforge.io/html/gravity.html)
for a description of Gravity models and the supported earth gravity models. 

## Installation of Additional Models
1. The following EGM gravity models are supported
* [EGM84](https://sourceforge.net/projects/geographiclib/files/gravity-distrib/egm84.zip) (27 kB)
* **_[EGM96](https://sourceforge.net/projects/geographiclib/files/gravity-distrib/egm96.zip) (2.1 MB) - INSTALLED BY DEFAULT_**
* [EGM2008](https://sourceforge.net/projects/geographiclib/files/gravity-distrib/egm2008.zip) (74 MB)

2. Once downloaded, extract the contents (2 files egm*.egm, egm*.egm.cof) 
to this directory.

3. Update the planet's data file [here](../../addons/Core_Celestial_Bodies/bodies/earth.json) 
with the name of the new EGM gravity model in the "gravityModel" field.