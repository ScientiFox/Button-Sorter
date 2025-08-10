###
#
# Button sorter imaging
#  Experimental script which evaluates methods for identifying
#  whether button caps are top- or bottom-up
#
###


#Standards
import math,time,random

#Numerical and image processing
import numpy as np
from scipy import ndimage
import cv2

#File management
import glob

def blobs_size(s):
    #Utilite to grab the 5 largest blob regions in a labeled image s
    o_list = [] #List of sized
    for a in range(s[1]+1): #For each labeled blob
        o_list.append(np.sum(1.0*(s[0]==a))) #Get area sum and append to list
    o_list.sort() #Sort list
    return o_list[-5:] #Grab up to the five largest region

def face_test_2(img,img_avg_top,img_avg_bottom):
    #Second test for facing direction
    t,b = abs(np.average(1.0*(s[0] == 0) - img_avg_top)),abs(np.average(1.0*(s[0] == 0) - img_avg_bottom)) #comparisons to average samples
    return "top"*(t < b) + "bottom"*(t >= b),abs(t-b)

def face_test_4(i):
    #Fourth test for facing direction
    S = sum(blobs_size(i)[:-1]) #Sum of non-background blob areas
    thresh = 5500
    return ("top"*(S > thresh) + "bottom"*(S <= thresh),S)

def face_test_5(i):
    #Fifth test for facing direction
    S = np.sum(i[0][28:32,10:310]) #Well-known shadow region on buttons
    thresh = 1148
    return ("top"*(S < thresh) + "bottom"*(S >= thresh),S)

def face_test_6(i):
    #Sixth test for facing direction

    tran = 0 #Number of level transitions
    state = i[0][28,190] #Current value state

    #Crossing a 40px line in the imaging region
    for a in range(40):
        if np.sum(i[0][28:32,190+a]) == 4*state: #Check if 4-wide segment is in the current state
            state = 1 - state #Toggle state
            tran += 1 #count up transitions
    return ("top"*(tran < 3) + "bottom"*(tran >= 3),tran)

#Top and bottom average images from samples
img_avg_top = np.zeros((240,320))
img_avg_bottom = np.zeros((240,320))

#Average label vectors for top and bottom samples
avg_vt = np.array([[0,0,0,0]])
avg_vb = np.array([[0,0,0,0]])

#Looping over the 6 top side samples
for a in range(6):
    img = cv2.imread("Button Images/top down/top"+str(a+1)+".JPG") #Load image
    s = ndimage.label(img[:,:,2]>((1.0*np.max(img[:,:,2])+1.0*np.min(img[:,:,0]))*(0.25))) #Get the labeled blobs
    img_avg_top = img_avg_top + 0.3333*(s[0] == 0) #Add proportional 'background' region to average

    #build label vectors for top images
    avg_vt = avg_vt + 0.3333*np.array([[face_test_2(img,img_avg_top,img_avg_bottom)[1],face_test_4(s)[1],face_test_5(s)[1],face_test_6(s)[1]]])

#Looping over the 6 bottom side samples
for a in range(6):
    img = cv2.imread("Button Images/bottom down/bottom"+str(a+1)+".JPG") #Load image
    s = ndimage.label(img[:,:,2]>((1.0*np.max(img[:,:,2])+1.0*np.min(img[:,:,0]))*(0.25))) #Get the labeled blobs
    img_avg_bottom = img_avg_bottom + 0.3333*(s[0] == 0) #Add proportional 'background' region to average

    #build label vectors for bottom images
    avg_vb = avg_vb + 0.3333*np.array([[face_test_2(img,img_avg_top,img_avg_bottom)[1],face_test_4(s)[1],face_test_5(s)[1],face_test_6(s)[1]]])

#ground truth test labels
ground = ['top','top','top','bottom','bottom','bottom']

print("Label:    | Outputs:")

#Testing the 6 testing samples
for a in range(6):
    img = cv2.imread("Button Images/test/test"+str(a+1)+".JPG") #Load image
    s = ndimage.label(img[:,:,2]>((1.0*np.max(img[:,:,2])+1.0*np.min(img[:,:,0]))*(0.25))) #Get the labeled blobs

    #Make label vector for this sample
    values = np.array([[face_test_2(img,img_avg_top,img_avg_bottom)[1],face_test_4(s)[1],face_test_5(s)[1],face_test_6(s)[1]]])

    #make the group of all results
    group = [face_test_2(img,img_avg_top,img_avg_bottom)[0],face_test_4(s)[0],face_test_5(s)[0],face_test_6(s)[0]]

    #Make up final label from outputs
    label = [1.0*(group[a]=='top') for a in range(len(group))]
    label = sum(label)/7.0

    #Final output of test
    o_string = ""
    o_str = " "*(10-len(ground[a])) + ground[a] + "|"
    for b in group:
        o_str = o_str + " "*(8-len(b)) + b
    print(o_str)

    
