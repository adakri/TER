// A adapter en fonction du raffinement de maillage souhaité 
h = 0.04;

Point(1) = {0.0, 0.0, 0.0, h};
Point(2) = {0.0, 1.0, 0.0, h};
Point(3) = {1.0, 0.0, 0.0, h};
Point(4) = {1.0, 1.0, 0.0, h};

Line(1) = {2, 1};
Line(2) = {1, 3};
Line(3) = {3, 4};
Line(4) = {4, 2};

Line Loop(5) = {1, 2, 3, 4};

Plane Surface(1) = {5};

Physical Surface(1) = {1};

Physical Line(1) = {1};
Physical Line(2) = {2};
Physical Line(3) = {3};
Physical Line(4) = {4};
