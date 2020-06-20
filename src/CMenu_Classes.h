#ifndef CMENU_H_
#define CMENU_H_

#include "Arduino.h"

/***************************************************************************/
/*                                                                         */
/* Defines                                                                 */
/*                                                                         */
/***************************************************************************/
#define MAX_PAGES   5

/***************************************************************************/
/*                                                                         */
/*                                                                         */
/*                                                                         */
/* CMenuPageBase Class                                                     */
/*                                                                         */
/*                                                                         */
/*                                                                         */
/***************************************************************************/
class CMenuPageBase
{
private:
    int       m_PageIndex;
    String    m_PageName;

public:
              CMenuPageBase();
    virtual   ~CMenuPageBase() {}

    int       GetPageIndex() { return m_PageIndex; }
    void      SetPageIndex( int PageIndex ) { m_PageIndex = PageIndex; }

    // Overridable methods
    virtual String  GetName() { return m_PageName; }
    virtual void    SetName( String PageName ) { m_PageName = PageName; }

    // Pure methods
//    virtual void    IncrementValue() = 0;
//    virtual void    DecrementValue() = 0;
};

/***************************************************************************/
/*                                                                         */
/*                                                                         */
/*                                                                         */
/* CMenu Class                                                             */
/*                                                                         */
/*                                                                         */
/*                                                                         */
/***************************************************************************/
class CMenu
{
private:
    int             m_Level;
    int             m_CurrentPageIndex;
    int             m_TotalPages;
    CMenuPageBase*  m_pPages[ MAX_PAGES ];

public:
                    CMenu( int NumOfPages, String PageNames[] );
    virtual         ~CMenu();

    int             GetLevel() { return m_Level; }
    void            SetLevel( int Level ) { m_Level = Level; }
    int             GetCurrentPageIndex() { return m_CurrentPageIndex; }
    void            SetCurrentPageIndex( int CurrentPageIndex ) { m_CurrentPageIndex = CurrentPageIndex; }
    int             GetTotalPages() { return m_TotalPages; }
    void            SetTotalPages( int TotalPages) { m_TotalPages = TotalPages; }
    CMenuPageBase*  GetPage( int PageIndex = -1 ) { return PageIndex == -1 ? m_pPages[ m_CurrentPageIndex ] : m_pPages[ PageIndex ]; }

    void            IncrementCurrentPageIndex();
    void            DecrementCurrentPageIndex();
    void            IncrementValueFromPage( int PageIndex = -1 );
    void            DecrementValueFromPage( int PageIndex = -1 );
    void            Print();
};

#endif /* CMENU_H_ */
