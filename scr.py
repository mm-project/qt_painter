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

#dicmdCreateLeminsicata1(300,300)
#dicmdCreateLeminsicata2(400,400)
#dicmdCreateLissajous(500,500)
#dicmdCreateRose(1900,900)
#dicmdCreateSpiral(0,0)

#for idx, obj in enumerate(objects, start=1):
#    if idx % 3 == 0:  # every 3rd object
#        obj_id = obj["object_id"]
#        mmproject.dicmdDeleteObj2(int(obj_id))



 
import random
from collections import deque

def create_blockages(num_rects=5, grid_size=1000):
    rects = []
    for _ in range(num_rects):
        x1 = random.randint(0, grid_size - 200)
        y1 = random.randint(0, grid_size - 200)
        x2 = x1 + random.randint(100, 200)
        y2 = y1 + random.randint(100, 200)

        mmproject.dicmdCreateObjRectangle(x1, y1, x2, y2)
        rects.append((x1, y1, x2, y2))
    return rects


def is_blocked(x, y, rects, delta=5):
    for (x1, y1, x2, y2) in rects:
        if (x1 - delta) <= x <= (x2 + delta) and (y1 - delta) <= y <= (y2 + delta):
            return True
    return False


def manhattan_path(start, goal, rects, grid_size=1000, delta=5):
    q = deque([start])
    visited = {start: None}
    moves = [(1,0), (-1,0), (0,1), (0,-1)]  # Manhattan moves

    while q:
        cur = q.popleft()
        if cur == goal:
            break
        for dx, dy in moves:
            nx, ny = cur[0] + dx, cur[1] + dy
            if 0 <= nx < grid_size and 0 <= ny < grid_size:
                if not is_blocked(nx, ny, rects, delta) and (nx, ny) not in visited:
                    visited[(nx, ny)] = cur
                    q.append((nx, ny))

    if goal not in visited:
        print("No path found!")
        return []

    path = []
    cur = goal
    while cur:
        path.append(cur)
        cur = visited[cur]
    path.reverse()
    return path


if __name__ == "__main__":
    grid_size = 1000
    rects = create_blockages(num_rects=15, grid_size=grid_size)

    start = (300, 300)
    goal = (295,190) 
    #goal = (grid_size - 1, grid_size - 1)

    path = manhattan_path(start, goal, rects, grid_size, delta=20)

    for (x, y) in path:
        dicmdCreateObjPoint(x, y)
