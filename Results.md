
# Results of the Evaluation

## 1. Implementation Details

- **main().** The main() function has been changed such that we can run the actual implementation using a _process(.)_ function with different parameters. This makes running experiments easier. 

- **Experiment Selector.** There is a variable _SELECTOR_ in _main()_ that can be used to run Task 7, 8 or 9. Additionally, one can filter for output using _grep_ on the command line, e.g. `~/U/D/S/build> make && ./2D_feature_tracking | grep "TASK"`

- **Limitation BF matcher.** In function _matchDescriptors(.)_ in file _matching2D_Student.cpp_ we have implemented the BF matcher. Hamming distance is not working with SIFT detector so we choose NORM_L1 instead.

- **OpenCV Version.** The instantiation of SIFT depends on the OpenCV version. There are simple preprocessor conditions to pick the right one.

- **Detectors vs. Descriptors.** Some pairs of detectors and descriptors don't play well together (raised exceptions in OpenCV). This can be seen in _main()_ function where different combinations are skipped. I'd be happy to get more info about that as I could not figure out the problem.

## 2. Results

### MP.1 Data Buffer Optimization

Implementation of queue starts [here](https://github.com/MichaelSteurer/SFND_2D_Feature_Tracking/blob/submission/src/MidTermProject_Camera_Student.cpp#L131).

### MP.2 Keypoint Detection

Implementation [SHITOMASI](https://github.com/MichaelSteurer/SFND_2D_Feature_Tracking/blob/submission/src/matching2D_Student.cpp#L152), [HARRIS](https://github.com/MichaelSteurer/SFND_2D_Feature_Tracking/blob/submission/src/matching2D_Student.cpp#L187) and [MODERN](https://github.com/MichaelSteurer/SFND_2D_Feature_Tracking/blob/submission/src/matching2D_Student.cpp#L256).

### MP.3 Keypoint Removal

Filter non-car keypoints is implemented [here](https://github.com/MichaelSteurer/SFND_2D_Feature_Tracking/blob/submission/src/MidTermProject_Camera_Student.cpp#L182).

### MP.4 Keypoint Descriptors

Keypoint descriptors are implemented [here](https://github.com/MichaelSteurer/SFND_2D_Feature_Tracking/blob/submission/src/matching2D_Student.cpp#L67).

### MP.5 Descriptor Matching

FLANN matching can be found [here](https://github.com/MichaelSteurer/SFND_2D_Feature_Tracking/blob/submission/src/matching2D_Student.cpp#L31) and KNN matching is [here](https://github.com/MichaelSteurer/SFND_2D_Feature_Tracking/blob/submission/src/matching2D_Student.cpp#L48).

### MP.6 Descriptor Distance Ratio

KNN Distance ratio is implmented [here](https://github.com/MichaelSteurer/SFND_2D_Feature_Tracking/blob/submission/src/matching2D_Student.cpp#L54).


### MP.7 Performance Evaluation 1

Count the number of keypoints on the preceding vehicle for all 10 images and take note of the distribution of their neighborhood size. Do this for all the detectors you have implemented. 

#### SHITOMASI

Compared to the other algorithms we've got an average number of keypoints of 100. 
All dots have the same size. 

```
TASK 7: (img:0)  SHITOMASI Num:        125, Mean:          4, Std:          0
...
TASK 7: (img:9)  SHITOMASI Num:        112, Mean:          4, Std:          0
```

#### HARRIS

Very low number of keypoints compared to other algorithms.
Similar to the previous one, the size is always the same. Though, they are a little bit larger.

```
TASK 7: (img:0)     HARRIS Num:         17, Mean:          6, Std:          0
...
TASK 7: (img:9)     HARRIS Num:         34, Mean:          6, Std:          0
```

#### FAST

Comparably, a lot of keypoints detected. Size similar to the first two algorithms.

```
TASK 7: (img:0)       FAST Num:        419, Mean:          7, Std:          0
...
TASK 7: (img:9)       FAST Num:        401, Mean:          7, Std:          0
```

#### BRISK

For the first time, we've got different sizes. The mean of this size is around 21-22 with a standard deviation of ~14. 

```
TASK 7: (img:0)      BRISK Num:        254, Mean:    21.4408, Std:    14.4372
...
TASK 7: (img:9)      BRISK Num:        250, Mean:    22.2167, Std:     14.714
```

#### ORB

More than double the size on average compared to BRISK. Standard deviation ~25. Average number of keypoints.

```
TASK 7: (img:0)        ORB Num:         91, Mean:    57.1108, Std:    25.8496
...
TASK 7: (img:9)        ORB Num:        125, Mean:    54.4083, Std:    23.9525
```

#### AKAZE

Much smaller sizes and deviation. Slightly above average number of detected keypoints.

```
TASK 7: (img:0)      AKAZE Num:        162, Mean:    7.77008, Std:    3.95659
...
TASK 7: (img:9)      AKAZE Num:        175, Mean:    7.84846, Std:    3.61867
```

#### SIFT

Similar to AKAZE. 

```
TASK 7: (img:0)       SIFT Num:        137, Mean:    5.04017, Std:    5.95885
...
TASK 7: (img:9)       SIFT Num:        135, Mean:    5.67081, Std:    6.71983
```

--- 

> INTERPRETATION
>
> The experiment in task 7 clearly shows that the algorithms have significant differences in number of detected keypoints as well as the size and variation of the keypoint size. 

---

### MP.8 Performance Evaluation 1

The output of the program shows the number of matches per image frame. We do not sum them up as the number would then depend on the actual numbers of input frames. The full table can be found in APPENDIX A. Here is just a brief overview how the full list looks.


```
TASK 8: (img:1) Detector:  SHITOMASI, Descriptor:      BRISK, Kpt Matches: 95
TASK 8: (img:9) Detector:  SHITOMASI, Descriptor:      BRISK, Kpt Matches: 82

TASK 8: (img:1) Detector:  SHITOMASI, Descriptor:      BRIEF, Kpt Matches: 115
TASK 8: (img:9) Detector:  SHITOMASI, Descriptor:      BRIEF, Kpt Matches: 100

TASK 8: (img:1) Detector:  SHITOMASI, Descriptor:        ORB, Kpt Matches: 104
TASK 8: (img:9) Detector:  SHITOMASI, Descriptor:        ORB, Kpt Matches: 97
...
```

---

> INTERPRETATION
>
> - HARRIS detector has the lowest number of matches no matter which descriptor is used. It's only between 12 and 20 something.
> - FAST and BRISK detectors have by far the highest number of matches. 
> - ORB and SIFT detectors are also on the lower end regarding number of matches. 
> - AZAZE can be compared to BRISK reagarding the found matches.

---

### MP.9 Performance Evaluation 1

Comparing the runtime of the algorithms with different detector/descriptor combinations results in

```
TASK 9: Detector: ORB,        Descriptor: BRIEF,    t: 146.178 ms
TASK 9: Detector: HARRIS,     Descriptor: BRIEF,    t: 188.688 ms
TASK 9: Detector: HARRIS,     Descriptor: ORB,      t: 204.1 ms
TASK 9: Detector: ORB,        Descriptor: ORB,      t: 232.878 ms
TASK 9: Detector: SHITOMASI,  Descriptor: ORB,      t: 273.313 ms
TASK 9: Detector: SHITOMASI,  Descriptor: BRIEF,    t: 251.249 ms
TASK 9: Detector: ORB,        Descriptor: FREAK,    t: 359.637 ms
TASK 9: Detector: HARRIS,     Descriptor: BRISK,    t: 385.246 ms
TASK 9: Detector: HARRIS,     Descriptor: FREAK,    t: 414.224 ms
TASK 9: Detector: ORB,        Descriptor: BRISK,    t: 431.504 ms
TASK 9: Detector: SHITOMASI,  Descriptor: FREAK,    t: 491.064 ms
TASK 9: Detector: SHITOMASI,  Descriptor: BRISK,    t: 616.059 ms
TASK 9: Detector: BRISK,      Descriptor: BRIEF,    t: 1072.58 ms
TASK 9: Detector: AKAZE,      Descriptor: ORB,      t: 1155.89 ms
TASK 9: Detector: AKAZE,      Descriptor: BRIEF,    t: 1034.14 ms
TASK 9: Detector: BRISK,      Descriptor: ORB,      t: 1169.89 ms
TASK 9: Detector: BRISK,      Descriptor: BRISK,    t: 1310.22 ms
TASK 9: Detector: BRISK,      Descriptor: FREAK,    t: 1372.65 ms
TASK 9: Detector: SIFT,       Descriptor: BRIEF,    t: 1304.68 ms
TASK 9: Detector: AKAZE,      Descriptor: FREAK,    t: 1466.51 ms
TASK 9: Detector: SIFT,       Descriptor: FREAK,    t: 1545.38 ms
TASK 9: Detector: FAST,       Descriptor: ORB,      t: 1544.9 ms
TASK 9: Detector: AKAZE,      Descriptor: BRISK,    t: 1455.06 ms
TASK 9: Detector: FAST,       Descriptor: BRIEF,    t: 1680.58 ms
TASK 9: Detector: SIFT,       Descriptor: BRISK,    t: 1823.54 ms
TASK 9: Detector: FAST,       Descriptor: BRISK,    t: 1911.88 ms
TASK 9: Detector: FAST,       Descriptor: FREAK,    t: 1925.07 ms
TASK 9: Detector: AKAZE,      Descriptor: AKAZE,    t: 1817.84 ms
TASK 9: Detector: SIFT,       Descriptor: SIFT,     t: 2010.11 ms     
```

Hence, the top-3 combinations are 

  1. Detector: ORB,        Descriptor: BRIEF,    t: 146.178 ms
  2. Detector: HARRIS,     Descriptor: BRIEF,    t: 188.688 ms
  3. Detector: HARRIS,     Descriptor: ORB,      t: 204.1 ms


---

> INTERPRETATION
>
> The computational expensive task is the detection of keypoints. 
> - ORB and HARRIS are pretty fast.
> - FAST and SIFT are rather slow. 
> - SHITOMASE is a little above average, AKAZE a little below average

---

## A. APPENDIX
Full table of measure from Task 8.

```
TASK 8: (img:1) Detector:  SHITOMASI, Descriptor:      BRISK, Kpt Matches: 95
TASK 8: (img:9) Detector:  SHITOMASI, Descriptor:      BRISK, Kpt Matches: 82

TASK 8: (img:1) Detector:  SHITOMASI, Descriptor:      BRIEF, Kpt Matches: 115
TASK 8: (img:9) Detector:  SHITOMASI, Descriptor:      BRIEF, Kpt Matches: 100

TASK 8: (img:1) Detector:  SHITOMASI, Descriptor:        ORB, Kpt Matches: 104
TASK 8: (img:9) Detector:  SHITOMASI, Descriptor:        ORB, Kpt Matches: 97

TASK 8: (img:1) Detector:  SHITOMASI, Descriptor:      FREAK, Kpt Matches: 90
TASK 8: (img:9) Detector:  SHITOMASI, Descriptor:      FREAK, Kpt Matches: 84

TASK 8: (img:1) Detector:     HARRIS, Descriptor:      BRISK, Kpt Matches: 12
TASK 8: (img:9) Detector:     HARRIS, Descriptor:      BRISK, Kpt Matches: 21

TASK 8: (img:1) Detector:     HARRIS, Descriptor:      BRIEF, Kpt Matches: 14
TASK 8: (img:9) Detector:     HARRIS, Descriptor:      BRIEF, Kpt Matches: 23

TASK 8: (img:1) Detector:     HARRIS, Descriptor:        ORB, Kpt Matches: 12
TASK 8: (img:9) Detector:     HARRIS, Descriptor:        ORB, Kpt Matches: 20

TASK 8: (img:1) Detector:     HARRIS, Descriptor:      FREAK, Kpt Matches: 13
TASK 8: (img:9) Detector:     HARRIS, Descriptor:      FREAK, Kpt Matches: 18

TASK 8: (img:1) Detector:       FAST, Descriptor:      BRISK, Kpt Matches: 256
TASK 8: (img:9) Detector:       FAST, Descriptor:      BRISK, Kpt Matches: 247

TASK 8: (img:1) Detector:       FAST, Descriptor:      BRIEF, Kpt Matches: 320
TASK 8: (img:9) Detector:       FAST, Descriptor:      BRIEF, Kpt Matches: 307

TASK 8: (img:1) Detector:       FAST, Descriptor:        ORB, Kpt Matches: 306
TASK 8: (img:9) Detector:       FAST, Descriptor:        ORB, Kpt Matches: 304

TASK 8: (img:1) Detector:       FAST, Descriptor:      FREAK, Kpt Matches: 251
TASK 8: (img:9) Detector:       FAST, Descriptor:      FREAK, Kpt Matches: 246

TASK 8: (img:1) Detector:      BRISK, Descriptor:      BRISK, Kpt Matches: 168
TASK 8: (img:9) Detector:      BRISK, Descriptor:      BRISK, Kpt Matches: 182

TASK 8: (img:1) Detector:      BRISK, Descriptor:      BRIEF, Kpt Matches: 174
TASK 8: (img:9) Detector:      BRISK, Descriptor:      BRIEF, Kpt Matches: 179

TASK 8: (img:1) Detector:      BRISK, Descriptor:        ORB, Kpt Matches: 153
TASK 8: (img:9) Detector:      BRISK, Descriptor:        ORB, Kpt Matches: 171

TASK 8: (img:1) Detector:      BRISK, Descriptor:      FREAK, Kpt Matches: 154
TASK 8: (img:9) Detector:      BRISK, Descriptor:      FREAK, Kpt Matches: 165

TASK 8: (img:1) Detector:        ORB, Descriptor:      BRISK, Kpt Matches: 73
TASK 8: (img:9) Detector:        ORB, Descriptor:      BRISK, Kpt Matches: 90

TASK 8: (img:1) Detector:        ORB, Descriptor:      BRIEF, Kpt Matches: 49
TASK 8: (img:9) Detector:        ORB, Descriptor:      BRIEF, Kpt Matches: 65

TASK 8: (img:1) Detector:        ORB, Descriptor:        ORB, Kpt Matches: 65
TASK 8: (img:9) Detector:        ORB, Descriptor:        ORB, Kpt Matches: 89

TASK 8: (img:1) Detector:        ORB, Descriptor:      FREAK, Kpt Matches: 41
TASK 8: (img:9) Detector:        ORB, Descriptor:      FREAK, Kpt Matches: 53

TASK 8: (img:1) Detector:      AKAZE, Descriptor:      BRISK, Kpt Matches: 134
TASK 8: (img:9) Detector:      AKAZE, Descriptor:      BRISK, Kpt Matches: 141

TASK 8: (img:1) Detector:      AKAZE, Descriptor:      BRIEF, Kpt Matches: 137
TASK 8: (img:9) Detector:      AKAZE, Descriptor:      BRIEF, Kpt Matches: 150

TASK 8: (img:1) Detector:      AKAZE, Descriptor:        ORB, Kpt Matches: 127
TASK 8: (img:9) Detector:      AKAZE, Descriptor:        ORB, Kpt Matches: 144

TASK 8: (img:1) Detector:      AKAZE, Descriptor:      FREAK, Kpt Matches: 123
TASK 8: (img:9) Detector:      AKAZE, Descriptor:      FREAK, Kpt Matches: 135

TASK 8: (img:1) Detector:      AKAZE, Descriptor:      AKAZE, Kpt Matches: 135
TASK 8: (img:9) Detector:      AKAZE, Descriptor:      AKAZE, Kpt Matches: 148

TASK 8: (img:1) Detector:       SIFT, Descriptor:      BRISK, Kpt Matches: 61
TASK 8: (img:9) Detector:       SIFT, Descriptor:      BRISK, Kpt Matches: 83

TASK 8: (img:1) Detector:       SIFT, Descriptor:      BRIEF, Kpt Matches: 74
TASK 8: (img:9) Detector:       SIFT, Descriptor:      BRIEF, Kpt Matches: 88

TASK 8: (img:1) Detector:       SIFT, Descriptor:      FREAK, Kpt Matches: 63
TASK 8: (img:9) Detector:       SIFT, Descriptor:      FREAK, Kpt Matches: 83

TASK 8: (img:1) Detector:       SIFT, Descriptor:       SIFT, Kpt Matches: 83
TASK 8: (img:9) Detector:       SIFT, Descriptor:       SIFT, Kpt Matches: 99
```
