struct Coord {
  union {
    int v[2];
    struct {
      int x, y;
    };
  };

  Coord();
  Coord(int x_, int y_);

  Coord neighbor(int dir);

  int abs();
};

Coord operator+(Coord a, Coord b);
Coord operator-(Coord a, Coord b);
Coord operator*(Coord a, double d);
Coord operator/(Coord a, double d);
bool operator==(Coord a, Coord b);
bool operator!=(Coord a, Coord b);
