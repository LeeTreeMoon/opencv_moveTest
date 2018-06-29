#!/usr/bin/python2
# -*- coding:utf-8 -*-

#import module
import cv2

BACKSIZE=24

# name    : getAverageImage
# function: get average image
# input   : the list of images
# output  : an average image
def getAverageImage(imgList):
    averageImage=[[0 for x in range(imgList[0].height)]for y in range(imgList[0].width)]
    if isinstance(imgList, list) is False:
        print("the parameter input is not a list!\n")
        exit(0)
    for x in range(imgList[0].height):
        for y in range(imgList[0].width):
            for n in range(BACKSIZE):
                  pixel=imgList[n][x,y]+pixel
            pixel=pixel/BACKSIZE
            averageImage[x,y]=pixel
    return averageImage
    

#define background list
backGroundList[]

#make background
for i in range(BACKSIZE):
    cap=cv2.VideoCapture(0)
    ret,backFrame=cap.read()
    backGroundList.append(backFrame)  #read a frame of background and store it


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
