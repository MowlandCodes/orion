#include <opencv2/core.hpp>
#include <opencv2/core/mat.hpp>
#include <opencv2/core/types.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>

using namespace cv;
using namespace std;

int low_h = 0, low_s = 0, low_v = 0;
int high_h = 180, high_s = 255, high_v = 255;

static void on_trackbar(int, void*) {}

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

    // create trackbar for color selection
    namedWindow("Tuning", WINDOW_AUTOSIZE);

    createTrackbar("Low H", "Tuning", &low_h, 180, on_trackbar);
    createTrackbar("High H", "Tuning", &high_h, 180, on_trackbar);
    createTrackbar("Low S", "Tuning", &low_s, 255, on_trackbar);
    createTrackbar("Low V", "Tuning", &low_v, 255, on_trackbar);
    createTrackbar("High S", "Tuning", &high_s, 255, on_trackbar);
    createTrackbar("High V", "Tuning", &high_v, 255, on_trackbar);

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

        cv::Scalar lower_orange(low_h, low_s, low_v);
        cv::Scalar upper_orange(high_h, high_s, high_v);

        cv::inRange(hsv, lower_orange, upper_orange, mask);
        cv::erode(mask, mask, cv::Mat(), cv::Point(-1, -1), 2);
        cv::dilate(mask, mask, cv::Mat(), cv::Point(-1, -1), 2);

        cv::Moments m = cv::moments(mask);
        if (m.m00 > 200) 
        {
            cv::Point center(m.m10 / m.m00, m.m01 / m.m00);

            cv::circle(frame, center, 10, cv::Scalar(0, 255, 0), -1);

            cout << "Ball Positoon :" << center.x << ", " << center.y << endl;
        }

        imshow("Mask", mask);
        imshow("Live", frame);
        if (waitKey(1) == 27) break;
    }
    cout << "Final Value : " << endl;
    cout << "Lower Value : " << low_h << "," << low_s << "," << low_v << endl;
    cout << "Upper Value: " << high_h << "," << high_s << "," << high_v << endl;
    return 0;
}
