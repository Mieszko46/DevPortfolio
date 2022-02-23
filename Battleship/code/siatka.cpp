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

 //	int x = rand() % COL_MAX;
 //	int y = rand() % ROW_MAX;

 //	m_xPlansza[x][y] = STAN_STATEK;
}
//---------------------------------------------------------------------------

STAN TSiatka::getStan(int x, int y)
{
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

