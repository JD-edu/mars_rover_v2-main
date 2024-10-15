import cv2 as cv
import numpy as np
from urllib.request import urlopen
from cobit_opencv_lane_detect import CobitOpencvLaneDetect
import os
import datetime
import time
import sys
import socket

# init OpenCV lane detect 
cv_detector = CobitOpencvLaneDetect()



#change to your ESP32-CAM ip
url="http://192.168.254.29:81/stream"
CAMERA_BUFFRER_SIZE=4096
stream=urlopen(url)
bts=b''

HOST = '192.168.254.29'
PORT = 83

client_socket = socket.socket(socket.AF_INET,socket.SOCK_STREAM)
client_socket.connect((HOST, PORT))
while True:
    bts+=stream.read(CAMERA_BUFFRER_SIZE)
    jpghead=bts.find(b'\xff\xd8')
    jpgend=bts.find(b'\xff\xd9')
    if jpghead>-1 and jpgend>-1:
        jpg=bts[jpghead:jpgend+2]
        bts=bts[jpgend+2:]
        buf = np.frombuffer(jpg,dtype=np.uint8)
        if buf.any():
            img=cv.imdecode(buf,cv.IMREAD_UNCHANGED)
            #v=cv.flip(img,0)
            #h=cv.flip(img,1)
            #p=cv.flip(img,-1)        
            #frame=p
            #h,w=frame.shape[:2]
            img_=cv.resize(img,(320,240))
            cv.imshow("a",img_)
            message = 'test'
            client_socket.send(message.encode())
            lanes, img_lane = cv_detector.get_lane(img_)
            angle, img_angle = cv_detector.get_steering_angle(img_lane, lanes)
            try:
                cv.imshow("a",img_angle)
            except:
                print("error")
            if img_angle is None:
                print("angle out")
            else:
                print(angle)
            k=cv.waitKey(1)
            if k & 0xFF==ord('q'):
                break

cv.destroyAllWindows() 