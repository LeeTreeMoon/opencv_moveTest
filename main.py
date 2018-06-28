#!/usr/bin/python2
# -*- coding:utf-8 -*-

#import module
import cv2

#open camera
cap=cv2.VideoCapture(0)
ret,background=cap.read()#read the first image as background
if background is None:
    print("background is empty!\n")
    exit(0)
backGray=cv2.cvtColor(background, cv2.COLOR_BGR2GRAY) #transform to gray image
while True:
    ret,frame=cap.read()
    if frame is None:
        print("this frame is empty!\n")
        exit(0)
    else:
        imgGray=cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
        diff=cv2.absdiff(imgGray, backGray)
        cv2.accumulateWeighted(backGray, diff, 0.6, backGray)
        cv2.imshow("capture", diff)    #display gray image
    if cv2.waitKey(1)==27:
        break
#end operation       	
cap.release()
cv2.destoryWindow("capture")
