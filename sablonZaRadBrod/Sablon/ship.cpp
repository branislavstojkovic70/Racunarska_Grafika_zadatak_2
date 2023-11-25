#include "ship.h"
Ship::Ship() {
    gear = 0;
    speedometer = 0;
    odometer = 0;
    fuelAmount = maxFuelAmount;
    engineTemperature = 0;
    tirePressure = 0;
    tachometer = 0;
    isEngineOn = false;
    //Warning lights
    absLight = false;
    strongHeadLight = false;
    brakeWarningLight = false;
    coolantLight = false;
    lowFuelLight = false;
    lowTirePressureLight = false;
    doorOpenLight = false;
    beltLight = false;
    turnLeftLight = false;
    turnRightLight = false;
    airbagLight = false;
    batteryLight = false;
    lightNotWorking = false;
    oilPressureLow = false;
    checkEngine = false;
    brakeActivated = false;
}

//getters return the value of a property
float Ship::getMaxSpeed() const {
    return maxSpeed;
}

int Ship::getGear() const {
    return gear;
}

float Ship::getSpeed() const {
    return speedometer;
}

float Ship::getOdometer() const {
    return odometer;
}
float Ship::getMaxFuelAmount() const {
    return maxFuelAmount;

}
float Ship::getFuelAmount() const {
    return fuelAmount;
}

float Ship::getEngineTemperature() const {
    return engineTemperature;
}

float Ship::getTirePressure() const {
    return tirePressure;
}

float Ship::getTachometer() const {
    return tachometer;
}


bool Ship::getAbsLight() const {
    return absLight;
}

bool Ship::getStrongHeadLight() const {
    return strongHeadLight;
}
bool Ship::getBrakeWarningLight() const {
    return brakeWarningLight;
}


bool  Ship::getCoolantLight() const {
    return coolantLight;
}

bool Ship::getLowFuelLight() const {
    return lowFuelLight;
}

bool  Ship::getLowTirePressureLight() const {
    return lowTirePressureLight;
}

bool  Ship::getDoorOpenLight() const {
    return doorOpenLight;
}

bool  Ship::getBeltLight() const {
    return beltLight;
}

bool  Ship::getTurnLeftSignal() const {
    return turnLeftLight;
}

bool  Ship::getTurnRightSignal() const {
    return turnRightLight;
}
bool  Ship::getIsEngineOn() const {
    return isEngineOn;
}

bool Ship::getBatteryProblemLight() const {
    return batteryLight;
}
bool Ship::getAirbagLight() const {
    return airbagLight;
}
bool Ship::getLightNotWorking() const {
    return lightNotWorking;
}
bool Ship::getOilPressureLow() const {
    return oilPressureLow;
}
bool Ship::getCheckEngineLight() const {
    return checkEngine;
}
bool Ship::getBrakeActivated() const {
    return brakeActivated;
}
//setters
void  Ship::setMaxSpeed(float newMaxSpeed) {
    maxSpeed = newMaxSpeed;
}

void  Ship::setGear(int newGear) {
    if (newGear < -1 || newGear >5) {
        throw std::invalid_argument("Ship can be in gears  -1 to 5");
    }
    gear = newGear;
}

void  Ship::setSpeedometer(float newSpeedometer) {
    if (newSpeedometer < 0)
    {
        throw std::invalid_argument("speed has to be greater than or equal to 0");
    }
    speedometer = newSpeedometer;

}

void  Ship::setOdometer(float newOdometer) {
    odometer = newOdometer;

}

void  Ship::setFuelAmount(float newfuelAmount) {
    if (newfuelAmount > maxFuelAmount) {
        throw std::invalid_argument("fuels level can't be larger than max fuels amount");
    }
    fuelAmount = newfuelAmount;
}

void Ship::setEngineTemperature(float newEngineTemp) {
    engineTemperature = newEngineTemp;
}

void Ship::setTirePressure(float newTirePressure) {
    tirePressure = newTirePressure;
}

void Ship::setTachometer(float newTachometer) {
    if (newTachometer < 1500)
    {
        if (gear != 1)
            gear = gear - 1;
        odometer = 1500;
    }
    else
    {
        tachometer = newTachometer;
    }
}


void Ship::setAbsLight(bool newValue) {
    absLight = newValue;
}

void Ship::setStrongHeadLight(bool newValue) {
    strongHeadLight = newValue;
}

void Ship::setBrakeWarningLight(bool newValue) {
    brakeWarningLight = newValue;
}

void Ship::setCoolantLight(bool newValue) {
    coolantLight = newValue;
}

void Ship::setLowFuelLight(bool newValue) {
    lowFuelLight = newValue;
}

void Ship::setLowTirePressureLight(bool newValue) {
    lowTirePressureLight = newValue;
}

void Ship::setDoorOpenLight(bool newValue) {
    doorOpenLight = newValue;
}

void Ship::setBeltLight(bool newValue) {
    beltLight = newValue;
}

void Ship::setTurnLeftSignal(bool newValue) {
    turnLeftLight = newValue;
}

void Ship::setTurnRightSignal(bool newValue) {
    turnRightLight = newValue;
}
void Ship::setIsEngineOn(bool newValue) {
    isEngineOn = newValue;
}
void Ship::setBatteryLight(bool newValue) {
    batteryLight = newValue;
}
void Ship::setAirbagLight(bool newValue) {
    airbagLight = newValue;
}
void Ship::setLightNotWorking(bool newValue) {
    lightNotWorking = newValue;
}
void Ship::setOilPressureLow(bool newValue) {
    oilPressureLow = newValue;
}
void Ship::setCheckEngine(bool newValue) {
    checkEngine = newValue;
}
void Ship::setBrakeActivated(bool newValue) {
    brakeActivated = newValue;
}
//methods
void Ship::speedUp()
{
    speedometer == maxSpeed ? speedometer : speedometer += 1;
    tachometer = 2.65 * transfer_ratio_of_main_transmitter * transfer_ratios_in_the_gearbox[gear - 1] * speedometer / dynamic_radius_of_wheel;
    speedometer == 1 ? gear = 1 : speedometer;
}
void  Ship::slowDown()
{
    if (speedometer != 0) {
        speedometer -= 1;
        tachometer = 2.65 * transfer_ratio_of_main_transmitter * transfer_ratios_in_the_gearbox[gear - 1] * speedometer / dynamic_radius_of_wheel;
    }
    else
    {
        tachometer = 1500;
    }

}
void  Ship::turnStoppedLightsOn()
{
    turnLeftLight = true;
    turnRightLight = true;
}
void  Ship::turnStoppedLightsOff()
{
    turnLeftLight = false;
    turnRightLight = false;
}
void Ship::calculateFuelSpent(float kmPassed)
{
    float newfuelAmount = fuelAmount;
    switch (gear)
    {
    case 1:
        //polynomial value is an interpolation of fuel consumption curve in relation to velocity, and current gear, 
        newfuelAmount = fuelAmount - kmPassed * (0.00000545455 * pow(speedometer, 4) - 0.00080575 * pow(speedometer, 3) + 0.0433974 * pow(speedometer, 2) - 0.894308 * pow(speedometer, 1) + 20.2636) / 100;
        break;
    case 2:
        newfuelAmount = fuelAmount - kmPassed * (3.2579 * pow(10, -7) * pow(speedometer, 4) - 0.0000770258 * pow(speedometer, 3) + 0.00723163 * pow(speedometer, 2) - 0.273164 * pow(speedometer, 1) + 12.4449) / 100;
        break;
    case 3:
        newfuelAmount = fuelAmount - kmPassed * (-1.8748 * pow(10, -7) * pow(speedometer, 4) + 0.0000478741 * pow(speedometer, 3) - 0.00272499 * pow(speedometer, 2) + 0.017912 * pow(speedometer, 1) + 7.66769) / 100;
        break;
    case 4:
        newfuelAmount = fuelAmount - kmPassed * (3.8153 * pow(10, -8) * pow(speedometer, 4) - 0.0000158767 * pow(speedometer, 3) + 0.00258409 * pow(speedometer, 2) - 0.155483 * pow(speedometer, 1) + 8.81499) / 100;
        break;
    case 5:
        newfuelAmount = fuelAmount - kmPassed * (1.0016 * pow(10, -7) * pow(speedometer, 4) - 0.0000377478 * pow(speedometer, 3) + 0.00513284 * pow(speedometer, 2) - 0.250828 * pow(speedometer, 1) + 8.90571) / 100;
        break;
    default:
        break;
    }
    if (newfuelAmount <= 0.15 * maxFuelAmount)
    {
        lowFuelLight = true;
        if (newfuelAmount <= 0) {
            fuelAmount = 0;
            while (speedometer)
            {
                slowDown();
            }
            isEngineOn = false;
        }
    }
    else
    {
        lowFuelLight = false;
        fuelAmount = newfuelAmount;
    }


}
