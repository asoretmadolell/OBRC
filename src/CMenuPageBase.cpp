#include "Arduino.h"

#include "CMenu_Classes.h"

/***************************************************************************/
/*                                                                         */
/*                                                                         */
/* CMenuPageBase::CMenuPageBase()                                          */
/*                                                                         */
/*                                                                         */
/***************************************************************************/
CMenuPageBase::CMenuPageBase( String PageName )
{
    m_PageIndex = 0;
    PageName == "" ? m_PageName = "Unknown" : m_PageName = PageName;
}
