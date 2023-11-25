#include "TestBed.h"
#include <thread>
#include <random>
using seconds = std::chrono::duration<double>;
#define km_conversion pow(10, -6)   / 3600 

double randomNumber() {
    std::mt19937 generator(static_cast<unsigned int>(std::time(0)));
    std::uniform_real_distribution<double> distribution(0.0, 1.0);
    return distribution(generator);
}
void checkGear(Ship* ship) {
    double speedRandomizer = randomNumber();
    double speed = ship->getSpeed();
    if (ship->getTachometer() <= 1500 && ship->getGear() == 2)
    {
        ship->setGear(1);
    }
    if ((speed > 20 + speedRandomizer * 5 && speed < 35 + speedRandomizer * 5) || (ship->getGear() == 3 && ship->getTachometer() <= 1500) || (ship->getGear() == 1 && ship->getTachometer() >= 3000)) {
        ship->setGear(2);
        return;
    }

    if ((speed >= 35 + speedRandomizer * 5 && speed < 55 + speedRandomizer * 5) || (ship->getGear() == 4 && ship->getTachometer() <= 1500) || (ship->getGear() == 2 && ship->getTachometer() >= 3000)) {
        ship->setGear(3);
        return;
    }

    if ((speed >= 55 + speedRandomizer * 5 && speed < 75 + speedRandomizer * 5) || (ship->getGear() == 5 && ship->getTachometer() <= 1500) || (ship->getGear() == 3 && ship->getTachometer() >= 3000)) {
        ship->setGear(4);
        return;
    }

    if (speed >= 75 + speedRandomizer * 5 || (ship->getGear() == 4 && ship->getTachometer() >= 3000))
    {
        ship->setGear(5);
        return;
    }
}

void someoneExitsship(Ship* ship) {
    int slowDownTime = 166 + 300 * randomNumber();
    while (ship->getSpeed() > 0)
    {
        auto iteration_start = std::chrono::system_clock::now();
        ship->slowDown();
        std::this_thread::sleep_for(std::chrono::milliseconds(slowDownTime));
        checkGear(ship);
        ship->setOdometer(ship->getOdometer() + (std::chrono::system_clock::now() - iteration_start).count() * ship->getSpeed() * km_conversion);
        ship->calculateFuelSpent((std::chrono::system_clock::now() - iteration_start).count() * ship->getSpeed() * km_conversion);
    }
    ship->turnStoppedLightsOn();
    ship->setBeltLight(true);
    int exitTime = 10000 + randomNumber() * 30000;
    std::this_thread::sleep_for(std::chrono::milliseconds(exitTime));
    ship->setDoorOpenLight(true);
    std::this_thread::sleep_for(std::chrono::milliseconds(3000));
    ship->setDoorOpenLight(false);
    ship->turnStoppedLightsOff();
}

void turn(Ship* ship)
{
    double direction = randomNumber();
    direction > 0.5 ? ship->setTurnLeftSignal(true) : ship->setTurnRightSignal(true);
    double turnSpeed = 10 + 15 * randomNumber();
    int slowDownTime = 166 + 300 * randomNumber();
    //ship is moving too fast to make a turn
    while (ship->getSpeed() > turnSpeed)
    {
        auto iteration_start = std::chrono::system_clock::now();
        ship->slowDown();
        std::this_thread::sleep_for(std::chrono::milliseconds(slowDownTime));
        checkGear(ship);
        ship->setOdometer(ship->getOdometer() + (std::chrono::system_clock::now() - iteration_start).count() * ship->getSpeed() * km_conversion);
        ship->calculateFuelSpent((std::chrono::system_clock::now() - iteration_start).count() * ship->getSpeed() * km_conversion);
    }
    //ship was slowing down or stopped and is now making a turn
    while (ship->getSpeed() < turnSpeed)
    {
        auto iteration_start = std::chrono::system_clock::now();
        ship->speedUp();
        std::this_thread::sleep_for(std::chrono::milliseconds(slowDownTime));
        checkGear(ship);
        ship->setOdometer(ship->getOdometer() + (std::chrono::system_clock::now() - iteration_start).count() * ship->getSpeed() * km_conversion);
        ship->calculateFuelSpent((std::chrono::system_clock::now() - iteration_start).count() * ship->getSpeed() * km_conversion);
    }
    //time it takes to make a turn
    std::this_thread::sleep_for(std::chrono::milliseconds(3000));
    direction > 0.5 ? ship->setTurnLeftSignal(false) : ship->setTurnRightSignal(false);

}

void overtake(Ship* ship)
{
    double overtakingSpeed = ship->getSpeed() + ship->getSpeed() * (0.5 - 0.1 * randomNumber() - 0.07 * ship->getGear());
    double overtakingDuration = 166 + 166 * randomNumber();
    ship->setTurnLeftSignal(true);
    while (ship->getSpeed() < overtakingSpeed)
    {
        auto iteration_start = std::chrono::system_clock::now();
        ship->speedUp();
        std::this_thread::sleep_for(std::chrono::milliseconds((int)overtakingDuration));
        checkGear(ship);
        ship->setOdometer(ship->getOdometer() + (std::chrono::system_clock::now() - iteration_start).count() * ship->getSpeed() * km_conversion);
        ship->calculateFuelSpent((std::chrono::system_clock::now() - iteration_start).count() * ship->getSpeed() * km_conversion);
    }
    ship->setTurnLeftSignal(false);
    ship->setTurnRightSignal(true);
    std::this_thread::sleep_for(std::chrono::milliseconds(3000));
    ship->setTurnRightSignal(false);
    ship->setOdometer(ship->getOdometer() + (3 * pow(10, 7) * ship->getSpeed() * km_conversion));
    ship->calculateFuelSpent(3000000 * ship->getSpeed() * km_conversion);
}
void speedUp(Ship* ship)
{
    ship->speedUp();
    checkGear(ship);
}
void slowDown(Ship* ship) {
    ship->slowDown();
    checkGear(ship);
}
void testWarningLightsAtshipStart(Ship* ship)
{
    ship->setAbsLight(true);
    ship->setCheckEngine(true);
    ship->setAirbagLight(true);
    ship->setBatteryLight(true);
    ship->setBeltLight(true);
    ship->setBrakeWarningLight(true);
    ship->setCheckEngine(true);
    ship->setCoolantLight(true);
    ship->setLightNotWorking(true);
    ship->setLowTirePressureLight(true);
    ship->setOilPressureLow(true);
    ship->setBrakeActivated(true);
    std::this_thread::sleep_for(std::chrono::milliseconds(4000));
    double abs_working_probability = randomNumber();
    if (abs_working_probability > 0.008)
    {
        ship->setAbsLight(false);
    }
    double engine_working_probability = randomNumber();
    if (engine_working_probability > 0.02)
    {
        ship->setCheckEngine(false);
    }
    double airbag_working_probability = randomNumber();
    if (abs_working_probability > 0.002)
    {
        ship->setAirbagLight(false);
    }
    double battery_working_probability = randomNumber();
    if (battery_working_probability > 0.005)
    {
        ship->setBatteryLight(false);
    }
    double fastened_belt_probability = randomNumber();
    if (fastened_belt_probability > 0.01)
    {
        ship->setBeltLight(false);
    }
    double brake_working_probability = randomNumber();
    if (brake_working_probability > 0.002)
    {
        ship->setBrakeWarningLight(false);

    }
    double coolant_normal_probability = randomNumber();
    if (coolant_normal_probability > 0.002)
    {
        ship->setCoolantLight(false);
    }
    double lights_working_probability = randomNumber();
    if (lights_working_probability > 0.01)
    {
        ship->setLightNotWorking(false);
    }
    double tire_pressure_good_probability = randomNumber();
    if (tire_pressure_good_probability > 0.008)
    {
        ship->setLowTirePressureLight(false);
    }
    double oil_pressure_good_probability = randomNumber();
    if (oil_pressure_good_probability > 0.006)
    {
        ship->setOilPressureLow(false);
    }
    ship->setBrakeActivated(false);
}
void runSimulation(Ship* ship) {
    ship->setIsEngineOn(true);
    if (ship->getGear() == 0) {
        double door_not_closed_properly = randomNumber();
        if (door_not_closed_properly < 0.05)
        {
            ship->setDoorOpenLight(true);
            std::this_thread::sleep_for(std::chrono::milliseconds(5000));
            ship->setDoorOpenLight(false);
        }
        if (ship->getSpeed() == 0)
        {
            testWarningLightsAtshipStart(ship);
            ship->setGear(1);
        }

        while (ship->getSpeed() < 30)
        {
            auto iteration_beginnign_time = std::chrono::system_clock::now();
            ship->speedUp();
            std::this_thread::sleep_for(std::chrono::milliseconds(166));
            ship->setOdometer(ship->getOdometer() + ship->getSpeed() * (std::chrono::system_clock::now() - iteration_beginnign_time).count() * km_conversion);
            ship->calculateFuelSpent(ship->getSpeed() * (std::chrono::system_clock::now() - iteration_beginnign_time).count() * km_conversion);
        }

    }
    while (ship->getIsEngineOn()) {
        auto last_odometer_calculation_time = std::chrono::system_clock::now();
        auto last_fuel_calculation_time = std::chrono::system_clock::now();
        checkGear(ship);
        double speedUpChance = randomNumber();
        if (speedUpChance > (0.70)) {
            if (speedUpChance <= 1 - (ship->getGear()) * 0.05) {
                std::this_thread::sleep_for(std::chrono::milliseconds(166));
                ship->speedUp();
            }
            else
            {
                std::this_thread::sleep_for(std::chrono::milliseconds(166));
                ship->slowDown();
            }
            checkGear(ship);
        }
        //brake is activated
        double brakeChance = randomNumber();
        if (brakeChance < 0.035) {
            ship->setBrakeActivated(true);
            double slowDownAmount = randomNumber();
            int slowDownTime = 166 + 300 * randomNumber();
            for (int i = 0; i <= 5 + (int)ship->getSpeed() * 0.7 * slowDownAmount; i++) {
                std::this_thread::sleep_for(std::chrono::milliseconds(slowDownTime));
                ship->slowDown();
            }
            checkGear(ship);
            ship->setBrakeActivated(false);
        }
        //belt taken off while ship is working
        double change_belt_status_chance = randomNumber();
        if (change_belt_status_chance < 0.008)
        {
            if (change_belt_status_chance < 0.005) {
                someoneExitsship(ship);
            }
            ship->setBeltLight(!ship->getBeltLight());
        }
        //faulty brakes
        double brakes_faulty_probability = randomNumber();
        if (!ship->getBrakeWarningLight() && brakes_faulty_probability < 0.0045)
        {
            ship->setOdometer(ship->getOdometer() + ship->getSpeed() * (std::chrono::system_clock::now() - last_odometer_calculation_time).count() * km_conversion);
            ship->setBrakeWarningLight(true);
            last_odometer_calculation_time = std::chrono::system_clock::now();
        }
        //headlights
        double strong_headlight_change_probability = randomNumber();
        if (strong_headlight_change_probability < 0.015)
        {
            ship->setStrongHeadLight(!ship->getStrongHeadLight());
        }
        double abs_working_probability = randomNumber();
        if (abs_working_probability < 0.004)
        {
            ship->setAbsLight(true);
        }
        double engine_problems = randomNumber();
        if (engine_problems < 0.008)
        {
            ship->setCheckEngine(true);
        }
        double oilPressureLow = randomNumber();
        if (oilPressureLow < 0.008)
        {
            ship->setOilPressureLow(true);
        }
        double lightNotWorking = randomNumber();
        if (lightNotWorking < 0.01)
        {
            ship->setLightNotWorking(true);
        }
        //turn
        double turnChance = randomNumber();
        if (turnChance < 0.08 - (ship->getGear() - 1) * 0.015)
        {
            ship->setOdometer(ship->getOdometer() + ship->getSpeed() * (std::chrono::system_clock::now() - last_odometer_calculation_time).count() * km_conversion);
            ship->calculateFuelSpent(ship->getSpeed() * (std::chrono::system_clock::now() - last_fuel_calculation_time).count() * km_conversion);
            turn(ship);
            last_odometer_calculation_time = std::chrono::system_clock::now();
            last_fuel_calculation_time = std::chrono::system_clock::now();
        }
        double overtakeChance = randomNumber();
        if (overtakeChance < 0.03)
        {
            ship->setOdometer(ship->getOdometer() + ship->getSpeed() * (std::chrono::system_clock::now() - last_odometer_calculation_time).count() * km_conversion);
            ship->calculateFuelSpent(ship->getSpeed() * (std::chrono::system_clock::now() - last_fuel_calculation_time).count() * km_conversion);
            overtake(ship);
            last_odometer_calculation_time = std::chrono::system_clock::now();
            last_fuel_calculation_time = std::chrono::system_clock::now();
        }
        ship->setOdometer(ship->getOdometer() + ship->getSpeed() * (std::chrono::system_clock::now() - last_odometer_calculation_time).count() * km_conversion);
        ship->calculateFuelSpent(ship->getSpeed() * (std::chrono::system_clock::now() - last_fuel_calculation_time).count() * km_conversion);
        last_fuel_calculation_time = std::chrono::system_clock::now();
        double batteryProblemsProbability = randomNumber();
        if (batteryProblemsProbability < 0.02) {
            ship->setBatteryLight(true);
        }

        double airbagProblemsProbability = randomNumber();
        if (airbagProblemsProbability < 0.001) {
            ship->setAirbagLight(true);
        }

        double lowTireProbability = randomNumber();
        if (lowTireProbability < 0.018) {
            ship->setLowTirePressureLight(true);
        }
    }

}

Ship getShip()
{

    return  Ship();
}
void startSimulation(Ship* ship)
{
    std::thread simuation(runSimulation, ship);
    simuation.detach();

}
void endSimulation(Ship* ship)
{
    ship->setIsEngineOn(false);
}
