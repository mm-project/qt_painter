import mmproject
import random
import time

def create_good_shapes(row,column):
    delta = 200
    epsilon = 70
    mmproject.dicmdCreateObjRectangle(delta*row,delta*column,delta*row-epsilon,delta*column-epsilon)

#k=1
#for i in range(4):
#    #print("column {}, row {}".format(column,row))
#    create_good_shapes(i,i)
#    k = k + 1
#    print(k)


start_time = time.time()
#k=1
for column in range(100):
    for row in range(100):
        #print("column {}, row {}".format(column,row))
        create_good_shapes(column,row)
        #k = k + 1
        #print(k)
print("Creation: --- %s seconds ---" % (time.time() - start_time))
#mmproject.dicmdQaToolExit()


#for i in range(10):
#    start_time = time.time()
#    mmproject.dicmdCanvasViewportZoomOut()
#    print(str(i)+" zoomout: --- %s seconds ---" % (time.time() - start_time))


#start_time = time.time()
#mmproject.dicmdSelectShapesByRegion(81,116,246,241)
#print("Selection: --- %s seconds ---" % (time.time() - start_time))

#for i in range(35):
#    start_time = time.time()
#    mmproject.dicmdCanvasViewportZoomOut()
#    print(str(i)+" zoomout: --- %s seconds ---" % (time.time() - start_time))
#mmproject.dicmdQaToolExit()
#mmproject.dicmdQaToolExit()

