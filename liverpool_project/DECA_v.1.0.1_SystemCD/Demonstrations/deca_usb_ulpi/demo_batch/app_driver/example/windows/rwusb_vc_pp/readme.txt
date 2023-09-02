========================================================================
       CONSOLE APPLICATION : SLSUSBClient
========================================================================


AppWizard has created this SLSUSBClient application for you.  

This file contains a summary of what you will find in each of the files that
make up your SLSUSBClient application.

SLSUSBClient.dsp
    This file (the project file) contains information at the project level and
    is used to build a single project or subproject. Other users can share the
    project (.dsp) file, but they should export the make files locally.

SLSUSBClient.cpp
    This is the main application source file.


/////////////////////////////////////////////////////////////////////////////
Other standard files:

StdAfx.h, StdAfx.cpp
    These files are used to build a pre compiled header (PCH) file
    named SLSUSBClient.pch and a recompiled types file named StdAfx.obj.


/////////////////////////////////////////////////////////////////////////////
Other notes:

The "Read Data (Bulk IN)" and "Write Data (Bulk OUT)" modules in the application
will give the USB 2.0 performance speed in Mbits/s.  In order to measure proper 
performance data, the data transfer (read or write) should last for at least 2 ms.
This could be achieved by any combination of "number of times to read/write" and 
"number of bytes to read/write".

AppWizard uses "TODO:" to indicate parts of the source code you
should add to or customize.

/////////////////////////////////////////////////////////////////////////////
How to use:
- Click on the application exe which will ask to enter the selection.
- Enter any selection and it will ask you to do further steps.
- To continue this application, just press 'n' when it asks about "Y:N"
- You can use streaming template for NIOS side application.