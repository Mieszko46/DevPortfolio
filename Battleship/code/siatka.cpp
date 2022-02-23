//---------------------------------------------------------------------------

#pragma hdrstop

#include "siatka.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

TSiatka::TSiatka()
{
	srand( time( NULL ) );
	clear();
}

void TSiatka::clear()
{
	for (int x=0; x < COL_MAX; x++)
		for (int y=0; y < ROW_MAX; y++) {
			m_xPlansza[x][y] = STAN_PUSTE;
		}
}
//---------------------------------------------------------------------------

STAN TSiatka::getStan(int x, int y)
{
	if (x<0 || x >= COL_MAX)
		return STAN_NULL;
	if (y<0 || y >= ROW_MAX)
		return STAN_NULL;

	return m_xPlansza[x][y];
}
//---------------------------------------------------------------------------

void TSiatka::setStan(int x, int y, STAN eStan)
{
	if (x >= 0 && x < COL_MAX && y >= 0 && y < ROW_MAX)
		m_xPlansza[x][y] = eStan;
}
//---------------------------------------------------------------------------

bool TSiatka::czyStatekObok(int x, int y)
{
	bool bTak = false;

	for (int iRow = -1; iRow < 2 && !bTak; iRow++)
		for (int iCol = -1; iCol < 2; iCol++)
		{
			//wyznaczam wspó³rzêdne danej komórki obszaru
			int iX = x + iCol;
			int iY = y + iRow;

			//sprawdzam czy komorka jest w siatce
			if (iX < 0 || iX >= COL_MAX || iY < 0 || iY >= ROW_MAX)
				continue;

			//sprawdzam czy to jest s¹siednia komórka
			if (iX == x && iY==y)
				continue;

			if (m_xPlansza[iX][iY] == STAN_STATEK)
			{
				bTak = true;
				break;
			}
		}

	return bTak;
}
//---------------------------------------------------------------------------

int TSiatka::policz(STAN eStan)
{
	int iIlosc = 0;

	for (int x=0; x < COL_MAX; x++)
		for (int y=0; y < ROW_MAX; y++)
			if (m_xPlansza[x][y] == eStan)
				iIlosc++;

	return iIlosc;
}
//---------------------------------------------------------------------------

bool TSiatka::getKomorka(STAN eStan, int iIndex, int &iX, int &iY)
{
	bool bOk = false;
	int iPozycja = 0;

	for (int x=0; x < COL_MAX && !bOk; x++)
		for (int y=0; y < ROW_MAX && !bOk; y++)
		{
			if (m_xPlansza[x][y]==eStan && iPozycja==iIndex)
			{
				iX = x;
				iY = y;
				bOk = true;
				break;
			}

			if (m_xPlansza[x][y]==eStan)
				iPozycja++;
		}

	return bOk;
}
//---------------------------------------------------------------------------

int TSiatka::mozliweStrzaly(int iX, int iY, int r)
{
	int iIlosc = 0;

	for (int x = iX-r; x <= iX+r; x++)
		for (int y = iY-r; y <= iY+r; y++)
			if (getStan(x, y)==STAN_PUSTE && (x==iX || y==iY)) //bierzemy tylko punkty w pionie/poziome wzglêdem pkt wejœciowego
				iIlosc++;

	return iIlosc;
}
//---------------------------------------------------------------------------

bool TSiatka::getStrzal(int iIndex, int iX, int iY, int r, int &iStrzalX, int &iStrzalY)
{
	bool bStrzal = false;
	int iPozycja = 0;

	for (int x = iX-r; x <= iX+r && !bStrzal; x++)
		for (int y = iY-r; y <= iY+r && !bStrzal; y++)
		{
			if (getStan(x, y)==STAN_PUSTE && (x==iX || y==iY) && iPozycja==iIndex)
			{
				iStrzalX = x;
				iStrzalY = y;
				bStrzal = true;
				break;
			}

			if (getStan(x, y)==STAN_PUSTE && (x==iX || y==iY))
				iPozycja++;
		}

	return bStrzal;
}
//---------------------------------------------------------------------------

