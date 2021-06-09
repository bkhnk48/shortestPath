import turtle
import reeds_shepp as rs
import utils
import draw
import math
import random as rd



def main():
    # points to be followed
    pts1 = [(6.000000, 30.000000), (10.000000, 36.000000), (12.000000, 36.000000), (14.000000, 24.000000), (14.000000, 12.000000), (12.501727,0.083045)] 
    pts = []
    for i in range(len(pts1)):
        pts.append((pts1[i][0]/ rs.PathElement.RATIO, pts1[i][1]/rs.PathElement.RATIO))

    # generate PATH so the vectors are pointing at each other
    PATH = []
    PATH.append((pts[0][0], pts[0][1], 90))
    for i in range(1, len(pts) - 1):
        dx = pts[i+1][0] - pts[i][0]
        dy = pts[i+1][1] - pts[i][1]
        theta = math.atan2(dy, dx)
        PATH.append((pts[i][0], pts[i][1], utils.rad2deg(theta)))
    PATH.append((pts[-1][0], pts[-1][1], 270))

    # or you can also manually set the angles:
    # PATH = [(-5,5,90),(-5,5,-90),(1,4,180), (5,4,0), (6,-3,90), (4,-4,-40),(-2,0,240), 
    #         (-6, -7, 160), (-7,-1,80)]

    # or just generate a random route:
    # PATH = []
    # for _ in range(10):
    #     PATH.append((rd.randint(-7,7), rd.randint(-7,7), rd.randint(0,359)))

    # init turtle
    tesla = turtle.Turtle()
    tesla.speed(0) # 0: fast; 1: slow, 8.4: cool
    tesla.shape('arrow')
    tesla.resizemode('user')
    tesla.shapesize(1, 1)

    screen = turtle.Screen()
    screen.screensize(1500, 4000)


    # draw vectors representing points in PATH
    for pt in PATH:
        draw.goto(tesla, pt)
        draw.vec(tesla)

    # draw all routes found
    tesla.speed(0)
    #edges = rs.get_all_polygons()

    f = open("trajectory.txt","w")
    
    s = "#Segments " + str(len(PATH) - 1) + "\n"
    f.write(s)#number of segments at all
    #allBannedTrajectories = []
    for i in range(len(PATH) - 1):
        paths = rs.get_all_paths(PATH[i], PATH[i+1])
        #forbiddenTrajectories = []
        s = "_SegmentID " + str(i) + " from (" + str(PATH[i][0]) + ", " + str(PATH[i][1]) + ") to (" + str(PATH[i+1][0]) + ", " + str(PATH[i+1][1]) + ")\n"
        f.write(s)
        s = "__#Possible_Paths " + str(len(paths)) + "\n"
        f.write(s)
        j = 0
        for path in paths:
            s = "___SegmentID_" + str(i) + "_PathID_" + str(j) + "\n"
            f.write(s)
            draw.set_random_pencolor(tesla)
            draw.goto(tesla, PATH[i])
            #first = PATH[i]
            s = "____ " + str(PATH[i][0]) + " " + str(PATH[i][1]) + " " + str(PATH[i][2]) + "\n"
            
            localTrajectory = draw.draw_path(tesla, path)
            j += 1

            for k in range(len(localTrajectory)):
                s = "_____ " + str(round(localTrajectory[k][0], 3)) + " "
                s = s + str(round(localTrajectory[k][1], 3)) + " "
                param = round(localTrajectory[k][2], 2)
                s = s + str(param) + " "
                typeOfTraj = localTrajectory[k][3]
                s = s + str(typeOfTraj) + " "
                steering = localTrajectory[k][4]
                s = s + str(steering) + "\n"
                f.write(s)
                #if rs.cutThroughCurver(first, last, param, steering, edges) == 1:
                    #forbiddenTrajectories.append(j)
                #else:
                    #first = last
                    #last = []
        
        #allBannedTrajectories.append(forbiddenTrajectories)'''

    f.close()
    # draw shortest route
    #tesla.pencolor(1, 0, 0)
    #tesla.pensize(3)
    #tesla.speed(10)
    #draw.goto(tesla, PATH[0])
    #path_length = 0
    #trajectory = []
    #f = open("trajectory.txt","w")
    #s = str(round(rs.PathElement.RATIO*pts[0][0], 3)) + " " + str(round(rs.PathElement.RATIO*pts[0][1], 3)) + " B\n"
    #f.write(s)

    #for i in range(len(PATH) - 1):
        #path = rs.get_optimal_path(PATH[i], PATH[i+1])
        #path_length += rs.path_length(path)
        #trajectory = draw.draw_path(tesla, path)
        #for j in range(len(trajectory)):
        ##    s = str(round(rs.PathElement.RATIO*trajectory[j][0], 3)) + " " + str(round(rs.PathElement.RATIO*trajectory[j][1], 3)) + \
        #        " " + str(round(trajectory[j][2], 2)) + " " + trajectory[j][3] + " " + trajectory[j][4] + "\n"
        #    f.write(s)


    #print("Shortest path length: {} px.".format(int(draw.scale(path_length))))
    #f.close()

    turtle.done()


if __name__ == '__main__':
        main()
