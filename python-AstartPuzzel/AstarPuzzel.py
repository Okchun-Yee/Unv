import heapq
import copy

__all__ = ['astar']

goal = [['1', '2', '3'], ['4', '5', '6'], ['7', '8', '0']]
oper = ['up', 'down', 'left', 'right']

class Node:
    def __init__(self, data, parent=None, move=None, g=0):
        self.data = data
        self.parent = parent
        self.move = move
        self.g = g
        self.h = self.heuristic()
        self.f = self.g + self.h

    def heuristic(self):
        dist = 0
        for i in range(3):
            for j in range(3):
                val = self.data[i][j]
                if val == '0':
                    continue
                for x in range(3):
                    for y in range(3):
                        if goal[x][y] == val:
                            dist += abs(i - x) + abs(j - y)
        return dist

    def __lt__(self, other):
        return self.f < other.f

def get_zero(puzzle):
    for i in range(3):
        for j in range(3):
            if puzzle[i][j] == '0':
                return i, j

def move(puzzle, x, y, direction):
    dxdy = {'up': (-1, 0), 'down': (1, 0), 'left': (0, -1), 'right': (0, 1)}
    dx, dy = dxdy[direction]
    nx, ny = x + dx, y + dy
    if 0 <= nx < 3 and 0 <= ny < 3:
        new_puzzle = copy.deepcopy(puzzle)
        new_puzzle[x][y], new_puzzle[nx][ny] = new_puzzle[nx][ny], new_puzzle[x][y]
        return new_puzzle
    return None

def puzzle_to_tuple(p):
    return tuple(tuple(row) for row in p)

def astar(start):
    start_node = Node(data=start)
    open_list = []
    heapq.heappush(open_list, start_node)
    visited = set()

    while open_list:
        current = heapq.heappop(open_list)
        if current.data == goal:
            path = []
            while current:
                path.append(current.data)
                current = current.parent
            return path[::-1]

        visited.add(puzzle_to_tuple(current.data))
        x, y = get_zero(current.data)

        for op in oper:
            new_state = move(current.data, x, y, op)
            if new_state and puzzle_to_tuple(new_state) not in visited:
                child = Node(data=new_state, parent=current, move=op, g=current.g + 1)
                heapq.heappush(open_list, child)

    return []