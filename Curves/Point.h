#ifndef POINT_H
#define POINT_H

#include <OpenGL/gl.h>

#define POINTSIZE 5

class Point
{
public:
	float x, y;

	Point ( void ) : x ( 0 ), y ( 0 ) { }
	Point ( float x1, float y1 ) : x ( x1 ), y ( y1 ) { }

	Point operator + ( const Point& v ) const
	{
		return Point ( x + v.x, y + v.y );
	}

	Point operator - ( const Point& v ) const
	{
		return Point ( x - v.x, y - v.y );
	}

	Point operator * ( float s ) const
	{
		return Point ( x * s , y * s );
	}

	Point operator / ( float s ) const
	{
		return Point ( x / s , y / s );
	}

	void operator += ( const Point& v )
	{
		x += v.x;
		y += v.y;
	}

	void operator -= ( const Point& v )
	{
		x -= v.x;
		y -= v.y;
	}

	void operator *= ( float s )
	{
		x *= s;
		y *= s;
	}

	void operator /= ( float s )
	{
		x /= s;
		y /= s;
	}

	float dot ( const Point &v ) const
	{
		return x * v.x + y * v.y;
	}

	void zero ( void )
	{
		x = y = 0;
	}

	void normalize ( void )
	{
		float len = sqrt ( x * x + y * y );
		x /= len;
		y /= len;
	}

	void draw ( void )
	{
		glColor3f ( 0, 0, 1 );
		glBegin ( GL_QUADS );
			glVertex2f ( x - POINTSIZE / 2.0, y - POINTSIZE / 2.0 ); 
			glVertex2f ( x + POINTSIZE / 2.0, y - POINTSIZE / 2.0 ); 
			glVertex2f ( x + POINTSIZE / 2.0, y + POINTSIZE / 2.0 ); 
			glVertex2f ( x - POINTSIZE / 2.0, y + POINTSIZE / 2.0 ); 
		glEnd ( );
	}
};

#endif