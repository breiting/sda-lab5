#include <gtest/gtest.h>

#include <memory>

#include "AlarmObserver.hpp"
#include "DisplayObserver.hpp"
#include "TemperatureSensor.hpp"

TEST(TestObserver, DisplayReceivesValue) {
    auto sensor = std::make_unique<TemperatureSensor>("T1");
    auto display = std::make_shared<DisplayObserver>();

    sensor->Attach(display);
    sensor->SetTemperature(21.5);

    EXPECT_NEAR(display->LastValue(), 21.5, 1e-9);
}

TEST(TestObserver, AlarmWorks) {
    TemperatureSensor sensor("T1");
    auto alarm = std::make_shared<AlarmObserver>(30.0);

    sensor.Attach(alarm);

    sensor.SetTemperature(25.0);
    EXPECT_FALSE(alarm->IsOn());

    sensor.SetTemperature(35.0);
    EXPECT_TRUE(alarm->IsOn());
}

TEST(TestObserver, DetachStopsUpdates) {
    TemperatureSensor sensor("T1");
    auto display = std::make_shared<DisplayObserver>();

    sensor.Attach(display);
    sensor.SetTemperature(10.0);

    sensor.Detach(display);
    sensor.SetTemperature(99.0);

    EXPECT_NEAR(display->LastValue(), 10.0, 1e-9);
    EXPECT_EQ(display->LastSensorName(), "T1");
}

TEST(TestObserver, MultipleObserver) {
    TemperatureSensor sensor("T2");
    auto display = std::make_shared<DisplayObserver>();
    auto alarm = std::make_shared<AlarmObserver>(30.0);

    sensor.Attach(display);
    sensor.Attach(alarm);
    sensor.SetTemperature(10.0);
    EXPECT_FALSE(alarm->IsOn());

    sensor.SetTemperature(99.0);
    EXPECT_TRUE(alarm->IsOn());
    EXPECT_NEAR(display->LastValue(), 99.0, 1e-9);
}
