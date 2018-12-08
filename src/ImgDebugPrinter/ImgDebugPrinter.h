//
// Created by osboxes on 11/9/18.
//

#ifndef PROJECT_IMGDEBUGPRINTER_H
#define PROJECT_IMGDEBUGPRINTER_H

#include <iostream>
#include <map>

#include <opencv2/core/mat.hpp>
#include <opencv2/highgui.hpp>

using namespace std;

class AbstractImgDebugPrinter {
public:
    void printImage(std::string windowName, cv::Mat img) {

        auto search = frameCount.find(windowName);
        frameCount.insert(std::pair<string, int>(windowName, 0));
        int index = 0; //TODO

        if (search != frameCount.end()) {
            // Found
            showImage(windowName, index, img);
        } else {
            preShowImage(windowName);
            showImage(windowName, index, img);
        }
    }

protected:
    std::map<std::string, int> frameCount;
private:
    virtual void preShowImage(std::string windowName) = 0;
    virtual void showImage(std::string windowName, int index, cv::Mat mat) = 0;
};

class DefaultImgDebugPrinter : public AbstractImgDebugPrinter {
    void preShowImage(std::string windowName) override;
    void showImage(std::string windowName, int index, cv::Mat mat) override;
};

class StubImgDebugPrinter : public AbstractImgDebugPrinter {
    void preShowImage(std::string windowName) override;
    void showImage(std::string windowName, int index, cv::Mat mat) override;
};

class IImgDebugPrinterFactory {
public:
    AbstractImgDebugPrinter* getPrinter() {
#ifdef UNIT_TESTS_RUNNING
        return new StubImgDebugPrinter();
#else
        return new DefaultImgDebugPrinter();
#endif
    }
};

#endif //PROJECT_IMGDEBUGPRINTER_H
