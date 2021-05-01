r = 0.5;

// A adapter en fonction du raffinement de maillage souhaité 
h = 0.2;

Point(1) = {-2.0, -1.0, 0.0, h};
Point(2) = {-2.0, 1.0, 0.0, h};
Point(3) = {2.0, -1.0, 0.0, h};
Point(4) = {2.0, 1.0, 0.0, h};

Point(5) = {-r, 0.0, 0.0, h};
Point(6) = {r, 0.0, 0.0, h};
Point(7) = {0.0, r, 0.0, h};
Point(8) = {0.0, -r, 0.0, h};


Point(9) = {0.0, 0.0, 0.0, h};


Circle(1) = {5, 9, 7};
Circle(2) = {7, 9, 6};
Circle(3) = {6, 9, 8};
Circle(4) = {8, 9, 5};

Line(5) = {2, 1};
Line(6) = {1, 3};
Line(7) = {3, 4};
Line(8) = {4, 2};

Line Loop(9) = {5, 6, 7, 8};
Line Loop(10) = {1, 2, 3, 4};


Plane Surface(1) = {9, 10};

Physical Surface(1) = {1};

Physical Line(1) = {5};
Physical Line(2) = {8, 7, 6};
Physical Line(3) = {1, 2, 3, 4};
