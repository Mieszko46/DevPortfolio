//---------------------------------------------------------------------------

#pragma hdrstop

#include "statki.h"
#include "siatka.h"
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

int TStatki::getRozmiar()
{
	return m_vModul.size();
}
//---------------------------------------------------------------------------

Modul TStatki::getModul(int i)
{
	if (i>=0 && i<m_vModul.size())
		return m_vModul[i];

	throw Exception("[TStatki::getModul]B��dny indeks!");
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
	//czy taki modul ju� nie istnieje
	for (it = m_vModul.begin(); it != m_vModul.end() && bPoprawny; it++)
	{
		if (it->x == x && it->y == y)
		{
			bPoprawny = false;
			break;
		}
	}

	//czy nowy modul jest pionowo lub poziomo
	KIERUNEK eKierunek = BRAK;
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

TStatki::KIERUNEK TStatki::getKierunek()
{
	KIERUNEK eKierunek = BRAK;

	if (m_vModul.size()==0)
		return eKierunek;

	std::vector<Modul>::iterator it = m_vModul.begin();
	int x = it->x;
	int y = it->y;

	for (it = m_vModul.begin()+1; it != m_vModul.end(); it++)
	{
		if (it->x == x) //idziemy pionowo
		{
			eKierunek = PIONOWO;
			break;
		}
		else
		if (it->y == y) //idziemy poziomo
		{
			eKierunek = POZIOMO;
			break;
		}
	}

return eKierunek;
}
//---------------------------------------------------------------------------

int TStatki::getLastX()
{
	if (m_vModul.size()==0)
		return -1;

return m_vModul[m_vModul.size()-1].x;
}
//---------------------------------------------------------------------------

int TStatki::getLastY()
{
	if (m_vModul.size()==0)
		return -1;

return m_vModul[m_vModul.size()-1].y;
}
//---------------------------------------------------------------------------

bool TStatki::czyTrafienie(int x, int y)
{
	for (std::vector<Modul>::iterator it = m_vModul.begin(); it != m_vModul.end(); it++)
	{
		if (it->x == x && it->y == y)
		{
			it->trafienie = true;
			return true;
		}
	}

return false;
}
//---------------------------------------------------------------------------

bool TStatki::czyZatopiony()
{
	for (std::vector<Modul>::iterator it = m_vModul.begin(); it != m_vModul.end(); it++)
	{
		if (!it->trafienie)
			return false;
	}

return true;
}
//---------------------------------------------------------------------------

bool TStatki::getGraniceStatku(std::vector<Modul> &vGraniceStatku)
{
	vGraniceStatku.clear();

	Modul xGranicaPocz;
	Modul xGranicaKoniec;

	KIERUNEK eKierunek = getKierunek();
	for (std::vector<Modul>::iterator it = m_vModul.begin(); it != m_vModul.end(); it++)
	{
		if (eKierunek == POZIOMO)
		{
			//granica pocz�tku statku poziomego
			if (xGranicaPocz.x == -1)
				{
				xGranicaPocz.x = it->x;
				xGranicaPocz.y = it->y;
				}
			else
			if (xGranicaPocz.x > it->x)
				xGranicaPocz.x = it->x;

			//granica ko�ca statku poziomego
			if (xGranicaKoniec.x == -1)
				{
				xGranicaKoniec.x = it->x;
				xGranicaKoniec.y = it->y;
				}
			else
			if (xGranicaKoniec.x < it->x)
				xGranicaKoniec.x = it->x;
		}
		else
		if (eKierunek == PIONOWO)
		{
			//granica pocz�tku statku pionowego
			if (xGranicaPocz.y == -1)
				{
				xGranicaPocz.x = it->x;
				xGranicaPocz.y = it->y;
				}
			else
			if (xGranicaPocz.y > it->y)
				xGranicaPocz.y = it->y;

			//granica ko�ca statku pionowego
			if (xGranicaKoniec.y == -1)
				{
				xGranicaKoniec.x = it->x;
				xGranicaKoniec.y = it->y;
				}
			else
			if (xGranicaKoniec.y < it->y)
				xGranicaKoniec.y = it->y;
		}
		else
			return false;
	}

	//wyznaczenie obszar�w tu� przed i za statkiem
	if (eKierunek == POZIOMO)
	{
		if (xGranicaPocz.x != -1)
			{
			xGranicaPocz.x--;
			if (xGranicaPocz.x >= 0)
				vGraniceStatku.push_back(xGranicaPocz);
			}

		if (xGranicaKoniec.x != -1)
			{
			xGranicaKoniec.x++;
			if (xGranicaKoniec.x < COL_MAX)
				vGraniceStatku.push_back(xGranicaKoniec);
			}
	}
	else
	if (eKierunek == PIONOWO)
	{
		if (xGranicaPocz.y != -1)
			{
			xGranicaPocz.y--;
			if (xGranicaPocz.y >= 0)
				vGraniceStatku.push_back(xGranicaPocz);
			}

		if (xGranicaKoniec.y != -1)
			{
			xGranicaKoniec.y++;
			if (xGranicaKoniec.y < ROW_MAX)
				vGraniceStatku.push_back(xGranicaKoniec);
			}
	}

return vGraniceStatku.size()>0;
}
//---------------------------------------------------------------------------

