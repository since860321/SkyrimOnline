#pragma once

class cObject
{
	int m_nRefCount;

protected:
	stTransformInfo	m_TransformInfo; //<<형구 TM을 만들기 위한 정보
	D3DXMATRIXA16	m_WolrdTM;		 //<<형구 그리기 최종적용 메트릭스

public:
	cObject( void )
	: m_nRefCount(1) //< 자기가 생성되면 자기자신도 생성된 갯수에 포함되므로 1로 초기화
	{}
	virtual ~cObject(void);

	//메모리 관리 부분입니다 << 형구 
	void Retain( void ); //< 레퍼런스 카운터를 증가시키는 함수
	void Release( void ); //< 레퍼런스 카운터를 감소시키고 카운터가 0이 되면 메모리해제를 하는 함수 
	void AutoRelease( void ); //< 해당 객체를 오토릴리즈 풀에 넣는 함수

	//기본 정보를 얻어옵니다.
	stTransformInfo* GetTransformInfo(){return &m_TransformInfo;}
	D3DXMATRIXA16*	 GetWorldTM(){return &m_WolrdTM;}

	//<< 월드 매트릭트를 구합니다
	virtual void SetWorldTM( void ); 
};

