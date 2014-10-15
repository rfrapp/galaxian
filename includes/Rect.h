/*****************************************************************************
 Yihsiang Liow
 Copyright
 *****************************************************************************/
#ifndef RECT_H
#define RECT_H

#include <iostream>

class Rect
{
public:
	Rect(int x0 = 0, int y0 = 0, int w0 = 0, int h0 = 0)
		: x(x0), y(y0), w(w0), h(h0)
	{}
    Rect( const SDL_Rect & r)
		: x(r.x), y(r.y), w(r.w), h(r.h)
	{}

	friend std::ostream & operator<<(std::ostream &cout, Rect r)
	{
		cout << "<x: " << r.x << ", y: " << r.y << ", right: " << r.right()
			 << ", bottom: " << r.bottom() << ", w: " << r.w << ", h: " << r.h << '>';
		return cout; 
	}

	SDL_Rect getSDL_Rect()
	{
		SDL_Rect r = {x, y, w, h};
		return r;
	}

	bool collide_rect(Rect r) const 
	{
		return ((x >= r.x && x <= r.right()) || (right() >= r.x && right() <= r.right()))
            && ((y > r.y && y < r.bottom()) || (bottom() > r.y && bottom() < r.bottom()));
	}

	bool collide_point(int x0, int y0) const
	{
		return ((x0 >= x && x0 <= right()) && (y0 >= y && y0 <= bottom()));
	}

	int right()  const { return x + w; }
	int bottom() const { return y + h; }

	int x, y, w, h;
};

// inline std::ostream & operator<<(std::ostream & cout, const Rect & r)
// {
// 	cout << "(" << r.x << "," << r.y << "," << r.w << "," << r.h << ")";
// 	return cout;
// }
#endif
