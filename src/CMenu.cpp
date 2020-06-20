#include "Arduino.h"
#include <LiquidCrystal_I2C.h>

#include "CMenu_Classes.h"

/***************************************************************************/
/*                                                                         */
/* Globals                                                                 */
/*                                                                         */
/***************************************************************************/
// LCD screen
extern LiquidCrystal_I2C Lcd;

/***************************************************************************/
/*                                                                         */
/*                                                                         */
/* CMenu::CMenu()                                                          */
/*                                                                         */
/*                                                                         */
/***************************************************************************/
CMenu::CMenu()
{
    m_CurrentPageIndex = m_Level = 0;
    for( int i = 0; i < MAX_PAGES; i++ )
    {
        m_pPages[ i ] = NULL;
    }
}

/***************************************************************************/
/*                                                                         */
/*                                                                         */
/* CMenu::~CMenu()                                                         */
/*                                                                         */
/*                                                                         */
/***************************************************************************/
CMenu::~CMenu()
{
    for( int i = 0; i < MAX_PAGES; i++ )
    {
        if( m_pPages[ i ] ) delete m_pPages[ i ];
    }
    delete this;
}

/***************************************************************************/
/*                                                                         */
/*                                                                         */
/* CMenu::AddPage()                                                        */
/*                                                                         */
/*                                                                         */
/***************************************************************************/
int CMenu::AddPage( CMenuPageBase* Page )
{
    int Error = -1;
    for( int i = 0; i < MAX_PAGES; i++ )
    {
        if( m_pPages[ i ] == NULL )
        {
            m_pPages[ i ] = Page;
            Error = 0;
            break;
        }
    }
    return Error;
}

/***************************************************************************/
/*                                                                         */
/*                                                                         */
/* CMenu::GetCount()                                                       */
/*                                                                         */
/*                                                                         */
/***************************************************************************/
int CMenu::GetCount()
{
    int Count = 0;
    for( int i = 0; i < MAX_PAGES; i++ )
    {
        if( m_pPages[ i ] != NULL ) Count++;
        else break;
    }
    return Count;
}

/***************************************************************************/
/*                                                                         */
/*                                                                         */
/* CMenu::IncrementPageIndex()                                             */
/*                                                                         */
/*                                                                         */
/***************************************************************************/
void CMenu::IncrementCurrentPageIndex()
{
    if( m_CurrentPageIndex < GetCount() - 1 ) m_CurrentPageIndex++;
}

/***************************************************************************/
/*                                                                         */
/*                                                                         */
/* CMenu::DecrementPageIndex()                                             */
/*                                                                         */
/*                                                                         */
/***************************************************************************/
void CMenu::DecrementCurrentPageIndex()
{
    if( m_CurrentPageIndex > 0 ) m_CurrentPageIndex--;
}

/***************************************************************************/
/*                                                                         */
/*                                                                         */
/* CMenu::Print()                                                          */
/*                                                                         */
/*                                                                         */
/***************************************************************************/
void CMenu::Print()
{
    Lcd.clear();
    Lcd.setCursor( 0, 0 );
    Lcd.print( "Level " );
    Lcd.print( this->GetLevel(), DEC );
    Lcd.print( ", Page " );
    Lcd.print( this->GetCurrentPageIndex(), DEC );
    Lcd.setCursor( 0, 1 );
    Lcd.print( this->GetPage()->GetName() );
}
