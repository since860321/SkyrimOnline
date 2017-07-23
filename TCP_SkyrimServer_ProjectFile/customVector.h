//for linux

class vector
{
public:
	vector();
	vector(int _x, int _y, int _z);
	
	vector	operator-(vector& rhw);
	vector	operator*(float _f);
	vector	operator+=(vector& rhw);
	vector	operator-=(vector& rhw);
	static void	normalize(vector *pOut, const vector *pV);

public:
	float x;
	float y;
	float z;
};