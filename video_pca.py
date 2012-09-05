#!/usr/bin/env python

import cv2 as cv
import sys
import os
import numpy as np

files = os.listdir(sys.argv[1])
stride = int(sys.argv[2])
out_dir = sys.argv[3]

f = 0
matrix_test = None
imgraw = None
print " "
for image in files:
    f = f + 1
    if f % stride != 0:
        continue
    imgraw = cv.imread(os.path.join(sys.argv[1], image), 0)
    imgvector = imgraw.reshape(imgraw.size)
    #print imgvector
    try:
        matrix_test = np.vstack((matrix_test, imgvector))
    except:
        matrix_test = imgvector
    sys.stderr.write( str(f) + " of " + str(len(files)) + "\r" )
print ""

# PCA
print "Running PCA ..."
mean, eigenvectors = cv.PCACompute(matrix_test, np.mean(matrix_test, axis=0).reshape(1,-1))

print "mean: " + str(mean)
print " "

i = 0
for eigenvector in eigenvectors:
    np.save(os.path.join(out_dir, str(i) + ".npy"), eigenvector)
#    print eigenvector
    evimg = eigenvector.reshape(imgraw.shape)
    print "min: " + str(evimg.min()) + ", max: " + str(evimg.max())
#    evimg += evimg.min()
    evimg *= 255.0/evimg.max()
    cv.imwrite(os.path.join(out_dir, str(i) + ".png"), evimg)
    i = i + 1
