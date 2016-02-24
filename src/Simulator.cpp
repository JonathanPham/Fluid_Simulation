#include <Simulator.h>

static const int GRID_SIZE = 40;
static const int WORLD_SIZE = 10;

Simulator::Simulator()
{
	_grid = new MacGrid(
		GRID_SIZE, 		// Size_x
		GRID_SIZE, 		// Size_y
		WORLD_SIZE, 	// Length_x
		WORLD_SIZE); 	// Length_y
	_particle_set = new MarkerParticleSet();
	_renderer = new Renderer(0,0, WORLD_SIZE, WORLD_SIZE);
	
	for (int j = 0; j < _grid->sizeY(); ++j)
	{
		for (int i = 0; i < _grid->sizeX(); ++i)
		{
			_grid->setColor(i, j, j%2);
		}
	}

	// Add a bunch of trace particles at random positions
	//for (int i = 0; i < 500; ++i)
	//{
	//	_particle_set->addParticle(
	//		rand() / double(INT_MAX) * WORLD_SIZE,
	//		rand() / double(INT_MAX) * WORLD_SIZE);
	//}

	// Setup
	int n_frames = 200;
	double seconds_per_frame = 1;
	
	// Start simulation
	for (int i = 0; i < n_frames; ++i)
	{
		std::cout << "iteration " << i << std::endl;

		double dt;
		// Simulate
		for (double frame_time = 0; frame_time < seconds_per_frame; frame_time += dt)
		{
			// Calculate dt (for now just set it)
			dt = 0.01;
			// Update the fluid grid
			//updateCellTypesWithParticles();
			_grid->addExternalForce(dt, 0, 10);
			_grid->advect(dt);
			_grid->enforceDirichlet();
			_grid->pressureSolve(dt);
			_grid->enforceDirichlet();

			// Advect particles through fluid
			advectParticles(dt);
		}

		// Render
		std::stringstream str;
		str << "test" << i << ".ppm";


		_renderer->clearCanvas();
		//_renderer->renderColorToCanvas(_grid);
		//_renderer->renderGridCellsToCanvas(_grid);
		_renderer->renderParticlesToCanvas(_particle_set);
		_renderer->renderGridVelocitiesToCanvas(_grid);
		
		_renderer->writeCanvasToPpm( str.str().c_str());
	}
}

void Simulator::advectParticles(double dt)
{
	for (MarkerParticle* iter = _particle_set->getFirst();
		iter != nullptr;
		iter = iter->next)
	{
		// Position in world
		double pos_x = iter->posX();
		double pos_y = iter->posY();

		// Calculate new position
		pos_x += _grid->velXInterpolated(pos_x, pos_y) * dt;
		pos_y += _grid->velYInterpolated(pos_x, pos_y) * dt;
		
		// Write data
		iter->setPosition(pos_x, pos_y);
	}
}

void Simulator::updateCellTypesWithParticles()
{
	// First reset types (set all cells to AIR)
	_grid->clearCellTypeBuffer();

	// Loop through all particles and set cells to liquid if they
	// contain a particle
	for (MarkerParticle* iter = _particle_set->getFirst();
		iter != nullptr;
		iter = iter->next)
	{
		// Find the particles position in the grid
		int x = (iter->posX() / _grid->lengthX()) * _grid->sizeX();
		int y = (iter->posY() / _grid->lengthY()) * _grid->sizeY();

		_grid->setCellType(x, y, LIQUID);
	}
}

Simulator::~Simulator()
{
	delete _grid;
	delete _particle_set;
	delete _renderer;
}