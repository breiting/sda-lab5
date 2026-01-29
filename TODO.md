# TODO – Observer Pattern

## Goal

Implement an Observer pattern for a temperature sensor system in C++17.
The final result must compile cleanly and all unit tests must pass.

- Subject keeps a list of observers
- Observers get notified when the temperature changes
- Use `shared_ptr` for observers

## Files You Will Work On

You will implement the following files:

Headers (`/inc`):

- `IObserver.hpp`
- `Sensor.hpp`
- `TemperatureSensor.hpp`
- `DisplayObserver.hpp`
- `AlarmObserver.hpp`

Sources (`/src`):

- `Sensor.cpp`
- `TemperatureSensor.cpp`
- `DisplayObserver.cpp`
- `AlarmObserver.cpp`

You must NOT change:

- `tests/test_observer.cpp`
- `CMakeLists.txt`

## Checkpoint 0 – Build the starter project

1. Configure and build:
   - mkdir build
   - cd build
   - cmake ..
   - make
   - make test

Expected:

- Makefiles are generated and build system works
- Expect compilation errors (missing symbols / missing includes).

## Checkpoint 1 – Create the Observer interface (IObserver)

Create `IObserver.hpp` with:

- `class IObserver`
- virtual destructor
- pure virtual method:

  `virtual void Update(const std::string& sensorName, double value) = 0;`

Rationale:

- Observers receive the sensor name and the new value (minimal “push” style).

## Checkpoint 2 – Implement the Subject base class (Sensor) – Header

Create `Sensor.hpp` with:

Forward declare:

- `class IObserver;`

Define:

- `class Sensor`
  - constructor: `explicit Sensor(const std::string& name);`
  - virtual destructor
  - public methods:
    - `void Attach(std::shared_ptr<IObserver> obs);`
    - `void Detach(const std::shared_ptr<IObserver>& obs);`
    - `std::string Name() const;`
  - protected method:
    - `void Notify(double value);`
  - private data:
    - `std::string m_Name;`
    - `std::vector<std::shared_ptr<IObserver>> m_Observers;`

Design notes:

- We store observers as `shared_ptr`

## Checkpoint 3 – Implement Sensor methods (Sensor.cpp)

Create `Sensor.cpp` and implement:

Includes:

- `#include <algorithm>` (IMPORTANT: needed for std::remove)

Implement:

1. `Sensor::Sensor(const std::string& name)`
   - store `name` in `m_Name`

2. `void Sensor::Attach(std::shared_ptr<IObserver> obs)`
   - minimal version: push into `m_Observers`

3. `void Sensor::Detach(const std::shared_ptr<IObserver>& obs)`
   - remove observer using erase-remove idiom:
     `m_Observers.erase(std::remove(...), m_Observers.end());`

4. `std::string Sensor::Name() const`
   - return `m_Name`

5. `void Sensor::Notify(double value)`
   - iterate observers and call:
     `obs->Update(m_Name, value);`

## Checkpoint 4 – Implement the concrete subject (TemperatureSensor)

Create `TemperatureSensor.hpp`:

- define `class TemperatureSensor final : public Sensor`
  - constructor: `explicit TemperatureSensor(const std::string& name);`
  - methods:
    - `void SetTemperature(double t);`
    - `double Temperature() const;`

Create `TemperatureSensor.cpp`:

- constructor calls base: `Sensor(name)`
- `SetTemperature(double t)`:
  - set `m_Temperature = t;` and notify observer
- `Temperature() const` returns `m_Temperature`

## Checkpoint 5 – Implement DisplayObserver (stores last update)

Create `DisplayObserver.hpp`:

- define `class DisplayObserver final : public IObserver`
  - override `Update(...)`
  - add getters:
    - `double LastValue() const;`
    - `std::string LastSensorName() const;`

Create `DisplayObserver.cpp`:

Implement:

- `Update(sensorName, value)` stores both into members
- `LastValue()` returns `m_LastValue`
- `LastSensorName()` returns `m_LastSensorName`

## Checkpoint 6 – Implement AlarmObserver (threshold alarm)

Create `AlarmObserver.hpp`:

- define `class AlarmObserver final : public IObserver`
  - constructor: `explicit AlarmObserver(double threshold);`
  - override `Update(...)`
  - getter: `bool IsOn() const;`
  - private:
    - `double m_Threshold{0.0};`
    - `bool m_On{false};`

Create `AlarmObserver.cpp`:

Implement:

- constructor stores threshold
- `Update(sensorName, value)` sets:
  - `m_On` to true if the value if over the given threshold
- `IsOn()` returns `m_On`

## Checkpoint 7 – Verify Detach behavior

Make sure:

- calling `Detach(observer)` prevents further updates for this observer
