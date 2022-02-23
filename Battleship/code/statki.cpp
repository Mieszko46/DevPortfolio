//---------------------------------------------------------------------------

#pragma hdrstop

#include "statki.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

Modul::Modul()
{
	x = -1;
	y = -1;
	trafienie = false;
}
//---------------------------------------------------------------------------

Modul::Modul(int x, int y)
{
	this->x = x;
	this->y = y;
	trafienie = false;
}
//---------------------------------------------------------------------------

void TStatki::przypiszWartosciStartowe(int temp)
{
	if(temp == 0)
		m_iRozmiar = 4;
	if(temp == 1 || temp == 2)
		m_iRozmiar = 3;
	if(temp == 3 || temp == 4 || temp == 5)
		m_iRozmiar = 2;
	else
		m_iRozmiar = 1;

	for (int i=0; i < m_iRozmiar; i++)
	{
		m_vModul.push_back(Modul());
	}
}
//---------------------------------------------------------------------------

void TStatki::przypiszWspolrzedne(int X, int Y, bool b)
{
	for (int i=0; i < m_iRozmiar; i++)
	{
		Modul *p = &m_vModul[i];
		if (i == 0)
		{
			p->x = X;
			p->y = Y;
		}
	}

	//TODO
    //petle w ktorej dalsze wartosci bedziemy dawac do wektora
}
//---------------------------------------------------------------------------

int TStatki::getRozmiar()
{
	return m_vModul.size();
}
//---------------------------------------------------------------------------

Modul TStatki::getModul(int i)
{
	if (i>=0 && i<m_vModul.size())
		return m_vModul[i];

	throw Exception("[TStatki::getModul]B³êdny indeks!");
}
//---------------------------------------------------------------------------

void TStatki::clear()
{
	m_vModul.clear();
}
//---------------------------------------------------------------------------

bool TStatki::czyNowyModulOK(int x, int y)
{
	if (m_vModul.size()==0)
		return true;

	bool bPoprawny = true;

	std::vector<Modul>::iterator it;
	//czy taki modul ju¿ nie istnieje
	for (it = m_vModul.begin(); it != m_vModul.end() && bPoprawny; it++)
	{
		if (it->x == x && it->y == y)
		{
			bPoprawny = false;
			break;
		}
	}

	//czy nowy modul jest pionowo lub poziomo
	enum KIERUNEK { BRAK, PIONOWO, POZIOMO } eKierunek = BRAK;
	for (it = m_vModul.begin(); it != m_vModul.end() && bPoprawny; it++)
	{
		if (it->x == x) //idziemy pionowo
		{
			if (eKierunek==BRAK)
				eKierunek = PIONOWO;
			else if (eKierunek==POZIOMO)
			{
				bPoprawny = false;
				break;
			}
		}
		else
		if (it->y == y) //idziemy poziomo
		{
			if (eKierunek==BRAK)
				eKierunek = POZIOMO;
			else if (eKierunek==PIONOWO)
			{
				bPoprawny = false;
				break;
			}
		}
		else
		{
			bPoprawny = false;
			break;
		}
	}

	//czy nowy modul sasiaduje w pionie/poziomie przynajmniej z jednym modulem
	//tylko cztery pozycie w stosunku do danego modulu
	bool bSasiaduje = false;
	for (it = m_vModul.begin(); it != m_vModul.end() && bPoprawny; it++)
	{
		bSasiaduje = (it->x==x && it->y==y-1)
				  || (it->x==x+1 && it->y==y)
				  || (it->x==x && it->y==y+1)
				  || (it->x==x-1 && it->y==y);

		if (bSasiaduje)
			break;
	}

	return bPoprawny && eKierunek!=BRAK && bSasiaduje;
}
//---------------------------------------------------------------------------

bool TStatki::utworzModul(int x, int y)
{
	Modul xModul(x, y);
	m_vModul.push_back(xModul);

return true;
}
//---------------------------------------------------------------------------

bool TStatki::pobierzOstatni(Modul &xModul)
{
	if (m_vModul.size()==0)
		return false;

	xModul = *(m_vModul.end()-1);
	m_vModul.pop_back();

return true;
}
//---------------------------------------------------------------------------

