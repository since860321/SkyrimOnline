#pragma once 

#include "cComposite.h"
#include <vector>


class cCompositeComplex : public cComposite
{
protected:
	std::vector<cComposite*> Data;

public:
	cCompositeComplex(void);
	virtual ~cCompositeComplex(void);

	virtual void Process( stEnemyInfo* target )  = 0;
	void Add( cComposite* newData );
	cComposite* Get( int nIndex );

};
