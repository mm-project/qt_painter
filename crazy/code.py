import time
import random

# Conway’s Game of Life
class GameOfLife:
    def __init__(self, rows=20, cols=20, cell_size=10):
        self.rows = rows
        self.cols = cols
        self.cell_size = cell_size
        # Initialize with random 0/1
        #self.grid = [[random.randint(0, 1) for _ in range(cols)] for _ in range(rows)]
        #self.grid = [[1 if random.random() < 0.2 else 0 for _ in range(cols)] for _ in range(rows)]
        self.grid = [[0 for _ in range(cols)] for _ in range(rows)]
        for _ in range(int(rows * cols * 0.2)):  # seed 20% of cells
            r = random.randrange(rows)
            c = random.randrange(cols)
            # seed a 3x3 cluster around (r,c)
            for dr in (-1, 0, 1):
                for dc in (-1, 0, 1):
                    if 0 <= r+dr < rows and 0 <= c+dc < cols:
                        self.grid[r+dr][c+dc] = 1

    def step(self):
        #mmproject.dicmdCanvasReset()
        new_grid = [[0 for _ in range(self.cols)] for _ in range(self.rows)]
        for r in range(self.rows):
            for c in range(self.cols):
                live_neighbors = self.count_neighbors(r, c)
                if self.grid[r][c] == 1:
                    if live_neighbors in (2, 3):
                        new_grid[r][c] = 1
                else:
                    if live_neighbors == 3:
                        new_grid[r][c] = 1
        self.grid = new_grid

    def count_neighbors(self, r, c):
        count = 0
        for dr in (-1, 0, 1):
            for dc in (-1, 0, 1):
                if dr == 0 and dc == 0:
                    continue
                nr, nc = r + dr, c + dc
                if 0 <= nr < self.rows and 0 <= nc < self.cols:
                    count += self.grid[nr][nc]
        return count

    def draw(self):
        #time.sleep(0.2)  # 200 ms per frame
        mmproject.dicmdCanvasReset()
        for r in range(self.rows):
            for c in range(self.cols):
                if self.grid[r][c] == 1:
                    x1 = c * self.cell_size
                    y1 = r * self.cell_size
                    x2 = x1 + self.cell_size
                    y2 = y1 + self.cell_size
                    #mmproject.dicmdCanvasReset()
                    #time.sleep(0.1)
                    mmproject.dicmdCreateObjRectangle(x1, y1, x2, y2)
                    #time.sleep(0.2)  # 200 ms per frame



if __name__ == "__main__":
    game = GameOfLife(rows=50, cols=50, cell_size=50)
    time.sleep(5)
    
    game.draw()
    #time.sleep(0.7)
    #mmproject.dicmdCreateObjRectangle(0, 0, 100, 100)
    #mmproject.dicmdCanvasReset()

    #time.sleep(0.7)
    #game.step()
    
    while True:
        #mmproject.dicmdCanvasReset()
        #time.sleep(0.1)
        time.sleep(0.001)
        game.draw()
        time.sleep(0.001)
        game.step()
        #time.sleep(0.1)
        #time.sleep(0.2)  # 200 ms per frame
