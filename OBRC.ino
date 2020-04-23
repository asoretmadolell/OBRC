/*****************************************************************************/
/*                                                                           */
/*                                                                           */
/*                                                                           */
/* On-Board Racing COntroller v0.1                                           */
/*                                                                           */
/*                                                                           */
/*                                                                           */
/*****************************************************************************/

#include "Arduino.h"
#include <LiquidCrystal_I2C.h>
#include <ClickEncoder.h>
#include <TimerOne.h>

/*****************************************************************************/
/*                                                                           */
/* Defines                                                                   */
/*                                                                           */
/*****************************************************************************/
// Rotary encoder
#define RESW_PIN    2
#define RECLK_PIN   3
#define REDT_PIN    4

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
ClickEncoder* clickEncoder = new ClickEncoder( RECLK_PIN, REDT_PIN, RESW_PIN, 4 );
int16_t clickEncoderValue;
int16_t clickEncoderLast = -1;

// LCD screen
LiquidCrystal_I2C myLcd( 0x27, 16, 2 );

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
    pinMode( RECLK_PIN, INPUT );
    pinMode( REDT_PIN, INPUT );
    pinMode( RESW_PIN, INPUT );//INPUT_PULLUP );
    Timer1.initialize( 1000 );
    Timer1.attachInterrupt( timerIsr );
    clickEncoder->setAccelerationEnabled( false );

    Serial.begin( 9600 );
    Wire.begin();
    myLcd.begin( 16, 2 );

    myLcd.backlight();
    myLcd.setCursor( 0, 0 );
    myLcd.print( "OBRC" );
    myLcd.setCursor( 0, 1 );
    myLcd.print( "v0.1" );
//    delay( 2000 );
//    myLcd.clear();
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
    debugClickEncoder();


    // Menu logic

    if( buttonPressed )
    {
        buttonPressed = false;
    }

    if( knobTurnedCW )
    {
        knobTurnedCW = false;
    }

    if( knobTurnedCCW )
    {
        knobTurnedCCW = false;
    }

    // Rotary encoder logic

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
