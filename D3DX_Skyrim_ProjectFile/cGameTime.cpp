#include "StdAfx.h"
#include "cGameTime.h"


cGameTime::cGameTime(void)
{
}


cGameTime::~cGameTime(void)
{
}

cGameTime& cGameTime::Instance()
{
	static cGameTime gameTime;
	return gameTime;
}

void cGameTime::Start(void) 
{
	m_dwLastTime		= GetTickCount();
	m_dwWorldTime		= 0;				
	m_dwFrame			= 0;
	m_dwElapsedTime		= 0;
	m_dwTempWorldTime	= 0;
	m_nFramePerSec		= 0;
}


void cGameTime::Update(void)
{
	DWORD dwcurrent = GetTickCount();			

	m_dwFrame++;								
	m_dwElapsedTime = dwcurrent - m_dwLastTime;	
	m_dwWorldTime	+= m_dwElapsedTime;			

	m_dwLastTime = dwcurrent;					

	if( m_dwTempWorldTime < m_dwWorldTime)
	{
		m_dwTempWorldTime = m_dwWorldTime + 1000;
		m_dwFps = m_nFramePerSec;
		m_nFramePerSec = 0;
	}
	else
	{
		m_nFramePerSec++;

	}
}


void cGameTime::Render()
{
 	//���� ���� ���� �ð� 1/ 1000;
 	char szBuf[128];
 	sprintf(szBuf, "WorldTime :  %d", m_dwWorldTime);
 	RECT rc = { 10 , 10 , 11 ,11};
	cDevice::GetInstance().GetFont()->DrawText(NULL, szBuf, strlen(szBuf), &rc, DT_LEFT | DT_NOCLIP | DT_TOP , 0xffffffff);
 
 	//������ �ð� 1/ 1000;
 	sprintf(szBuf, "�� ������ :  %d", m_dwFrame);
 	RECT rc1 = { 10 , 30 , 11 ,31};
 	cDevice::GetInstance().GetFont()->DrawText(NULL, szBuf, strlen(szBuf), &rc1, DT_LEFT | DT_NOCLIP | DT_TOP , 0xffffffff);
 
 	//�ʴ� ������ 
 	sprintf(szBuf, "�ʴ� ������ :  %d", m_dwFps);
 	RECT rc2 = { 10 , 50 , 11 ,51};
 	cDevice::GetInstance().GetFont()->DrawText(NULL, szBuf, strlen(szBuf), &rc2, DT_LEFT | DT_NOCLIP | DT_TOP , 0xffffffff);
 
 	//���ð��� �����ð��� ��
 	sprintf(szBuf, "���ð��� ����ð����� :  %d", m_dwElapsedTime);
 	RECT rc3 = { 10 , 70 , 11 ,71};
 	cDevice::GetInstance().GetFont()->DrawText(NULL, szBuf, strlen(szBuf), &rc3, DT_LEFT | DT_NOCLIP | DT_TOP , 0xffffffff);
 	
	/*
	//���� ���� ���� �ð� 1/ 1000;
	sprintf(szBuf, "WorldTime :  %d / 1000 ��", m_dwWorldTime);
	TextOut(Bhdc, 10, 10, szBuf, strlen(szBuf));

	//������ �ð� 1/ 1000;
	sprintf(szBuf, "�� ������ : %d", m_dwFrame);
	TextOut(Bhdc, 10, 30, szBuf, strlen(szBuf));

	//�� �ð��� ���� �ð���  
	sprintf(szBuf, "�ʴ� ������ :  %d", m_dwFps);
	TextOut(Bhdc, 10, 50, szBuf, strlen(szBuf));*/
}