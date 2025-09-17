import mmproject
import re
import math
import random

objects = []
def dicmdCreateObjPoint(x,y):
     o = mmproject.dicmdCreateObjRectangle(x,y,x+3,y+3)
     objects.append(o)

def dicmdCreateLeminsicata1(x0,y0):
    r = 100
    for t in range(500):
        #x = r*t - r*math.sin(t)
        #y = r - r*math.cos(t)
        d = 1+(math.sin(t)*math.cos(t))
        x = (r*math.sqrt(2)*math.cos(t))/d
        y = (r*math.sqrt(2)*math.cos(t)*math.sin(t))/d
        dicmdCreateObjPoint(x0 + int(x),y0 + int(y))

def dicmdCreateLeminsicata2(x0,y0):
    r = 100
    for t in range(500):
        d = 1+(math.sin(t)*math.sin(t))
        x = (r*math.sqrt(2)*math.cos(t))/d
        y = (r*math.sqrt(2)*math.cos(t)*math.sin(t))/d
        dicmdCreateObjPoint(x0 + int(x),y0 + int(y))

def dicmdCreateRose(x0, y0):
    r = 100
    k = 5  # number of petals
    for t in range(2000):
        theta = t * 0.01
        radius = r * math.cos(k * theta)
        x = radius * math.cos(theta)
        y = radius * math.sin(theta)
        dicmdCreateObjPoint(x0 + int(x), y0 + int(y))

def dicmdCreateLissajous(x0, y0):
    A, B = 100, 100
    a, b = 3, 4
    delta = math.pi / 2
    for t in range(2000):
        theta = t * 0.01
        x = A * math.sin(a * theta + delta)
        y = B * math.sin(b * theta)
        dicmdCreateObjPoint(x0 + int(x), y0 + int(y))

def dicmdCreateSpiral(x0, y0):
    for t in range(2000):
        theta = t * 0.1
        r = 0.5 * t
        x = r * math.cos(theta)
        y = r * math.sin(theta)
        dicmdCreateObjPoint(x0 + int(x), y0 + int(y))


dicmdCreateLeminsicata1(300,300)
dicmdCreateLeminsicata2(400,400)
dicmdCreateLissajous(500,500)
dicmdCreateRose(1900,900)
dicmdCreateSpiral(0,0)

for object in objects:
    print(object)
