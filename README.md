
# Vehicle Detection, Tracking and Counting
This project focuses on detecting, tracking and counting vehicles by using "[Blob Detection](http://www.learnopencv.com/blob-detection-using-opencv-python-c/)" method. The unique ID is assigned to the each vehicle so it can not be counted more than once. This project was developed by OpenCV library (**[version 2.4.10](https://sourceforge.net/projects/opencvlibrary/files/opencv-win/2.4.10/)**).

<p align="center">
  <img src="https://user-images.githubusercontent.com/22610163/30249200-efa2b594-963f-11e7-8c3e-b378cbf49101.gif">
</p>

The accuray is approximately 95% and developing of the accuracy is in progress.

## Theory
Target representation and localization is mostly a bottom-up process. These methods give a variety of tools for identifying the moving object. Locating and tracking the target object successfully is dependent on the algorithm. For example, using blob tracking is useful for identifying, tracking human/vehicle movement because a person's/vehicle's profile changes dynamically. Typically the computational complexity for these algorithms is low. The project utlizes the [Blob Tracking](https://github.com/ahmetozlu/vehicle_counting/blob/master/VehicleDetectionAndCounting/Blob.cpp) module which provides various parameters for connecting blobs on video frames in order to determine movements of vehicles. 

**What is Blob Detection?**

A Blob is a group of connected pixels in an image that share some common property ( E.g grayscale value ). Blob detection methods are aimed at detecting regions in a digital image that differ in properties, such as brightness or color, compared to surrounding regions. Informally, a blob is a region of an image in which some properties are constant or approximately constant; all the points in a blob can be considered in some sense to be similar to each other. The most common method for blob detection is convolution.

**How does Blob detection work ?**

The algorithm for extracting blobs from an image:

1. Convert the source image to binary images by applying thresholding with several thresholds from minThreshold (inclusive) to maxThreshold (exclusive) with distance thresholdStep between neighboring thresholds.

2. Extract connected components from every binary image by findContours and calculate their centers.

3. Group centers from several binary images by their coordinates. Close centers form one group that corresponds to one blob, which is controlled by the minDistBetweenBlobs parameter.

4. From the groups, estimate final centers of blobs and their radiuses and return as locations and sizes of keypoints.

## Project Demo
- The demo video is available on YouTube: https://youtu.be/3uMKK28bMuY

## Installation


**Building the project using CMake from the command-line:**

Linux:

    export OpenCV_DIR="~/OpenCV/build"
    mkdir build
    cd build
    cmake -D OpenCV_DIR=$OpenCV_DIR ..
    make 

MacOSX (Xcode):

    export OpenCV_DIR="~/OpenCV/build"
    mkdir build
    cd build
    cmake -G Xcode -D OpenCV_DIR=$OpenCV_DIR ..    

Windows (MS Visual Studio):

    set OpenCV_DIR="C:\OpenCV\build"
    mkdir build
    cd build
    cmake -G "Visual Studio 12 2013" -D OpenCV_DIR=%OpenCV_DIR% ..  

## Citation
If you use this code for your publications, please cite it as:

    @ONLINE{vdtc,
        author = "Ahmet Özlü",
        title  = "Vehicle Detection, Tracking and Counting",
        year   = "2017",
        url    = "https://github.com/ahmetozlu/vehicle_counting"
    }

## Author
Ahmet Özlü

## License
This system is available under the MIT license. See the LICENSE file for more info.
