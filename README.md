# vertex-cover

## Roads to Vertex
### Input
road-to-vertex.py takes input from standard input. The input comprises lines each of which speciﬁes a command. There are 4 kinds of commands. 

(1) add a street, (2) change a street, (3) remove a street, and, (4) generate a graph. 

There could be any whitespace before the commands. Each command is separated from its arguments with at least one whitespace. There could be any whitespace between the numbers that represent the coordinates.
#### Commands

* a is used to add a street. It is specified as: “a "Street Name" (x1, y1) (x2, y2) ... (xn ,yn)”.
  Each (xi, yi) is a GPS coordinate. We interpret the coordinates as a poly-line segment. That is, we draw a line segment from (xi, yi) to (xi+1, yi+1). Assume that     each xi and yi is an integer. (Note, however, that the coordinates of an intersection may not be integers.)

* c is used to change the speciﬁcation of a street. Its format is the same as for a. It is a new speciﬁcation for a street you’ve speciﬁed before.

* r is used to remove a street. It is speciﬁed as “r "Street Name"”.

* g causes the program to output the corresponding graph.

### Output Graph
There is a vertex corresponding to: (a) each intersection, and, (b) the end-point of a line segment of a street that intersects with another street. An example of (a) from above is Vertex 3. An example of (b) is Vertex 1. The identity of a vertex can be any string of letters or integers (but no special characters). For example, v1xyz is acceptable as the identity of a vertex, but not v1 !!#xyz. (The space is unacceptable, as are ’!’ and ’#’.

There is an edge between two vertices if: (a) at least one of them is an intersection, (b) both lie on the same street, and, (c) one is reachable from the other without traversing another vertex. An example from above is the edge 〈 1, 3 〉 , which connects the end-point of a line segment to an intersection. Another example is the edge 〈 3, 6 〉 , which connects two intersections.

## Shortest path
### Input
* V \<maximum number of vertex\>
  - eg. V 15
* E {<vertice_index,vertice_index>, ... , <vertice_index,vertice_index>}
  - eg. E {<2,6>,<2,8>,<2,5>,<6,5>,<5,8>,<6,10>,<10,8>}
* s \<start\> \<end\>  #asks for a shortest path from the ﬁrst vertex to the second that is speciﬁed after the s.
  - eg. s 2 10

### Output
The shortest path from the start to end.
