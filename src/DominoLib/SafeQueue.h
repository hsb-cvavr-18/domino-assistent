//
// Created by osboxes on 12/8/18.
//

#ifndef PROJECT_SAFEQUEUE_H
#define PROJECT_SAFEQUEUE_H

#include <stdio.h>
#include <iostream>
#include <mutex>
#include <queue>

#include <opencv2/core.hpp>

template<class T>
class SafeQueue {
    std::queue<T> q;
    std::mutex m;
public:
    SafeQueue() {}
    void push(T elem) {
        std::lock_guard<std::mutex> lock(m);
        q.push(elem);
    }

    bool next(T& elem) {
        std::lock_guard<std::mutex> lock(m);
        if (q.empty()) {
            return false;
        }
        elem = q.front();
        q.pop();
        return true;
    }
};

static SafeQueue<cv::Mat> previewFrames;
static SafeQueue<cv::Mat> gameFrames;

#endif //PROJECT_SAFEQUEUE_H
