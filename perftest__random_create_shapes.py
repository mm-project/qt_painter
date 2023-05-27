import mmproject
import re
import math
import random
import time

def dicmdCreateObjPoint(x,y):
     mmproject.dicmdCreateObjRectangle(x,y,x+3,y+3)

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

def create_rand_shapes():
    x1 = random.randint(0,10000)
    y1 = random.randint(0,10000)
    x2 = random.randint(0,10000)
    y2 = random.randint(0,10000)
    z = random.randint(0,1)
    #mmproject.dicmdCreateObjLine(x1,y1,x2,y2)
    if z==1:
        mmproject.dicmdCreateObjLine(x1,y1,x2,y2)
    else:
        mmproject.dicmdCreateObjRectangle(x1,y1,x2,y2)


#dicmdCreateLeminsicata1(300,300)
#dicmdCreateLeminsicata2(400,400)
#mmproject.dicmdSelectShapesByRegion(0,0,10000,10000)
#mmproject.dicmdQaCompareSelection()
#mmproject.dicmdQaToolExit()

#mmproject.dicmdCreateObjLine(0,0,100,100)
#mmproject.dicmdCreateObjLine(400,400,800,800)
#mmproject.dicmdCreateObjRectangle(0,0,100,100)

start_time = time.time()
for number in range(1000000):
    create_rand_shapes()
print("Creation: --- %s seconds ---" % (time.time() - start_time))

#mmproject.dicmdQaToolExit()


#start_time = time.time()
#mmproject.dicmdSelectShapesByRegion(0,0,1000000,1000000)
#print("Selection: --- %s seconds ---" % (time.time() - start_time))


#for i in range(35):
#    start_time = time.time()
#    mmproject.dicmdCanvasViewportZoomOut()
#    print(str(i)+" zoomout: --- %s seconds ---" % (time.time() - start_time))


#mmproject.dicmdQaToolExit()

#mmproject.dicmdCreateObjLine(0,0,100,100)
#mmproject.dicmdCreateObjRect(50,50,100,100)

