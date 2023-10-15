## Introduction
---
Tracking and estimating persistent items are essential to data-stream tasks. Due to the increasing volume of data stream, however, finding persistent items is becoming more and more challenging. Existing designs of finding persistent items can obtain high accuracy, but still need memory large enough to obtain acceptable performance. To achieve high precision and low error under small memory, we propose an accurate sketch DiceSketch, which greatly improves the accuracy of detecting persistent items and has the advantages of high memory efficiency and fast speed. Specifically, DiceSketch employs the selective persistence strategy to mitigate the over-estimation issue, and the time window strategy to reduce the de-duplication overhead for the same item in the same time window, therefore significantly improving memory efficiency. We implement the DiceSketch in a large-scale scenario to process persistent items detection and persistence estimation. The evaluation results show that, DiceSketch greatly improves the F1 score by 3.26x under small memory compared with the state-of-the-art solutions.

---
### Files
- dicesketch.cpp: example about implementation of dicesketch
---


### Compile and Run the examples
DiceSketch is implemented with C++. We show how to compile the examples on
Ubuntu with g++ and make.

#### Requirements
- Ensure __g++__ and __make__ are installed.  Our experimental platform is
  equipped with Ubuntu 18.04.3, g++ 7.5.0.

- Ensure the necessary library libpcap is installed.
    - It can be installed in most Linux distributions (e.g., apt-get install
      libpcap-dev in Ubuntu).

- Prepare the pcap files.
    - We provide two small pcap files in "data" folder for testing.  
    - Specify the path of each pcap file in "iptraces.txt". 
    - Note that one pcap file is regarded as all time windows in our examples. 
      

#### Run
- Run the examples, and the program will output some statistics about the detection accuracy. 

```
$ ./persistence
```




