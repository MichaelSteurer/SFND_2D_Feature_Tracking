/* INCLUDES FOR THIS PROJECT */
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <vector>
#include <cmath>
#include <limits>
#include <numeric>
#include <opencv2/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/features2d.hpp>
#include <opencv2/xfeatures2d.hpp>
#include <opencv2/xfeatures2d/nonfree.hpp>

#include "dataStructures.h"
#include "matching2D.hpp"

using namespace std;

bool DEBUG = true;

int process(string detectorType, string descriptorType, int selector);


/* MAIN PROGRAM */
int main(int argc, const char *argv[])
{
    cout << "OpenCV version : " << CV_VERSION << endl;
    cout << "Major version : " << CV_MAJOR_VERSION << endl;
    cout << "Minor version : " << CV_MINOR_VERSION << endl;
    cout << "Subminor version : " << CV_SUBMINOR_VERSION << endl;
	
    vector<string> detectorTypes;
    vector<string> descriptorTypes;

    int SELECTOR = 8; // 7, 8, 9

    if (SELECTOR == 7)
    {
        ///////////////////////////////////////////
        // TASK 7
        detectorTypes = {"SHITOMASI", "HARRIS", "FAST", "BRISK", "ORB", "AKAZE", "SIFT"};
        descriptorTypes = {""};
        // TASK 7
        ///////////////////////////////////////////
    } else if (SELECTOR == 8 || SELECTOR == 9)
    {
        ///////////////////////////////////////////
        // TASK 8
        detectorTypes = {"SHITOMASI", "HARRIS", "FAST", "BRISK", "ORB", "AKAZE", "SIFT"};
        descriptorTypes = {"BRISK", "BRIEF", "ORB", "FREAK", "AKAZE", "SIFT"};
        // TASK 8
        ///////////////////////////////////////////
    }
    
    for (int detectorTypeIterator = 0; detectorTypeIterator < detectorTypes.size(); detectorTypeIterator++)
    {
        for (int descriptorTypeIterator = 0; descriptorTypeIterator < descriptorTypes.size(); descriptorTypeIterator++)
        {
            string detectorType = detectorTypes[detectorTypeIterator];
            string descriptorType = descriptorTypes[descriptorTypeIterator];
            cout << "DEBUG " << detectorType << "  " << descriptorType << endl;
            if (
                (descriptorType == "AKAZE" && detectorType != "AKAZE") ||  // descriptor AKAZE requires detector AKAZE
                (descriptorType == "SIFT" && detectorType != "SIFT") ||  // descriptor SIFT only with detector sift
                (descriptorType == "ORB" && detectorType == "SIFT")  // detector SIFT doesn't play well with descriptor ORB
            )
            {
                continue;
            }

            double t = (double)cv::getTickCount();
            process(detectorType, descriptorType, SELECTOR);
            t = ((double)cv::getTickCount() - t) / cv::getTickFrequency();
            if (SELECTOR == 9)
            {
                cout << "TASK 9: " << "Detector: " << setw(10) << detectorType << ", Descriptor: " << setw(10) << descriptorType << ", t: " << 1000 * t / 1.0 << " ms" << endl;
            }

        }
    }
    return 0;
}

int process(string detectorType, string descriptorType, int selector) 
{
    /* INIT VARIABLES AND DATA STRUCTURES */

    // data location
    string dataPath = "../";

    // camera
    string imgBasePath = dataPath + "images/";
    string imgPrefix = "KITTI/2011_09_26/image_00/data/000000"; // left camera, color
    string imgFileType = ".png";
    int imgStartIndex = 0; // first file index to load (assumes Lidar and camera names have identical naming convention)
    int imgEndIndex = 9;   // last file index to load
    int imgFillWidth = 4;  // no. of digits which make up the file index (e.g. img-0001.png)

    // misc
    int dataBufferSize = 2;       // no. of images which are held in memory (ring buffer) at the same time
    vector<DataFrame> dataBuffer; // list of data frames which are held in memory at the same time
    bool bVis = false;            // visualize results

    /* MAIN LOOP OVER ALL IMAGES */

    for (size_t imgIndex = 0; imgIndex <= imgEndIndex - imgStartIndex; imgIndex++)
    {
        /* LOAD IMAGE INTO BUFFER */

        // assemble filenames for current index
        ostringstream imgNumber;
        imgNumber << setfill('0') << setw(imgFillWidth) << imgStartIndex + imgIndex;
        string imgFullFilename = imgBasePath + imgPrefix + imgNumber.str() + imgFileType;

        // load image from file and convert to grayscale
        cv::Mat img, imgGray;
        img = cv::imread(imgFullFilename);
        cv::cvtColor(img, imgGray, cv::COLOR_BGR2GRAY);

        //// STUDENT ASSIGNMENT
        //// TASK MP.1 -> replace the following code with ring buffer of size dataBufferSize

        // push image into data frame buffer
        DataFrame frame;
        frame.index = imgIndex;
        frame.cameraImg = imgGray;

        // Implement ring buffer with fixed size.
        if (dataBuffer.size() >= dataBufferSize)  // remove first element if
        {                                         // the buffer size is exceeded
            dataBuffer.erase(dataBuffer.begin());
        }
        dataBuffer.push_back(frame);              // append new element at the end

        if (DEBUG)
        {
            cout << "DEBUG: Push new element  ";
            for(int i = 0; i < dataBuffer.size(); i++)
            {
                cout << dataBuffer[i].index << "  ";
            }
            cout << endl;
        }

        //// EOF STUDENT ASSIGNMENT
        cout << "#1 : LOAD IMAGE INTO BUFFER done" << endl;

        /* DETECT IMAGE KEYPOINTS */

        // extract 2D keypoints from current image
        vector<cv::KeyPoint> keypoints; // create empty feature list for current image


        bool bVis = false;
        if (detectorType.compare("SHITOMASI") == 0)
        {
            detKeypointsShiTomasi(keypoints, imgGray, bVis);
        }
        else if (detectorType.compare("HARRIS") == 0)
        {
            detKeypointsHarris(keypoints, imgGray, bVis);
        }
        else
        {
            detKeypointsModern(keypoints, imgGray, detectorType, bVis);
        }

        if (keypoints.size() == 0)
        {
            cout << "Algorithm to detect image keypoints not found" << endl;
            return -1;
        }
        //// EOF STUDENT ASSIGNMENT

        //// STUDENT ASSIGNMENT
        //// TASK MP.3 -> only keep keypoints on the preceding vehicle

        // only keep keypoints on the preceding vehicle
        bool bFocusOnVehicle = true;
        cv::Rect vehicleRect(535, 180, 180, 150);
        if (bFocusOnVehicle)
        {
            for (vector<cv::KeyPoint>::iterator it = keypoints.begin(); it != keypoints.end();)
            {
                if(it->pt.inside(vehicleRect))
                {
                    ++it;
                }
                else
                {
                    it = keypoints.erase(it);
                }
            }
        }

        vector<float> keypointsSize;
        for (vector<cv::KeyPoint>::iterator it = keypoints.begin(); it != keypoints.end(); it++)
        {
            keypointsSize.push_back(it->size);
        }

        //////////////////////////////////////////////
        // TASK 7
        // Taken from https://stackoverflow.com/a/7616783

        double sum = std::accumulate(keypointsSize.begin(), keypointsSize.end(), 0.0);
        double mean = sum / keypointsSize.size();

        std::vector<double> diff(keypointsSize.size());
        std::transform(keypointsSize.begin(), keypointsSize.end(), diff.begin(),
                        std::bind2nd(std::minus<double>(), mean));
        double sq_sum = std::inner_product(diff.begin(), diff.end(), diff.begin(), 0.0);
        double stdev = std::sqrt(sq_sum / keypointsSize.size());

        if (selector == 7)
        {
            cout << "TASK 7: (img:" << imgIndex << ") " << setw(10) << detectorType << " Num: " << setw(10) << keypointsSize.size() << ", Mean: " << setw(10) << mean << ", Std: " << setw(10) << stdev << endl;
        }


        // TASK 7
        //////////////////////////////////////////////

        // optional : limit number of keypoints (helpful for debugging and learning)
        bool bLimitKpts = false;
        if (bLimitKpts)
        {
            int maxKeypoints = 50;

            if (detectorType.compare("SHITOMASI") == 0)
            { // there is no response info, so keep the first 50 as they are sorted in descending quality order
                keypoints.erase(keypoints.begin() + maxKeypoints, keypoints.end());
            }
            cv::KeyPointsFilter::retainBest(keypoints, maxKeypoints);
            cout << " NOTE: Keypoints have been limited!" << endl;
        }

        // push keypoints and descriptor for current frame to end of data buffer
        (dataBuffer.end() - 1)->keypoints = keypoints;
        cout << "#2 : DETECT KEYPOINTS done" << endl;

        /* EXTRACT KEYPOINT DESCRIPTORS */

        //// STUDENT ASSIGNMENT
        //// TASK MP.4 -> add the following descriptors in file matching2D.cpp and enable string-based selection based on descriptorType
        //// -> BRIEF, ORB, FREAK, AKAZE, SIFT

        cv::Mat descriptors;
        descKeypoints((dataBuffer.end() - 1)->keypoints, (dataBuffer.end() - 1)->cameraImg, descriptors, descriptorType);
        //// EOF STUDENT ASSIGNMENT

        // push descriptors for current frame to end of data buffer
        (dataBuffer.end() - 1)->descriptors = descriptors;

        cout << "#3 : EXTRACT DESCRIPTORS done" << endl;

        if (dataBuffer.size() > 1) // wait until at least two images have been processed
        {

            /* MATCH KEYPOINT DESCRIPTORS */

            vector<cv::DMatch> matches;
            string matcherType; // MAT_BF, MAT_FLANN
            matcherType = "MAT_BF";
            // matcherType = "MAT_FLANN";

            string descriptorType = "DES_BINARY"; // DES_BINARY, DES_HOG

            string selectorType;       // SEL_NN, SEL_KNN
            // selectorType = "SEL_NN";
            selectorType = "SEL_KNN";

            //// STUDENT ASSIGNMENT
            //// TASK MP.5 -> add FLANN matching in file matching2D.cpp
            //// TASK MP.6 -> add KNN match selection and perform descriptor distance ratio filtering with t=0.8 in file matching2D.cpp

            matchDescriptors((dataBuffer.end() - 2)->keypoints, (dataBuffer.end() - 1)->keypoints,
                             (dataBuffer.end() - 2)->descriptors, (dataBuffer.end() - 1)->descriptors,
                             matches, descriptorType, matcherType, selectorType, detectorType);

            //// EOF STUDENT ASSIGNMENT

            // store matches in current data frame
            (dataBuffer.end() - 1)->kptMatches = matches;

            cout << "#4 : MATCH KEYPOINT DESCRIPTORS done" << endl;

            // visualize matches between current and previous image
            bVis = false;
            if (bVis)
            {
                cv::Mat matchImg = ((dataBuffer.end() - 1)->cameraImg).clone();
                cv::drawMatches((dataBuffer.end() - 2)->cameraImg, (dataBuffer.end() - 2)->keypoints,
                                (dataBuffer.end() - 1)->cameraImg, (dataBuffer.end() - 1)->keypoints,
                                matches, matchImg,
                                cv::Scalar::all(-1), cv::Scalar::all(-1),
                                vector<char>(), cv::DrawMatchesFlags::DRAW_RICH_KEYPOINTS);

                string windowName = "Matching keypoints between two camera images";
                cv::namedWindow(windowName, 7);
                cv::imshow(windowName, matchImg);
                cout << "Press key to continue to next image" << endl;
                cv::waitKey(0); // wait for key to be pressed
            }
            bVis = false;
        }
 
        if (selector == 8)
        {
            cout << "TASK 8: (img:" << imgIndex << ") " << "Detector: " << setw(10) << detectorType << ", Descriptor: " << setw(10) << descriptorType << ", Kpt Matches: " << (dataBuffer.end() - 1)->kptMatches.size() << endl;
        }
 
    } // eof loop over all images

    return 0;
}
