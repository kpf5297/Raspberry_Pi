// How to build in linux on raspberry pi

Update libraries and install dev tools:


	sudo apt update
	sudo apt dist-upgrade 
	sudo reboot

*********** PER COURSE MATERIALS *************
# Development version of the X C Bindings library extension for Xinerama:
sudo apt install libxcb-xinerama0-dev
# Commonly used for multi-monitor support in X Window System applications.

# Provides development files for the Mesa OpenGL graphics library:
sudo apt-get install libgl1-mesa-dev
# Commonly used for rendering 2D and 3D graphics in applications that use OpenGL.

# Install essential tools for building software:
sudo apt-get install build-essential
# Commonly used for compiling and building software from source code.

# Install development libraries and dependencies for X11 and xcb:
sudo apt-get install libx11-dev libxcb1-dev libxkbcommon-x11-dev libx11-xcb-dev libxext-dev
# Commonly used for developing applications that use the X Window System and xcb protocol.

# Install the Qt5 make tool:
sudo apt-get install qt5-make
# Commonly used in the build process for Qt-based applications.

# Install default Qt5 development tools:
sudo apt-get install qt5-default
# Commonly used for Qt application development.


*********** PER Research  *************

# Additional Qt-related packages:
sudo apt-get install qtbase5-dev qtchooser
# Development packages for the Qt base library and Qt chooser tool.

sudo apt-get install qtbase5-dev-tools
# Development tools for the Qt base library.

sudo apt-get install qtcreator
# Integrated Development Environment (IDE) for Qt development.

sudo apt-get install qtdeclarative5-dev
# Development package for Qt Declarative, used for QML (Qt Meta-Object Language) development.

# Additional commonly used Qt libraries:

# Qt Multimedia:
sudo apt-get install qtmultimedia5-dev
# Adds support for multimedia features in Qt applications, including audio and video playback.

# Qt SQL:
sudo apt-get install libqt5sql5 libqt5sql5-sqlite
# Adds SQL database support to Qt applications.

# Qt WebEngine:
sudo apt-get install libqt5webengine5 libqt5webengine5-dev
# Enables the integration of web content in Qt applications.

# Qt Charts:
sudo apt-get install libqt5charts5 libqt5charts5-dev
# Adds support for creating interactive charts in Qt applications.

# Qt Location:
sudo apt-get install qtpositioning5-dev
# Provides APIs for location-based services.

# Qt Network:
sudo apt-get install libqt5network5 libqt5network5-dev
# Adds network-related functionality to Qt applications.

# Qt Test:
sudo apt-get install libqt5test5
# Includes classes for writing unit tests for Qt applications.

# Reboot the system (if necessary after installing certain updates or libraries):
sudo reboot
# This step may be required to apply changes to the system, especially after installing certain low-level libraries or kernel updates.


*********** To Build  *************
Under desktop select GCC compilers for both.

Edit the *.pro file to include:
LIBS += -lgpiod -lrt -lpthread

