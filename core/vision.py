import cv2
import numpy as np
import math
import config

class VisionSystem:
    def __init__(self) -> None:
        self.lower_hsv = np.array([0, 100, 100])
        self.upper_hsv = np.array([20, 255, 255])

        self.cap = None

    def set_hsv_range(self, lower, upper):
        """Update nilai HSV dari tuner secara real time"""
        self.lower_hsv = np.array(lower)
        self.upper_hsv = np.array(upper)

    def get_ball_data(self, frame, camera_type="WEBCAM"):
        """
        Input: Frame gambar
        Output: (Founf (bool), x_center (int), Distance_M (float))
        """
        if frame is None:
            return False, 0, 0.0

        hsv = cv2.cvtColor(frame, cv2.COLOR_BGR2HSV)
        mask = cv2.inRange(hsv, self.lower_hsv, self.upper_hsv)

        mask = cv2.erode(mask, None, iterations=2)
        mask = cv2.dilate(mask, None, iterations=2)

        contours, _ = cv2.findContours(mask, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)

        if len(contours) > 0:
            c = max(contours, key=cv2.contourArea)
            x, y, w, h = cv2.boundingRect(c)
            center_x = x + w // 2

            distance = 0.0

            if camera_type == "WEBCAM":
                if w > 0:
                    distance = (0.20 * 800) / w
            elif camera_type == "FISHEYE":
                distance = 0.0

            return True, center_x, distance

        return False, 0, 0.0

    def draw_debug(self, frame, x, dist, found):
        """Helper buat nampilin kotak di layar"""
        if found:
            cv2.circle(frame, (x, 240), 10, (0, 255, 0), -1)
            cv2.putText(frame, f"Dist: {dist:.2f}m", (x, 200),
                        cv2.FONT_HERSHEY_SIMPLEX, 0.6, (0, 255, 0), 2)
            return frame
