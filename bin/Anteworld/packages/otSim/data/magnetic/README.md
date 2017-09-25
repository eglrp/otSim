#Geomagnetic Models
These files give the data for various geomagnetic models.  It's 
necessary to download and install at least one of these files if you 
want to use the MagneticModel class in GeographicLib.

##World Magnetic Model (wmm), International Geomagnetic Reference Field (igrf), Enhanced Magnetic Model (emm)
See [Magnetic Models](https://geographiclib.sourceforge.io/html/magnetic.html)
for a description of Magnetic models and the supported wmm, igrf, or emm models. 

##Installation of Additional Models
1. The following magnetic models are supported
..* **_[wmm2010](https://sourceforge.net/projects/geographiclib/files/magnetic-distrib/wmm2010.zip) (3 kB) - INSTALLED BY DEFAULT_**
..* **_[wmm2015](https://sourceforge.net/projects/geographiclib/files/magnetic-distrib/wmm2015.zip (3 kB) - INSTALLED BY DEFAULT_**
..* **_[igrf11](https://sourceforge.net/projects/geographiclib/files/magnetic-distrib/igrf11.zip) (25 kB) - INSTALLED BY DEFAULT_**
..* **_[igrf12](https://sourceforge.net/projects/geographiclib/files/magnetic-distrib/igrf12.zip) (26 kB) - INSTALLED BY DEFAULT_**
..* [emm2010](https://sourceforge.net/projects/geographiclib/files/magnetic-distrib/emm2010.zip) (4.3 MB)
..* [emm2015](https://sourceforge.net/projects/geographiclib/files/magnetic-distrib/emm2015.zip) (4.2 MB)
..* [emm2017](https://sourceforge.net/projects/geographiclib/files/magnetic-distrib/emm2017.zip) (4.8 MB)

2. Once downloaded, extract the contents (2 files *.wmm, *.wmm.cof) 
to this directory.

3. Update the planet's data file [here](../../addons/Core_Celestial_Bodies/bodies/earth.json) 
with the name of the new magnetic model in the "magneticModel" field.