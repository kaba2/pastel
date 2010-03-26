Pastel's nearest neighbours example
===================================

Description
-----------

This is a program to visualize a bounding interval tree.
A point set is drawn along with its associated tree. 
You can move the mouse cursor over this point set and the 
program shows you its nearest points using the Euclidean 
norm. The smaller circle is the spray radius. 
The larger circle is the search radius beyond which 
points are not considered. When the search radius is infinite,
as in the start, it is not shown.

Adding and removing points
--------------------------

You can spray new points by pushing the left mouse button. 
Similarly, you can erase points by pushing the right mouse 
button.

When spraying new points, the tree can become unoptimal,
(especially if the new points extend the bounding box of
the tree). The optimality can be regained by recomputing the 
tree (see keys below).

Adaptation to removal
---------------------

When you erase points, the tree adapts itself by 
decreasing the level of detail in the subdivision.
These subtrees are not actually removed but just 
taken out of consideration to speed up searches. You can
see this by spraying points to the same area from which
you removed points from: the underlying subtrees are 
again taken in consideration.

Levels of subdivision
---------------------

At the start the program shows only the leaf nodes of the tree.
Using keypad + and - you can visualize nodes at higher levels on 
the tree.

Drawing performance
-------------------

Drawing is currently a performance bottleneck. 
When the number of points and tree nodes get higher, 
the frame rate will drop quickly. Therefore I have
included keys to toggle drawing on and off.
The program will always show the bounding box of the points
as well as the search radius circle.

Keys
----

F5 - toggle finite / infinite search radius
F6 - toggle searching k-nearest / all in range

left mouse button - spray some points
right mouse button - erase points
c - clear subdivision and all points
x - clear all points but not the subdivision
t - toggle drawing the tree
p - toggle drawing the point set
n - toggle drawing the nearest points
w, a, s, d - translate the camera
q, e - rotate the camera
r, f - zoom the camera
o - Refine the subdivision
esc - quit
0 - recompute the tree
