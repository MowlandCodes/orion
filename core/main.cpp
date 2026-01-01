#include <algorithm>
#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/core/types.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/videoio.hpp>

using namespace cv;
using namespace std;

// Gain variable
float Kp_X = 0.5; // gain for rotate
float Kp_Y = 0.8; // gain for moving forward

struct RobotCmd {
    int16_t vx;
    int16_t vy;
    int16_t omega;
    uint8_t kick;
};

int main()
{
    // capture the video from camera
    VideoCapture cap(0, CAP_V4L2);

    if (!cap.isOpened())
        cerr << "the camera stream is cannot be opened!" ;

    // Define the matrix and type data for robot movement
    Mat hsv, frame, mask;
    RobotCmd cmd;

    // Scalar for orange color range
    Scalar lower_orange(2, 77, 105);
    Scalar upper_orange(27, 255, 255);

    int center_x = 320;
    int center_y = 240;

    // looping the stream
    while (true)
    {
        cap >> frame;
        if (frame.empty()) break;

        cvtColor(frame, hsv, COLOR_BGR2HSV);
        inRange(hsv, lower_orange, upper_orange, mask);

        // erode n dilate
        erode(mask, mask, Mat(), Point(-1, -1), 2);
        dilate(mask, mask, Mat(), Point(-1, -1), 2);
        
        Moments m = moments(mask);
        cmd = {0, 0, 0, 0};

        if (m.m00 > 100)
        {
            int ball_x = static_cast<int>(m.m10 / m.m00);
            int ball_y = static_cast<int>(m.m01 / m.m00);

            // control logic
            int error_x = ball_x - center_x;
            int error_y = ball_y - center_y;

            // hitung output motor
            float out_omega = error_x * Kp_X;
            float out_vy = error_y * Kp_Y;

            // clamping power pwm, patok di max = 150, bukan 255
            cmd.omega = static_cast<int16_t>(clamp(out_omega, -100.0f, 100.0f));
            cmd.vy = static_cast<int16_t>(clamp(out_vy, -150.0f, 150.0f));

            if (abs(error_x) < 30 && abs(error_y) < 50)
            {
                cmd.kick = 1;
                putText(frame, "KICK!", Point(50, 50), FONT_HERSHEY_COMPLEX, 1, Scalar(0, 0, 255), 3);
            }
            line(frame, Point(center_x, center_y), Point(ball_x, ball_y), Scalar(0,0, 255), 2);
            circle(frame, Point(ball_x, ball_y), 10, Scalar(0, 255, 0), -1);
        }

        cout << "Data : " << cmd.vy
            << "| Omega : " << cmd.omega
            << "| Kick : " << (int)cmd.kick;
        
        imshow("mask", mask);
        imshow("live", frame);

        if (waitKey(1) == 27) break;
    }
    return  0;
}
