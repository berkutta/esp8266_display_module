float xx,xy,xz;
float yx,yy,yz;
float zx,zy,zz;

float fact;

int Xan,Yan;

int Xoff;
int Yoff;
int Zoff;

typedef struct Point3d
{
  int x;
  int y;
  int z;
} Point3d_t;

typedef struct Point2d
{
  int x;
  int y;
} Point2d_t;



int LinestoRender; // lines to render.
int OldLinestoRender; // lines to render just in case it changes. this makes sure the old lines all get erased.

typedef struct Line3d
{
  Point3d_t p0;
  Point3d_t p1;
} Line3d_t;



typedef struct Line2d
{
  Point2d_t p0;
  Point2d_t p1;
} Line2d_t;


Line3d_t Lines[20];
Line2d_t Render[20];
Line2d_t ORender[20];