# -*- coding: utf-8 -*-
"""
Created on Fri Apr 17 21:36:48 2026

@author: benz0
"""

import turtle

# ---------------- Hilbert Generator ----------------
def generate_hilbert(points, x, y, xi, xj, yi, yj, order):
    if order == 0:
        px = x + (xi + yi) / 2
        py = y + (xj + yj) / 2
        points.append((px, py))
    else:
        generate_hilbert(points, x, y, yi/2, yj/2, xi/2, xj/2, order - 1)
        generate_hilbert(points, x + xi/2, y + xj/2, xi/2, xj/2, yi/2, yj/2, order - 1)
        generate_hilbert(points, x + xi/2 + yi/2, y + xj/2 + yj/2, xi/2, xj/2, yi/2, yj/2, order - 1)
        generate_hilbert(points, x + xi/2 + yi, y + xj/2 + yj,
                         -yi/2, -yj/2, -xi/2, -xj/2, order - 1)

# ---------------- Generate Points ----------------
points = []
order = 5

generate_hilbert(points,
                 -1.0, -1.0,
                  2.0,  0.0,
                  0.0,  2.0,
                 order)

# ---------------- Draw with Turtle ----------------
t = turtle.Turtle()
t.speed(0)
turtle.bgcolor("black")
t.color("cyan")
t.width(2)

scale = 200

t.penup()
first_x, first_y = points[0]
t.goto(first_x * scale, first_y * scale)
t.pendown()

for x, y in points:
    t.goto(x * scale, y * scale)

turtle.done()