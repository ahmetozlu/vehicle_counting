# Vehicle Detection and Counting System for Two Way Road
This project focuses on detecting, tracking and counting vehicles by using "[Blob Detection](http://www.learnopencv.com/blob-detection-using-opencv-python-c/)" method. The unique ID is assigned to the each vehicle so it can not be counted more than once. This project was developed with OpenCV library (**[version 2.4.10](https://sourceforge.net/projects/opencvlibrary/files/opencv-win/2.4.10/)**).

## Theory
**What is Blob Detection?**

A Blob is a group of connected pixels in an image that share some common property ( E.g grayscale value ). In the image above, the dark connected regions are blobs, and the goal of blob detection is to identify and mark these regions.

**How does Blob detection work ?**

SimpleBlobDetector, as the name implies, is based on a rather simple algorithm described below. The algorithm is controlled by parameters ( shown in bold below )  and has the following steps. Scroll down to know how the parameters are set.

1. Thresholding : Convert the source images to several binary images by thresholding the source image with thresholds starting at minThreshold. These thresholds are incremented  by thresholdStep until maxThreshold. So the first threshold is minThreshold, the second is minThreshold + thresholdStep, the third is minThreshold + 2 x thresholdStep, and so on.

2. Grouping : In each binary image,  connected white pixels are grouped together.  Let’s call these binary blobs.

3. Merging  : The centers of the binary blobs in the binary images are computed, and  blobs located closer than minDistBetweenBlobs are merged.

4. Center & Radius Calculation :  The centers and radii of the new merged blobs are computed and returned.

## Project Demo
- The demo video is available on YouTube: https://youtu.be/3uMKK28bMuY

- The screenshot:
![screenshot_1](https://user-images.githubusercontent.com/22610163/28909069-89379050-782f-11e7-9051-1b6770cce191.png)

## Author
Ahmet Özlü

## License
This system is available under the MIT license. See the LICENSE file for more info.
