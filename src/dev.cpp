// dev.cpp
// Developer Settings for use during development and testing
// All flags are 0 = Off, 1 = On

#include "dev.h"

devSettings AR_DEV;

void setDeveloperFlags()
{
    AR_DEV.CHARACTER_CREATION = ON;
    AR_DEV.TELEPORT_OPTION = OFF;
}
