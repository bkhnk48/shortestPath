"""
Implementation of the optimal path formulas given in the following paper:

OPTIMAL PATHS FOR A CAR THAT GOES BOTH FORWARDS AND BACKWARDS
J. A. REEDS AND L. A. SHEPP

notes: there are some typos in the formulas given in the paper;
some formulas have been adapted (cf http://msl.cs.uiuc.edu/~lavalle/cs326a/rs.c)

Each of the 12 functions (each representing 4 of the 48 possible words)
have 3 arguments x, y and phi, the goal position and angle (in degrees) of the
object given it starts at position (0, 0) and angle 0, and returns the
corresponding path (if it exists) as a list of PathElements (or an empty list).

(actually there are less than 48 possible words but this code is not optimized)
"""

from utils import *
import math
from enum import Enum
from shapely.geometry import LineString
from shapely.geometry import Point


class Steering(Enum):
    LEFT = 1
    RIGHT = 2
    STRAIGHT = 3

class Gear(Enum):
    FORWARD = 1
    BACKWARD = 2

class PathElement():
    def __init__(self, param, steering, gear):
        self.param = param
        self.steering = steering
        self.gear = gear

    def __repr__(self):
        if self.steering == Steering.LEFT: steering_str = "left"
        elif self.steering == Steering.RIGHT: steering_str = "right"
        else: steering_str = "straight"

        if self.gear == Gear.FORWARD: gear_str = "forward"
        else: gear_str = "backward"

        s = "{ Steering: " + steering_str + "\tGear: " + gear_str \
            + "\tdistance: " + str(round(self.param, 2)) + " }"

        return s

    def reverse_steering(self):
        if self.steering == Steering.LEFT:
            self.steering = Steering.RIGHT
        elif self.steering == Steering.RIGHT:
            self.steering = Steering.LEFT

    def reverse_gear(self):
        if self.gear == Gear.FORWARD:
            self.gear = Gear.BACKWARD
        else:
            self.gear = Gear.FORWARD

def path_length(path):
    """
    this one's obvious
    """
    return sum([e.param for e in path])

def get_optimal_path(start, end, polygons = []):
    """
    Return the shortest path from start to end among those that exist
    """
    maxValue = math.inf
    paths = get_all_paths(start, end, polygons)
    i_min = 0
    L_min = path_length(paths[0])
    for i in range(1, len(paths)-1):
        L = path_length(paths[i])
        if L <= L_min:
            L_min, i_min = L, i
    return paths[i_min]

def get_all_polygons():
    edges = []
    f = open('polygons.txt', 'r')
    line = f.readline()
    numPolygons = int(line[0])
    for i in range(numPolygons):#doc N polygon co trong file
        line = f.readline()
        numEdges = int(line)
        for j in range(numEdges):
            coords = f.readline().split()
            x1 = float(coords[0])
            y1 = float(coords[1])
            x2 = float(coords[2]) 
            y2 = float(coords[3]) 
            edges.append((x1, y1, x2, y2))
    f.close()
    return edges

def readPlots():
	pts = []
	f = open('trajectory.txt', 'r')
	#line = f.readline()
	#numPolygons = int(line[0])
	for line in f:#doc N polygon co trong file
		line = f.readline()
        numEdges = int(line)
        for j in range(numEdges):
            coords = f.readline().split()
            x1 = float(coords[0])
            y1 = float(coords[1])
            x2 = float(coords[2]) 
            y2 = float(coords[3]) 
            #edges.append((x1, y1, x2, y2))
	f.close()
	return pts

def cutThroughCurver(start, end, e, edges):
    if e.steering == Steering.STRAIGHT:
        return 0
    deltaX = -start[0] + end[0]
    deltaY = -start[1] + end[1]
    distance = math.sqrt(deltaX*deltaX + deltaY*deltaY)/2
    midX = (start[0] + end[0])/2
    midY = (start[1] + end[1])/2
    xNormal = 0
    yNormal = 0
    if e.steering == Steering.LEFT:
        xNormal = -deltaY/(2*distance)
        yNormal = -deltaX/(2*distance)
    if e.steering == Steering.RIGHT:
        xNormal = deltaY/(2*distance)
        yNormal = deltaX/(2*distance)

    alpha = e.param/2
    Radius = distance/math.sin(alpha)
    deltaR = distance/math.tan(alpha)
    xR = midX + deltaR*xNormal
    yR = midY + deltaR*yNormal
    p = Point(xR, yR)
    c = p.buffer(Radius).boundary
    
    for segment in edges: #range(len(edges) - 1):
        l = LineString([(segment[0], segment[1]), (segment[2], segment[3])])
        i = c.intersection(l)
        if len(i) >= 1:
            xIntersection = i.geoms[0].coords[0][0]
            yIntersection = i.geoms[0].coords[0][0]
            if (xIntersection > start[0] or xIntersection > end[0]) and (yIntersection > start[1] or yIntersection > end[1]):
                return 1
        if len(i) == 2:
            xIntersection = i.geoms[1].coords[0][0]
            yIntersection = i.geoms[1].coords[0][0]
            if (xIntersection > start[0] or xIntersection > end[0]) and (yIntersection > start[1] or yIntersection > end[1]):
                return 1
    
    return 0

def get_all_paths(start, end, edges = []):
    """
    Return a list of all the paths from start to end generated by the
    12 functions and their variants
    """
    path_fns = [path1, path2, path3, path4, path5, path6, \
                path7, path8, path9, path10, path11, path12]
    paths = []

    

    # get coordinates of end in the set of axis where start is (0,0,0)
    x, y, theta = change_of_basis(start, end)

    for get_path in path_fns:
        # get the four variants for each path type, cf article
        paths.append(get_path(x, y, theta))
        paths.append(timeflip(get_path(-x, y, -theta)))
        paths.append(reflect(get_path(x, -y, -theta)))
        paths.append(reflect(timeflip(get_path(-x, -y, theta))))

    # remove path elements that have parameter 0
    for i in range(len(paths)):
        paths[i] = list(filter(lambda e: e.param != 0, paths[i]))

    # remove empty paths
    paths = list(filter(None, paths))
    if len(edges) > 0:
        # remove curver cuts through polygon
        paths = list(filter(lambda e: cutThroughCurver(start, end, e, edges) == 0, paths))

    return paths


def timeflip(path):
    """
    timeflip transform described around the end of the article
    """
    new_path = path.copy()
    for e in new_path:
        e.reverse_gear()
    return new_path

def reflect(path):
    """
    reflect transform described around the end of the article
    """
    new_path = path.copy()
    for e in new_path:
        e.reverse_steering()
    return new_path




def path1(x, y, phi):
    """
    Formula 8.1: CSC (same turns)
    """
    r, theta = R(x, y)
    phi = deg2rad(phi)
    path = []

    u, t = R(x - math.sin(phi), y - 1 + math.cos(phi))
    v = M(phi - t)

    if t >= 0 and u >= 0 and v >= 0:
        path.append(PathElement(t, Steering.LEFT, Gear.FORWARD))
        path.append(PathElement(u, Steering.STRAIGHT, Gear.FORWARD))
        path.append(PathElement(v, Steering.LEFT, Gear.FORWARD))

    return path


def path2(x, y, phi):
    """
    Formula 8.2: CSC (opposite turns)
    """
    r, theta = R(x, y)
    phi = M(deg2rad(phi))
    path = []

    rho, t1 = R(x + math.sin(phi), y - 1 - math.cos(phi))

    if rho * rho >= 4:
        u = math.sqrt(rho * rho - 4)
        t = M(t1 + math.atan2(2, u))
        v = M(t - phi)

        if t >= 0 and u >= 0 and v >= 0:
            path.append(PathElement(t, Steering.LEFT, Gear.FORWARD))
            path.append(PathElement(u, Steering.STRAIGHT, Gear.FORWARD))
            path.append(PathElement(v, Steering.RIGHT, Gear.FORWARD))

    return path


def path3(x, y, phi):
    """
    Formula 8.3: C|C|C
    """
    r, theta = R(x, y)
    phi = deg2rad(phi)
    path = []

    xi = x - math.sin(phi)
    eta = y - 1 + math.cos(phi)
    rho, theta = R(xi, eta)

    if rho <= 4:
        A = math.acos(rho / 4)
        t = M(theta + math.pi/2 + A)
        u = M(math.pi - 2*A)
        v = M(phi - t - u)

        if t >= 0 and u >= 0 and v >= 0:
            path.append(PathElement(t, Steering.LEFT, Gear.FORWARD))
            path.append(PathElement(u, Steering.RIGHT, Gear.BACKWARD))
            path.append(PathElement(v, Steering.LEFT, Gear.FORWARD))

    return path


def path4(x, y, phi):
    """
    Formula 8.4 (1): C|CC
    """
    r, theta = R(x, y)
    phi = deg2rad(phi)
    path = []

    xi = x - math.sin(phi)
    eta = y - 1 + math.cos(phi)
    rho, theta = R(xi, eta)

    if rho <= 4:
        A = math.acos(rho / 4)
        t = M(theta + math.pi/2 + A)
        u = M(math.pi - 2*A)
        v = M(t + u - phi)

        if t >= 0 and u >= 0 and v >= 0:
            path.append(PathElement(t, Steering.LEFT, Gear.FORWARD))
            path.append(PathElement(u, Steering.RIGHT, Gear.BACKWARD))
            path.append(PathElement(v, Steering.LEFT, Gear.BACKWARD))

    return path


def path5(x, y, phi):
    """
    Formula 8.4 (2): CC|C
    """
    r, theta = R(x, y)
    phi = deg2rad(phi)
    path = []

    xi = x - math.sin(phi)
    eta = y - 1 + math.cos(phi)
    rho, theta = R(xi, eta)

    if rho <= 4:
        u = math.acos(1 - rho*rho/8)
        A = math.asin(2 * math.sin(u) / rho)
        t = M(theta + math.pi/2 - A)
        v = M(t - u - phi)

        if t >= 0 and u >= 0 and v >= 0:
            path.append(PathElement(t, Steering.LEFT, Gear.FORWARD))
            path.append(PathElement(u, Steering.RIGHT, Gear.FORWARD))
            path.append(PathElement(v, Steering.LEFT, Gear.BACKWARD))

    return path


def path6(x, y, phi):
    """
    Formula 8.7: CCu|CuC
    """
    r, theta = R(x, y)
    phi = deg2rad(phi)
    path = []

    xi = x + math.sin(phi)
    eta = y - 1 - math.cos(phi)
    rho, theta = R(xi, eta)

    if rho <= 4:
        if rho <= 2:
            A = math.acos((rho + 2) / 4)
            t = M(theta + math.pi/2 + A)
            u = M(A)
            v = M(phi - t + 2*u)
        else:
            A = math.acos((rho - 2) / 4)
            t = M(theta + math.pi/2 - A)
            u = M(math.pi - A)
            v = M(phi - t + 2*u)

        if t >= 0 and u >= 0 and v >= 0:
            path.append(PathElement(t, Steering.LEFT, Gear.FORWARD))
            path.append(PathElement(u, Steering.RIGHT, Gear.FORWARD))
            path.append(PathElement(u, Steering.LEFT, Gear.BACKWARD))
            path.append(PathElement(v, Steering.RIGHT, Gear.BACKWARD))

    return path


def path7(x, y, phi):
    """
    Formula 8.8: C|CuCu|C
    """
    r, theta = R(x, y)
    phi = deg2rad(phi)
    path = []

    xi = x + math.sin(phi)
    eta = y - 1 - math.cos(phi)
    rho, theta = R(xi, eta)
    u1 = (20 - rho*rho) / 16

    if rho <= 6 and 0 <= u1 and u1 <= 1:
        u = math.acos(u1)
        A = math.asin(2 * math.sin(u) / rho)
        t = M(theta + math.pi/2 + A)
        v = M(t - phi)

        if t >= 0 and u >= 0 and v >= 0:
            path.append(PathElement(t, Steering.LEFT, Gear.FORWARD))
            path.append(PathElement(u, Steering.RIGHT, Gear.BACKWARD))
            path.append(PathElement(u, Steering.LEFT, Gear.BACKWARD))
            path.append(PathElement(v, Steering.RIGHT, Gear.FORWARD))

    return path


def path8(x, y, phi):
    """
    Formula 8.9 (1): C|C[pi/2]SC
    """
    r, theta = R(x, y)
    phi = deg2rad(phi)
    path = []

    xi = x - math.sin(phi)
    eta = y - 1 + math.cos(phi)
    rho, theta = R(xi, eta)

    if rho >= 2:
        u = math.sqrt(rho*rho - 4) - 2
        A = math.atan2(2, u+2)
        t = M(theta + math.pi/2 + A)
        v = M(t - phi + math.pi/2)

        if t >= 0 and u >= 0 and v >= 0:
            path.append(PathElement(t, Steering.LEFT, Gear.FORWARD))
            path.append(PathElement(math.pi/2, Steering.RIGHT, Gear.BACKWARD))
            path.append(PathElement(u, Steering.STRAIGHT, Gear.BACKWARD))
            path.append(PathElement(v, Steering.LEFT, Gear.BACKWARD))

    return path


def path9(x, y, phi):
    """
    Formula 8.9 (2): CSC[pi/2]|C
    """
    r, theta = R(x, y)
    phi = deg2rad(phi)
    path = []

    xi = x - math.sin(phi)
    eta = y - 1 + math.cos(phi)
    rho, theta = R(xi, eta)

    if rho >= 2:
        u = math.sqrt(rho*rho - 4) - 2
        A = math.atan2(u+2, 2)
        t = M(theta + math.pi/2 - A)
        v = M(t - phi - math.pi/2)

        if t >= 0 and u >= 0 and v >= 0:
            path.append(PathElement(t, Steering.LEFT, Gear.FORWARD))
            path.append(PathElement(u, Steering.STRAIGHT, Gear.FORWARD))
            path.append(PathElement(math.pi/2, Steering.RIGHT, Gear.FORWARD))
            path.append(PathElement(v, Steering.LEFT, Gear.BACKWARD))

    return path


def path10(x, y, phi):
    """
    Formula 8.10 (1): C|C[pi/2]SC
    """
    r, theta = R(x, y)
    phi = deg2rad(phi)
    path = []

    xi = x + math.sin(phi)
    eta = y - 1 - math.cos(phi)
    rho, theta = R(xi, eta)

    if rho >= 2:
        t = M(theta + math.pi/2)
        u = rho - 2
        v = M(phi - t - math.pi/2)

        if t >= 0 and u >= 0 and v >= 0:
            path.append(PathElement(t, Steering.LEFT, Gear.FORWARD))
            path.append(PathElement(math.pi/2, Steering.RIGHT, Gear.BACKWARD))
            path.append(PathElement(u, Steering.STRAIGHT, Gear.BACKWARD))
            path.append(PathElement(v, Steering.RIGHT, Gear.BACKWARD))

    return path


def path11(x, y, phi):
    """
    Formula 8.10 (2): CSC[pi/2]|C
    """
    r, theta = R(x, y)
    phi = deg2rad(phi)
    path = []

    xi = x + math.sin(phi)
    eta = y - 1 - math.cos(phi)
    rho, theta = R(xi, eta)

    if rho >= 2:
        t = M(theta)
        u = rho - 2
        v = M(phi - t - math.pi/2)

        if t >= 0 and u >= 0 and v >= 0:
            path.append(PathElement(t, Steering.LEFT, Gear.FORWARD))
            path.append(PathElement(u, Steering.STRAIGHT, Gear.FORWARD))
            path.append(PathElement(math.pi/2, Steering.LEFT, Gear.FORWARD))
            path.append(PathElement(v, Steering.RIGHT, Gear.BACKWARD))

    return path


def path12(x, y, phi):
    """
    Formula 8.11: C|C[pi/2]SC[pi/2]|C
    """
    r, theta = R(x, y)
    phi = deg2rad(phi)
    path = []

    xi = x + math.sin(phi)
    eta = y - 1 - math.cos(phi)
    rho, theta = R(xi, eta)

    if rho >= 4:
        u = math.sqrt(rho*rho - 4) - 4
        A = math.atan2(2, u+4)
        t = M(theta + math.pi/2 + A)
        v = M(t - phi)

        if t >= 0 and u >= 0 and v >= 0:
            path.append(PathElement(t, Steering.LEFT, Gear.FORWARD))
            path.append(PathElement(math.pi/2, Steering.RIGHT, Gear.BACKWARD))
            path.append(PathElement(u, Steering.STRAIGHT, Gear.BACKWARD))
            path.append(PathElement(math.pi/2, Steering.LEFT, Gear.BACKWARD))
            path.append(PathElement(v, Steering.RIGHT, Gear.FORWARD))

    return path
