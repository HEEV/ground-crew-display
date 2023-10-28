#include <locale>
#include <thread>
#include <string>
#include <chrono>
#include <cstdlib>
#include <random>

#include "Main/Main.h"
#include "DataSources/Sources.h"
#include "Packets.h"
#include "Widgets/InformationWidget.h"

#include "Pages/MainPage.h"
#include "Pages/SensorPage.h"

GroundCrewDisplay::GroundCrewDisplay() : cmanager("163.11.237.241:5001") {
}

GroundCrewDisplay::~GroundCrewDisplay() {
}

void GroundCrewDisplay::initialise(const juce::String &commandLine) {
    // Register all packet types and channels we are interested in
    REGISTER_TYPE_TO_MANAGER(WheelData, "vel", cmanager);
    REGISTER_TYPE_TO_MANAGER(BatteryVoltage, "bat", cmanager);
    REGISTER_TYPE_TO_MANAGER(EngineTemp, "enTemp", cmanager);
    REGISTER_TYPE_TO_MANAGER(GPSPosition, "gps", cmanager);
    REGISTER_TYPE_TO_MANAGER(WindSpeed, "wind", cmanager);
    REGISTER_TYPE_TO_MANAGER(CarTilt, "tilt", cmanager);

    // Add callbacks for when data is received on a specified channel.
    // These callbacks run on a separate thread, so be careful with data races
    cmanager.addDataReader("vel", std::function([this](WheelData *data) {
        InformationWidget::checkRaceStart();
        // std::cout << data->head().timeRec() << ',' << data->head().timeSent() << std::endl;

        Sources::latency.bufferData(data->head().timeRec(), data->head().timeRec() - data->head().timeSent());
        Sources::distanceTraveled.bufferData(data->head().timeSent(), data->distTravelled());
        Sources::velocity.bufferData(data->head().timeSent(), data->velocity());
    }));
    cmanager.addDataReader("bat", std::function([this](BatteryVoltage *data) {
        InformationWidget::checkRaceStart();
        Sources::battery.bufferData(data->head().timeSent(), data->volt());
    }));
    cmanager.addDataReader("enTemp", std::function([this](EngineTemp *data) {
        InformationWidget::checkRaceStart();
        Sources::engTemp.bufferData(data->head().timeSent(), data->temp());
    }));
    cmanager.addDataReader("wind", std::function([this](WindSpeed *data) {
        InformationWidget::checkRaceStart();
        Sources::wind.bufferData(data->head().timeSent(), data->headSpeed());
    }));
    cmanager.addDataReader("tilt", std::function([this](CarTilt *data) {
        InformationWidget::checkRaceStart();
        Sources::tilt.bufferData(data->head().timeSent(), data->angle());
    }));

#pragma clang diagnostic push
#pragma ide diagnostic ignored "UnusedValue"
    t1 = std::thread(createData);
#pragma clang diagnostic pop

    mainWindow.reset(new MainWindow(getApplicationName()));
}

void GroundCrewDisplay::shutdown() {
    mainWindow = nullptr;
}

void GroundCrewDisplay::systemRequestedQuit() {
    quit();
}

void GroundCrewDisplay::anotherInstanceStarted(const juce::String &commandLine) {
    /* Do nothing */
}

GroundCrewDisplay::MainWindow::MainWindow(juce::String name)
        : DocumentWindow(name,
                         Colours::grey,
                         DocumentWindow::closeButton) {
    mainPage = new MainPage(this);
    sensorPage = new SensorPage(this);

    setUsingNativeTitleBar(true);

    auto &llf = getLookAndFeel();
    llf.setColour(DocumentWindow::backgroundColourId, getBackgroundColour());
    llf.setColour(ColourIds::textColourId, Colours::black);

    setPage(ActivePage::MainPage);

    // Forces GUI to be fullscreen when not debugging, but remain windowed for development
#ifdef DEBUG
    setResizable(true, true);
    centreWithSize(WIDTH, HEIGHT);
#else
    setResizable(false, false);
    Desktop::getInstance().setKioskModeComponent(this, false);
#endif

#pragma clang diagnostic push
#pragma ide diagnostic ignored "VirtualCallInCtorOrDtor"
    setVisible(true);
#pragma clang diagnostic pop
}

GroundCrewDisplay::MainWindow::~MainWindow() {
    delete mainPage;
}

void GroundCrewDisplay::MainWindow::setPage(ActivePage page) {
    int width = getWidth();
    int height = getHeight();

    juce::Component *newPage;
    switch (page) {
        case ActivePage::MainPage:
            newPage = mainPage;
            break;
        case ActivePage::SensorPage:
            newPage = sensorPage;
            break;
        default:
            newPage = mainPage;
            break;
    }

    setContentNonOwned(newPage, true);
#ifdef DEBUG
    setResizable(true, true);
    centreWithSize(width, height);
#else
    setResizable(false, false);
    Desktop::getInstance().setKioskModeComponent(this, false);
#endif

    getContentComponent()->setBoundsRelative(0.0f, 0.0f, 1.0f, 1.0f);
}

void GroundCrewDisplay::MainWindow::closeButtonPressed() {
    JUCEApplication::getInstance()->systemRequestedQuit();
}

void createData() {
    using namespace std::chrono;
    while (true) {
        uint64_t time = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
        Sources::latency.bufferData(time, rand() % 45);
        Sources::distanceTraveled.bufferData(time, rand() % 30 + 1);
        Sources::velocity.bufferData(time, rand() % 30 + 1);
        Sources::battery.bufferData(time, rand() % 12 - 6);
        Sources::engTemp.bufferData(time, rand() % 200 + 90);
        Sources::wind.bufferData(time, rand() % 24 - 12);
        Sources::tilt.bufferData(time, rand() % 180 - 90);
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

// Launch the app
START_JUCE_APPLICATION(GroundCrewDisplay)