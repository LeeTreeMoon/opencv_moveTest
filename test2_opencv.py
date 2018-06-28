#!/usr/bin/python2
# -*- coding:utf-8 -*-
import cv2
import numpy as np
from matplotlib import pyplot as plt
#open camera
cap = cv2.VideoCapture(0)
while True:
    #get a frame
    ret,frame = cap.read()
    if frame is None:
        print("this frame is empty!\n")
        exit(0)
    else:
        cv2.imshow("capture", frame)
    if cv2.waitKey(1) & 0xff == ord('q'):
    	break
cap.release()
cv2.destoryWindow("capture")
