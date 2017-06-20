#include "StdAfx.h"
#include "cCompositeComplex.h"

cCompositeComplex::cCompositeComplex(void)
{
}

cCompositeComplex::~cCompositeComplex(void)
{
	for(unsigned int i = 0;Data.size() > i; ++i)
	{
		SAFE_DELETE(Data[i]);
	}
}

void cCompositeComplex::Add( cComposite* newData )
{
	Data.push_back(newData);
}

cComposite* cCompositeComplex::Get( int nIndex )
{
	if ( static_cast<int>(Data.size()) <= nIndex ) return NULL;

	return Data[nIndex];
		
}
