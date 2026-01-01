#include <opencv2/core.hpp>
#include <opencv2/core/mat.hpp>
#include <opencv2/core/types.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>

using namespace cv;
using namespace std;

// capture video from camera and stream it.
int main(int, char**)
{
    Mat frame, mask, hsv;
    VideoCapture cap;

    int deviceId = 0;
    int apiID = cv::CAP_V4L2;

    cap.open(deviceId, apiID);

    if (!cap.isOpened()) {
        cerr << "Error! Unable to open the Camera!\n";
        return -1;
    }

    cout << "Start grabbing the video wit loop" << endl
        << "Press any key to terminate" << endl;

    for (;;)
    {
        cap.read(frame);

        if (frame.empty())
        {
            cerr << "ERROR! blank frame grabbed";
            break;
        }

        // convert from BGR to HSV
        cv::cvtColor(frame, hsv, cv::COLOR_BGR2HSV);

        cv::Scalar lower_orange(10, 100, 20);
        cv::Scalar upper_orange(25, 255, 255);

        cv::inRange(hsv, lower_orange, upper_orange, mask);
        cv::erode(mask, mask, cv::Mat(), cv::Point(-1, -1), 2);
        cv::dilate(mask, mask, cv::Mat(), cv::Point(-1, -1), 2);

        cv::Moments m = cv::moments(mask);
        if (m.m00 > 20) 
        {
            cv::Point center(m.m10 / m.m00, m.m01 / m.m00);

            cv::circle(frame, center, 10, cv::Scalar(0, 255, 0), -1);

            cout << "Ball Positoon :" << center.x << ", " << center.y << endl;
        }

        imshow("Mask", mask);
        imshow("Live", frame);
        if (waitKey(1) == 27)
            break;
    }
    return 0;
}
