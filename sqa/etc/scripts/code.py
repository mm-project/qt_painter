import mmproject
import re
import math
import random

def get_rand_point(start_x=None, start_y=None, delta=None):
    MIN_X, MAX_X = -180000, 180000
    MIN_Y, MAX_Y = -80000, 80000
    MIN_WIDTH, MAX_WIDTH = 10, 100
    MIN_HEIGHT, MAX_HEIGHT = 10, 100

    if start_x is None or start_y is None:
        # First random point anywhere
        start_x = random.randint(MIN_X, MAX_X)
        start_y = random.randint(MIN_Y, MAX_Y)
        return f"({start_x},{start_y})", start_x, start_y
    else:
        if delta is None:  
            # Normal case (non-polygon shapes)
            dx = random.choice([-1, 1]) * random.randint(MIN_WIDTH, MAX_WIDTH)
            dy = random.choice([-1, 1]) * random.randint(MIN_HEIGHT, MAX_HEIGHT)
        else:
            # Polygon case: keep new point within delta box
            dx = random.randint(-delta, delta)
            dy = random.randint(-delta, delta)

        end_x = max(MIN_X, min(MAX_X, start_x + dx))
        end_y = max(MIN_Y, min(MAX_Y, start_y + dy))
        return f"({end_x},{end_y})", end_x, end_y


def get_rand_points(obj, delta=50):
    points = []
    # Just 2 points (start + end)
    p1, x1, y1 = get_rand_point()
    p2, _, _ = get_rand_point(x1, y1)
    points.extend([p1, p2])

    return "{" + ";".join(points) + "}"

def get_random_color():
    while True:
        r = random.randint(0, 255)
        g = random.randint(0, 255)
        b = random.randint(0, 255)

        # Reject if too close to black (all channels very low)
        if r < 40 and g < 40 and b < 40:
            continue

        return "{:02X}{:02X}{:02X}".format(r, g, b)

def create_rand_obj():
    objs = ["dicmdCreateObjRectangle"]
    obj = random.choice(objs)
    points = get_rand_points(obj, delta=50)  # polygon points max 50px apart

    s = points.strip("{}")
    pairs = s.split(";")
    x1, y1 = map(int, pairs[0].strip("()").split(","))
    x2, y2 = map(int, pairs[1].strip("()").split(","))

    mmproject.dicmdCreateObjRectangle(x1,y1,x2,y2)

def create_design():
    for _ in range(50000000):
        create_rand_obj()

if __name__ == "__main__":
    create_design()


