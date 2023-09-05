D4E - 4-dim manifold viewer
![Clifford Torus](./cliftor.png)

This program uses a simple method for viewing 4 dimensional objects. Objects are defined via a function which returns true or false - true if a given 4d point lies within the bounds of the object, and false otherwise. Random points are generated and filtered through the function to return a set of points defining the object, up to a set number.

Currently there are no shaders or other ways of rendering the surface other than plotting them point-wise and using hue to encode the 4th distance.

Viewing Controls:

mouse click+drag - rotate total view
mouse scroll - zoom total view in/out

"P":	toggles viewing of the primary rotation plane
"[":	toggles viewing of the secondary rotation plane

"9":	lessen the 4d shadow strength
"0":	increase the 4d shadow strength

"Z":	rotate object about primary rotation plane
"X":	" but opposite direction
"C": 	rotate object about secondary rotation plane
"V":	" but opposite direction

"1":	increase rotation speed
"2":	decrease rotation speed

WASD:	rotate the rotation planes

"O":	toggle orthogonal snap of the rotation planes
