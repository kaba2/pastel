Nearest neighbours example
==========================

Description
-----------

A point set is drawn along with its associated kd-tree.
You can move the mouse cursor over this point set and
the program shows you its nearest points (various L_p
metrics can be chosen).
The white circle is the (L_2) search radius beyond which points
are not considered.
You can spray new points by pushing the left mouse button.

Note
----

Drawing is currently a performance bottleneck. 
When the number of points and kd-tree nodes get higher, 
the frame rate will drop quickly. Therefore I have
included keys to toggle drawing on and off.
The program will always show the bounding box of the points
as well as the search radius circle.

When spraying new points, the kd-tree can become unoptimal
(especially if the new points extend the bounding box of
the tree). The optimality can be regained by computing the 
kd-tree from the start by using the number keys.

Keys
----

F1 - use manhattan (L_1) norm
F2 - use euclidean (L_2) norm
F3 - use L_3 norm
F4 - use L_4 norm
F5 - toggle finite / infinite search radius
F6 - toggle searching k-nearest / all in range

left mouse button - spray some points
t - toggle drawing of the kd-tree
p - toggle drawing of the point set
n - toggle drawing of the nearest points
w, a, s, d - translate the camera
q, e - rotate the camera
r, f - zoom the camera
esc - quit
1, 2, 3, 4, 5, 6, 7, 8, 9 - recompute kd-tree for a given maximum depth
0 - recompute the kd-tree for maximum depth of 24

Timings from my machine
-----------------------

Nearest neighbour searching (infinite radius):

Points  Neighbours  Timing
10000   15          0.2s
100000  15          2.4s
1000000 15          24.9s

10000   150         1.8s
100000  150         19s
1000000 150         218s

According to the papers, the average complexity 
(given some mild requirements on the data) of retrieving the
k nearest neighbours of a point in a kd-tree should be O(k log n).

Thus retrieving the nearest neighbours for all points is
O(k n log n). The timings support this well. This is quite fast:
merely listing the k nearest neighbors of n points takes O(k n).

Kd-tree construction:

Points  Timing
10000   0.03s
100000  0.3s
1000000 3.9s

According to the papers, the average complexity
(given some mild requirements on the data) of constructing a kd-tree
should be O(n log n). This complexity holds for the median splitting.
I am using a modified splitting criterion: the 'sliding midpoint',
which gives better nearest neighbour performance for certain kinds 
of data sets (by avoiding thin nodes). It should have similar construction
complexity. This is well backed up by the timings.
