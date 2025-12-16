import cv2

def test_cam(index):
    cap = cv2.VideoCapture(index)
    if not cap.isOpened():
        print(f"camera {index} tidak terdeteksi")
        return
    ret, frame = cap.read()
    if ret:
        cv2.imshow(f"camera {index}", frame)
        print("Camera {index} OK - press Q for break")
        while True:
            if cv2.waitKey(1) & 0xFF == ord('q'): break
    cap.release()

test_cam(0)
test_cam(1)
cv2.destroyAllWindows()


