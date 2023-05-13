import mmproject
import random
import time

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

#mmproject.dicmdCreateObjRectangle(0,0,100,100)
#mmproject.dicmdCreateObjRectangle(200,200,400,400)

start_time = time.time()
for number in range(10000):
    create_rand_shapes()
print("Creation: --- %s seconds ---" % (time.time() - start_time))
mmproject.dicmdQaToolExit()

#start_time = time.time()
#mmproject.dicmdSelectShapesByRegion(0,0,1000000,1000000)
#print("Selection: --- %s seconds ---" % (time.time() - start_time))

#for i in range(35):
#    start_time = time.time()
#    mmproject.dicmdCanvasViewportZoomOut()
#    print(str(i)+" zoomout: --- %s seconds ---" % (time.time() - start_time))
#mmproject.dicmdQaToolExit()

#mmproject.dicmdQaToolExit()

