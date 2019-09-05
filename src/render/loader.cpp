/*
 * tilehnd.cpp
 *
 *  Created on: Aug 20, 2017
 *      Author: Tim Stark
 */

#include "main/core.h"
#include "render/terrain.h"

TerrainHandler::TerrainHandler()
{
//	std::cout << "Startup tile load handler" << std::endl;
	msFreq = 50;
	start();
}

TerrainHandler::~TerrainHandler()
{
	if (loader.joinable())
		shutdown();
}

void TerrainHandler::start()
{
	// Start handle() thread process
	loader = std::thread([=]{ handle(); });
}

void TerrainHandler::shutdown()
{
	// Terminate handle() thread process
	runHandler = false;
	loader.join();
}

void TerrainHandler::handle()
{
	const int nTiles = 12;
	TerrainTile *tile[nTiles];
	int   nCount = 0;

//	std::cout << "Start handle() thread process" << std::endl;

	runHandler = true;
	while (runHandler) {
		// Loading tile(s) from queue
		if (!tiles.empty()) {
			std::unique_lock<std::mutex> mu1(mu_queue);
			while (!tiles.empty() && (nCount < nTiles)) {
				tile[nCount++] = tiles.front();
				tiles.pop();
			}
			mu1.unlock();
//			std::cout << "Processing " << nCount << " tiles.." << std::endl;
		}

		// Loading texture images
		if (nCount > 0) {
			std::unique_lock<std::mutex> mu2(mu_loading);
			for (int idx = 0; idx < nCount; idx++)
				tile[idx]->load();
			mu2.unlock();
			nCount = 0;
		}

		// Idle period
		std::this_thread::sleep_for(std::chrono::milliseconds(msFreq));
	}

//	std::cout << "Terminated handle() thread process" << std::endl;
}

void TerrainHandler::queue(TerrainTile *tile)
{
	if (tile == nullptr)
		return;
	if (tile->state == TerrainTile::InQueue)
		return;

	tile->state = TerrainTile::InQueue;
	tiles.push(tile);
}

bool TerrainHandler::unqueue(TerrainTile *tile)
{
	if (tile == nullptr)
		return false;
	if (tiles.empty())
		return false;
	if (tile->state != TerrainTile::InQueue)
		return false;

	std::unique_lock<std::mutex> mu(mu_queue);

	mu.unlock();
	return true;
}

void TerrainHandler::unqueue(TerrainManager *mgr)
{
	if (mgr == nullptr)
		return;
	if (tiles.empty())
		return;
}
