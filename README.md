This is unoffical fork of https://github.com/abreheret/PixelAnnotationTool

Based on fork: https://github.com/UndeadBlow/PixelAnnotationTool/releases/tag/v_1.6_neural

PixelAnnotationTool
============================

Software that allows you to manually and quickly annotate images in directories.
The method is pseudo manual because it uses the algorithm [watershed marked](http://docs.opencv.org/3.1.0/d7/d1b/group__imgproc__misc.html#ga3267243e4d3f95165d55a618c65ac6e1) of OpenCV. The general idea is to manually provide the marker with brushes and then to launch the algorithm. If at first pass the segmentation needs to be corrected, the user can refine the markers by drawing new ones on the erroneous areas (as shown on video below).

[![gif_file](giphy.gif)](https://youtu.be/wxi2dInWDnI)

Example :

<img src="https://raw.githubusercontent.com/abreheret/PixelAnnotationTool/master/images_test/Abbey_Road.jpg" width="300"/> <img src="https://raw.githubusercontent.com/abreheret/PixelAnnotationTool/master/images_test/Abbey_Road_color_mask.png" width="300"/>

----------

Versions:


**1.3beta**

* Added ability to load and save JSON with labels again.
* Last loaded JSON will be saved and automatically loaded on start.
* Segmentation saves and loads from color mask only, no ID mask used.
* You can navigate images with Q and E (previous and next image), even if multiply directories opened.

----------


### Building Dependencies :
* [Qt](https://www.qt.io/download-open-source/)  >= 5.x
* [CMake](https://cmake.org/download/) >= 2.8.x 
* [OpenCV](http://opencv.org/releases.html) >= 2.4.x 
* For Windows Compiler : Works under Visual Studio >= 2015

### Download binaries :
Go to release [page](https://github.com/UndeadBlow/PixelAnnotationTool/releases)

### License :

GNU Lesser General Public License v3.0 

Permissions of this copyleft license are conditioned on making available complete source code of licensed works and modifications under the same license or the GNU GPLv3. Copyright and license notices must be preserved. Contributors provide an express grant of patent rights. However, a larger work using the licensed work through interfaces provided by the licensed work may be distributed under different terms and without source code for the larger work.

[more](https://github.com/abreheret/PixelAnnotationTool/blob/master/LICENSE)
