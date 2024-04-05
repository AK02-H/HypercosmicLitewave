#pragma once


struct RectStruct
{
	int left;
	int top;
	int right;
	int bottom;
};

class Rectangle
{
private:

	

public:

	int left;
	int right;
	int top;
	int bottom;

	//Bandaid to resolve rect clipping issues from rounding inaccuracies
	int permaWidth = 0;
	int permaHeight = 0;

	Rectangle() : left(0), right(0), top(0), bottom(0) {}
	Rectangle(int l, int r, int t, int b) : left(l), right(r), top(t), bottom(b)
	{
		permaWidth = r - l;
		permaHeight = b - t;
	}
	void Translate(int dx, int dy)
	{
		left += dx;
		right += dx;
		top += dy;
		bottom += dy;
	}

	void ClipTo(const Rectangle& other)
	{
		//use min and max?
		if (right > other.right) {
			right = other.right;
		}

		if (bottom > other.bottom) {
			bottom = other.bottom;
		}

		if (left < other.left) {
			left = other.left;
		}

		if (top < other.top) {
			top = other.top;
		}
	}

	bool IsOutsideOf(const Rectangle& other) {

		if (right < other.left || bottom < other.top || left > other.right || top > other.bottom) {
			return true;
		}
		
		return false;
	}

	bool IfPointWithin(int x, int y) const
	{
		if (x < left || x > right || y < top || y > bottom)
		{
			return false;
		}
		else
		{
			return true;
		}
	}

	//??
	bool IsCompletelyContained(){}

	int Width() const
	{ 
		return right - left;
	};

	int Height() const { return bottom - top; };

};

class Circle {

public:

	int x, y;
	int radius;

	Circle(int newRad = 0, int newX = 0, int newY = 0) {
		radius = newRad;
		x = newX;
		y = newY;
	}

};