#pragma once
class cGameTime
{
private:
	cGameTime(void);			
	DWORD m_dwLastTime;			
	DWORD m_dwWorldTime;	    
	DWORD m_dwTempWorldTime;	
	DWORD m_dwFrame;			
	DWORD m_dwElapsedTime;		
	DWORD m_dwFps;				
	int	  m_nFramePerSec;

public:	
	static cGameTime& Instance();	
	void  Start();					
	void  Update();				    
	void  Render();					
	DWORD Get_ElapsedTime() {return m_dwElapsedTime;}
	DWORD Get_WordTime() {return m_dwWorldTime;}

	virtual ~cGameTime(void);
};

