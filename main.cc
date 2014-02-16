#include <chipmunk.h>
#include <libintl.h>
#include <locale.h>
#include <glog/logging.h>

#include <ctime>
#include <cstdlib>
#include <cstdint>
#include <cstdio>
#include <iostream>
#include <memory>
#include <vector>

#include "./main.h"
#include "engine/command.h"
#include "engine/world.h"
#include "ui/ui.h"
#ifdef DUMMY
#  include "ui/dummy.h"
#else
#  include "ui/w/wireframeui.h"
#endif

std::unique_ptr<World> world = nullptr;
std::unique_ptr<UI> ui = nullptr;
cpSpace* space = nullptr;

int main(int argc, char** argv)
{
    (void) argc;

    // initialize glog
    google::InitGoogleLogging(argv[0]);
    google::InstallFailureSignalHandler();

    // initialize seed
    unsigned int seedp = static_cast<unsigned int>(time(nullptr));
    LOG(INFO) << "Seed initialized to " << seedp << ".\n";

    // initialize gettext
    char* locale = setlocale(LC_ALL, "");
    bindtextdomain("newhope", "./" LOCALEDIR);
    textdomain("newhope");
    LOG(INFO) << "Locale set to " << locale << ".\n";

    // initialize physics
    space = cpSpaceNew();

    // initialize engine and UI
#ifdef DUMMY
    ui = std::unique_ptr<UI>(new DummyUI()); // this dummy lib is used for testing memory leaks
#else
    ui = std::unique_ptr<UI>(new WireframeUI());
#endif
    world = std::unique_ptr<World>(new World(-10000, -10000, 10000, 10000, seedp));
    world->Initialize();
    ui->Initialize();

    // main loop
    while(ui->Active()) {
        uint32_t next_frame = ui->Now() + static_cast<int>(1000.0/60.0);

        // process keyboard
        std::vector<std::unique_ptr<Command>> commands = ui->GetEvents();
        for(auto const& command : commands) {
            command->Execute();
        }
        commands.clear();

        // advance time
        world->Step();
        // int64_t time_step = next_frame - static_cast<int>(ui->Now());

        // redraw screen
        ui->RedrawScene();
        // int64_t redraw_time = static_cast<int>(next_frame) - static_cast<int>(ui->Now());
        ui->PresentScene();

        // wait for next frame
        uint32_t now = ui->Now();
        // int64_t time_present = static_cast<int>(next_frame) - static_cast<int>(now);

        // std::cout << "Step: " << time_step << " Redrawing: " << redraw_time << " Rendering: " << time_present << "\n";
        
        if(now < next_frame) {
            ui->Wait(next_frame - now);
        }
    }

    ui.reset();
    world.reset();
    cpSpaceFree(space);
    google::ShutdownGoogleLogging();

    return EXIT_SUCCESS;
}


// vim: ts=4:sw=4:sts=4:expandtab
