#include <FluidSolver.h>
#include <FluidDomain.h>
#include <Renderer.h>
#include <Canvas.h>

#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <time.h>

static const int GRID_SIZE = 40;
static const int WORLD_SIZE = 1;

int main(int argc, char const *argv[])
{
	FluidDomain fluid_domain(GRID_SIZE, GRID_SIZE, WORLD_SIZE, WORLD_SIZE);
	FluidSolver fluid_solver(GRID_SIZE, GRID_SIZE, float(WORLD_SIZE) / GRID_SIZE, float(WORLD_SIZE) / GRID_SIZE);
	Renderer renderer(0,0, WORLD_SIZE, WORLD_SIZE);
	Canvas canvas(400, 400);

	// Setup
    //fluid_domain.addFluidSource(FluidSource(1.0 / GRID_SIZE, 0.3, 1.0 / GRID_SIZE, 1 - 1.0 / GRID_SIZE, 0.0, 1));
    fluid_domain.addFluidSource(FluidSource(1.0 / GRID_SIZE, 0.2, 0.1, 0.4, 1.5, 5));
    fluid_domain.addFluidSource(FluidSource(0.8, 1.0 - 1.0 / GRID_SIZE, 0.1, 0.4, 1.5, 5));
    //fluid_domain.addFluidSource(FluidSource(0.45, 0.55, 0.1, 0.2, 0.0, 1));
    int n_frames = 500;
	MyFloat seconds_per_frame = 0.02;
	time_t time_start, time_end;
	time(&time_start);

	// Start simulation
	for (int i = 0; i < n_frames; ++i)
	{
		std::cout << "frame " << i << std::endl;

		MyFloat dt;
		// Simulate
		for (MyFloat frame_time = 0; frame_time < seconds_per_frame; frame_time += dt)
		{
			// Calculate dt (for now just set it)
			dt = 0.0005;
            dt = CLAMP(dt, 0, seconds_per_frame - frame_time);

			// Update fluid domain
			fluid_domain.update(dt);

			// Solve
			fluid_solver.stepPICFLIP(fluid_domain, dt, 0.0);
		}
		// Render
		renderer.clearCanvas(canvas);
		renderer.renderGridCellsToCanvas(fluid_domain.macGrid(), canvas);
		renderer.renderParticlesToCanvas(fluid_domain.markerParticleSet(), canvas);
		renderer.renderGridVelocitiesToCanvas(fluid_domain.macGrid(), canvas);
		
		std::stringstream file_name;
        file_name << std::setfill('0') << std::setw(4) << i << ".ppm";
		renderer.writeCanvasToPpm(file_name.str().c_str(), canvas);
	}
	time(&time_end);

	double time_elapsed = difftime(time_end, time_start);

	int hours_elapsed = time_elapsed / (60 * 60);
	int minutes_elapsed = (int(time_elapsed) % (60 * 60)) / 60;
	int seconds_elapsed = int(time_elapsed) % 60;

	std::string elapsed_time_string =
	 	  std::to_string(hours_elapsed) + "h:"
		+ std::to_string(minutes_elapsed) + "m:"
		+ std::to_string(seconds_elapsed) + "s";

	std::cout << "Simulation time : " << elapsed_time_string << std::endl;


	return 0;
}
