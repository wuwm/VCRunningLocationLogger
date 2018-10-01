## Introduction


[![Pod Platform](http://img.shields.io/cocoapods/p/SDWebImage.svg?style=flat)](http://cocoadocs.org/docsets/SDWebImage/)
[![Pod License](http://img.shields.io/cocoapods/l/SDWebImage.svg?style=flat)](https://www.apache.org/licenses/LICENSE-2.0.html)

VCRunningLocationLogger is a high-accuracy GPS distance tool with Kalman filter and median filter for iOS. The accuray can be increased by over 20% compared to traditional Euclidean distance.

I created a zone(NSMutableArray) where every 5 location point is stored, remove the largest and lowest point, and calculate their average to keep the result steady.

Considering the compatibility of Android, I used C++ to implement Kalman Filter.

## Usage


QMRunningLocationManager.h includes detailed comments and examples on this library. It can be easily accessed by 'Option' key in Xcode.

QMKalmanFilter.h includes detailed comments and examples on Kalman filter.

The most common usage is like:

```
#import "QMRunningLocationManager.h"
```

## Parameters

- Parameters for Kalman Filter: Sigma rValue

- Zone size ratio: ratio

- Gloabal configuration: QMLocationManagerConfiguration


## Demo

It‘s accurate.

<p align="center" >
  <img src="Demo1.JPG" title="Optimized" float=left height="640" width="295">
</p>

<p align="center" >
  <img src="Demo2.PNG" title="Baseline" float=right height="600" width="710">
</p>

## License

MIT



