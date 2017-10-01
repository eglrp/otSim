# Geoid Models
These files give the height of geoid above the ellipsoid on a 
rectangular grid.  It's necessary to download and install at least one 
of these files if you want to use the Geoid class in GeographicLib.

## Earth Gravity Model (EGM)
See [Geoid Height](https://geographiclib.sourceforge.io/html/geoid.html) 
for a description of Geoid height and the supported earth gravity models. 

## Installation of Additional Models
1. The following EGM models are supported
* [EGM84-30](https://sourceforge.net/projects/geographiclib/files/geoids-distrib/egm84-30.zip) (0.6 MB)
* [EGM84-15](https://sourceforge.net/projects/geographiclib/files/geoids-distrib/egm84-15.zip) (2.1 MB)
* [EGM96-15](https://sourceforge.net/projects/geographiclib/files/geoids-distrib/egm96-15.zip) (2.1 MB)
* **_[EGM96-5](https://sourceforge.net/projects/geographiclib/files/geoids-distrib/egm96-5.zip) (19 MB) - INSTALLED BY DEFAULT_**
* [EGM2008-5](https://sourceforge.net/projects/geographiclib/files/geoids-distrib/egm2008-5.zip) (19 MB)
* [EGM2008-2_5](https://sourceforge.net/projects/geographiclib/files/geoids-distrib/egm2008-2_5.zip) (75 MB)
* [EGM2008-1](https://sourceforge.net/projects/geographiclib/files/geoids-distrib/egm2008-1.zip) (470 MB)

2. Once downloaded, extract the contents (3 files egm*.pgm, egm*.pgmaux.xml, egm*.wld) 
to this directory.

3. Update the planet's data file [here](../../addons/Core_Celestial_Bodies/bodies/earth.json) 
with the name of the new EGM model in the "geoidModel" field.