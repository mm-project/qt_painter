import random

def get_rand_point(start_x=None, start_y=None, delta=None):
    MIN_X, MAX_X = -1800, 1800
    MIN_Y, MAX_Y = -800, 800
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
    if obj != "dicmdCreateObjPolygon":
        # Just 2 points (start + end)
        p1, x1, y1 = get_rand_point()
        p2, _, _ = get_rand_point(x1, y1)
        points.extend([p1, p2])
    else:
        MIN_VERTICES, MAX_VERTICES = 3, 10
        how_many = random.randint(MIN_VERTICES, MAX_VERTICES)

        # Start first vertex anywhere
        p, x, y = get_rand_point()
        points.append(p)

        # Subsequent vertices within delta
        for _ in range(how_many - 1):
            p, x, y = get_rand_point(x, y, delta=delta)
            points.append(p)

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
    #objs = ["dicmdCreateObjRectangle", "dicmdCreateObjLine", "dicmdCreateObjEllipse", "dicmdCreateObjPolygon"]
    objs = ["dicmdCreateObjLine"]
    obj = random.choice(objs)
    brush = random.randint(1, 9)
    fill = random.randint(1, 9)
    #color = "{:06X}".format(random.randint(0, 0xFFFFFF))
    #color = get_random_color()
    color = "000000"
    brush = "1" 
    fill = "9" 
    points = get_rand_points(obj, delta=50)  # polygon points max 50px apart
    cmd_line = f"{obj} -brush {brush} -color #{color} -fill {fill} -points {points}"
    print(cmd_line)

def create_design():
    for _ in range(1000):
        create_rand_obj()

if __name__ == "__main__":
    create_design()
