/*****************************************************************************/
/*                                                                           */
/*                                                                           */
/*                                                                           */
/* On-Board Racing COntroller v0.3                                           */
/*                                                                           */
/*                                                                           */
/*                                                                           */
/*****************************************************************************/

#include "Arduino.h"
#include <LiquidCrystal_I2C.h>
#include <ClickEncoder.h>
#include <TimerOne.h>

#include "src/CMenu_Classes.h"

/*****************************************************************************/
/*                                                                           */
/* Defines                                                                   */
/*                                                                           */
/*****************************************************************************/
// Rotary encoder
#define RESW_PIN    2
#define REDT_PIN    3
#define RECLK_PIN   4

/*****************************************************************************/
/*                                                                           */
/* Globals                                                                   */
/*                                                                           */
/*****************************************************************************/
// Buttons
bool buttonPressed = false;
bool knobTurnedCW = false;
bool knobTurnedCCW = false;
bool pedalPressed = false;

// Rotary encoder
ClickEncoder* clickEncoder = NULL;
int16_t clickEncoderValue = 0;
int16_t clickEncoderLast = -1;

// Menu
CMenu* pMainMenu = NULL;

// LCD screen
LiquidCrystal_I2C Lcd( 0x27, 16, 2 );

/*****************************************************************************/
/*                                                                           */
/*                                                                           */
/* setup()                                                                   */
/*                                                                           */
/*                                                                           */
/*****************************************************************************/
void setup()
{
    // Rotary encoder
    clickEncoder = new ClickEncoder( RECLK_PIN, REDT_PIN, RESW_PIN, 4 );
    pinMode( RECLK_PIN, INPUT );
    pinMode( REDT_PIN, INPUT );
    pinMode( RESW_PIN, INPUT );//INPUT_PULLUP );
    Timer1.initialize( 1000 );
    Timer1.attachInterrupt( timerIsr );
    clickEncoder->setAccelerationEnabled( false );

    // Menu
    pMainMenu = new CMenu;
    pMainMenu->AddPage( new CMenuPageBase( "Page 1" ) );
    pMainMenu->AddPage( new CMenuPageBase( "Page 2" ) );
    pMainMenu->AddPage( new CMenuPageBase( "Page 3" ) );

    Serial.begin( 9600 );
    Wire.begin();
    Lcd.begin( 16, 2 );

    Lcd.backlight();
    Lcd.setCursor( 0, 0 );
    Lcd.print( "OBRC" );
    Lcd.setCursor( 0, 1 );
    Lcd.print( "v0.3 Dani pesao" );
}

/*****************************************************************************/
/*                                                                           */
/*                                                                           */
/* loop()                                                                    */
/*                                                                           */
/*                                                                           */
/*****************************************************************************/
void loop()
{
//    debugMenu();
//    debugClickEncoder();
    menu();
}

/*****************************************************************************/
/*                                                                           */
/*                                                                           */
/* menu()                                                                    */
/*                                                                           */
/*                                                                           */
/*****************************************************************************/
void menu()
{
    // Menu logic !

    if( buttonPressed )
    {
        switch( pMainMenu->GetLevel() )
        {
            case 0:
                pMainMenu->SetLevel( 1 );
                pMainMenu->Print();
                break;
            case 1:
                pMainMenu->SetLevel( 2 );
                pMainMenu->Print();
                break;
            default:
                pMainMenu->SetLevel( 0 );
                Lcd.clear();
                break;
        }
        buttonPressed = false;
    }

    if( knobTurnedCW )
    {
        switch( pMainMenu->GetLevel() )
        {
            case 1:
                pMainMenu->IncrementCurrentPageIndex();
                pMainMenu->Print();
                break;
            case 2:
                pMainMenu->Print();
                break;
            default:
                break;
        }
        knobTurnedCW = false;
    }

    if( knobTurnedCCW )
    {
        switch( pMainMenu->GetLevel() )
        {
            case 1:
                pMainMenu->DecrementCurrentPageIndex();
                pMainMenu->Print();
                break;
            case 2:
                pMainMenu->Print();
                break;
            default:
                break;
        }
        knobTurnedCCW = false;
    }

    // Rotary encoder logic !

    clickEncoderValue += clickEncoder->getValue();

    if( clickEncoderValue > clickEncoderLast )
    {
        clickEncoderLast = clickEncoderValue;
        knobTurnedCW = true;
    }
    else if( clickEncoderValue < clickEncoderLast )
    {
        clickEncoderLast = clickEncoderValue;
        knobTurnedCCW = true;
    }
    else
    {
        knobTurnedCW = false;
        knobTurnedCCW = false;
    }

    ClickEncoder::Button button = clickEncoder->getButton();

    if( button != ClickEncoder::Open )
    {
        switch( button )
        {
            case ClickEncoder::Pressed:
                break;
            case ClickEncoder::Held:
                break;
            case ClickEncoder::Released:
                break;
            case ClickEncoder::Clicked:
                buttonPressed = true;
                break;
            case ClickEncoder::DoubleClicked:
                break;
            default:
                break;
        }
    }
}

/*****************************************************************************/
/*                                                                           */
/*                                                                           */
/* timerIsr()                                                                */
/*                                                                           */
/*                                                                           */
/*****************************************************************************/
void timerIsr()
{
    clickEncoder->service();
}

/*****************************************************************************/
/*                                                                           */
/*                                                                           */
/* debugClickEncoder()                                                       */
/*                                                                           */
/*                                                                           */
/*****************************************************************************/
void debugClickEncoder()
{
    clickEncoderValue += clickEncoder->getValue();

    if( clickEncoderValue != clickEncoderLast )
    {
        clickEncoderLast = clickEncoderValue;
        Serial.print( "Encoder value: " );
        Serial.println( clickEncoderValue );
    }

    ClickEncoder::Button button = clickEncoder->getButton();

    if( button != ClickEncoder::Open )
    {
        Serial.print( "Button: " );
#define VERBOSECASE(label) case label: Serial.println(#label); break;
        switch( button )
        {
            VERBOSECASE( ClickEncoder::Pressed )
                ;
            VERBOSECASE( ClickEncoder::Held )
            VERBOSECASE( ClickEncoder::Released )
            VERBOSECASE( ClickEncoder::Clicked )
            case ClickEncoder::DoubleClicked:
                Serial.println( "ClickEncoder::DoubleClicked" );
                clickEncoder->setAccelerationEnabled( !clickEncoder->getAccelerationEnabled() );
                Serial.print( "  Acceleration is " );
                Serial.println( ( clickEncoder->getAccelerationEnabled() ) ? "enabled" : "disabled" );
                break;
        }
    }
}
