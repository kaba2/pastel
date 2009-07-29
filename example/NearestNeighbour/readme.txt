Pastel's nearest neighbours example
===================================

Description
-----------

This is a program to visualize a point kd-tree.

A point set is drawn along with its associated kd-tree.
You can move the mouse cursor over this point set and
the program shows you its nearest points using the 
Euclidean norm. The larger circle is the search radius 
beyond which points are not considered. The smaller
circle is the spray radius. 

Adding and removing points
--------------------------

You can spray new points by pushing the left mouse button. 
Similarly, you can erase points by pushing the right mouse 
button.

When spraying new points, the kd-tree becomes unoptimal
(especially if the new points extend the bounding box of
the tree). The optimality can be regained by computing the 
kd-tree from the start by using the number keys.

Adaptation to removal
---------------------

When you erase points, the kd-tree adapts itself by
pruning empty subtrees. These subtrees are not actually
removed but just taken out of consideration. You can
see this by spraying points to the same area from which
you removed points from: the underlying subtrees are 
again taken in consideration.

Levels of subdivision
---------------------

The number keys can be used to visualize how the subdivision
is done at each step, by setting an upper bound
for the number of subdivision levels. For a given point
set, try pressing the keys from 1 to 9 and 0 
subsequently.

Drawing performance
-------------------

Drawing is currently a performance bottleneck. 
When the number of points and kd-tree nodes get higher, 
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
x - clear all points
t - toggle drawing of the kd-tree
p - toggle drawing of the point set
n - toggle drawing of the nearest points
w, a, s, d - translate the camera
q, e - rotate the camera
r, f - zoom the camera
esc - quit
1, 2, 3, 4, 5, 6, 7, 8, 9 - recompute kd-tree for a given maximum depth
0 - recompute the kd-tree for maximum depth of 24
