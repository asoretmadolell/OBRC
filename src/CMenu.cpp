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
CMenu::CMenu( int NumOfPages, String PageNames[] )
{
    m_CurrentPageIndex = m_Level = 0;
    m_TotalPages = NumOfPages;

    for( int i = 0; i < m_TotalPages && i < MAX_PAGES; i++ )
    {
        m_pPages[ i ] = new CMenuPageBase;
        m_pPages[ i ]->SetPageIndex( i );
        m_pPages[ i ]->SetName( PageNames[ i ] );
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
    if( m_CurrentPageIndex < m_TotalPages - 1 ) m_CurrentPageIndex++;
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
