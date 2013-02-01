#ifdef __unix
#  include <csignal>
#endif

#ifdef _SDL
#  include "libs/sdl/sdl.h"
#else
#  include "libs/null/nulllib.h"
#endif

#include "ui/ui.h"
#include "util/i18n.h"
#include "util/logger.h"
#include "world/world.h"

Logger logger(true, false);
static UI* ui;

static void ctrlc_handler(int s) {
	logger.Debug("CTRL+C caught!");
	ui->TurnOff();
}

int main(int argc, char** argv)
{
	// initialization
	i18n_init();
	World world(5000, 5000);
#ifdef _SDL
	GraphicLibrary* video(new SDL());
#else
	GraphicLibrary* video(new NullLibrary());
#endif
	ui = new UI(world, *video);

	// set signal to catch CTRL+C
#ifdef __unix
	struct sigaction s;
	s.sa_handler = ctrlc_handler;
	sigemptyset(&s.sa_mask);
	s.sa_flags = 0;
	sigaction(SIGINT, &s, NULL);
#endif

	// main loop
	while(ui->Active()) {
		ui->StartFrame();
		ui->Draw();
		ui->ProcessEvents();
		world.Process();
		ui->EndFrame();
	}
	
	// clean up
	delete ui;
	delete video;
}
