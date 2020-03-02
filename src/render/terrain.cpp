/*
 * terrain.cpp - Terrain manager package
 *
 *  Created on: Aug 22, 2019
 *      Author: Tim Stark
 */

#include "main/core.h"
#include "engine/engine.h"
#include "engine/object.h"
#include "render/gl/texture.h"
#include "render/render.h"
#include "render/terrain.h"

// Global parameters
TerrainHandler *TerrainManager::loader = nullptr;

static tcrd_t fullRange = { 0.0f, 1.0f, 0.0f, 1.0f };

TerrainTile::TerrainTile(TerrainManager &mgr, uint32_t lod, uint32_t ilat, uint32_t ilng, TerrainTile *parent)
: Tree(parent), tmgr(mgr), lod(lod), ilat(ilat), ilng(ilng), tcRange(fullRange)
{
	center = calculateCenter();
	// cout << "    New tile: lod = " << lod << " ilat = " << ilat << " ilng = " << ilng << endl;

	// mesh = Mesh::createSphere(lod, ilat, ilng, 32, tcRange);
	// if (mesh != nullptr) {
	// 	mesh->allocate(tmgr.scene.getContext());
	// 	mesh->texImage = texImage;
	// }
}

TerrainTile::~TerrainTile()
{
    if (mesh != nullptr)
        delete mesh;
}

TerrainTile *TerrainTile::createChild(int idx)
{
	TerrainTile *child = nullptr;

	int nlod = lod + 1;
	int nlat = ilat*2 + (idx / 2);
	int nlng = ilng*2 + (idx % 2);

	// cout << "Current tile: lod = " << lod << " ilat = " << ilat << " ilng = " << ilng << endl;
	// cout << "    New tile: lod = " << nlod << " ilat = " << nlat << " ilng = " << nlng << " index = " << idx << endl;

	child = new TerrainTile(tmgr, nlod, nlat, nlng, this);
	if (child != nullptr)
		tmgr.loader->queue(child);
	addChild(idx, child);

	return child;
}

void TerrainTile::split()
{
	TerrainTile *child = nullptr;

	for (int idx = 0; idx < 4; idx++) {
		if ((child = getChild(idx)) == nullptr)
			child = createChild(idx);
		else if (child->state == Invalid)
			child->load();
	}
}

vec3d_t TerrainTile::calculateCenter()
{
	int nlat = 1 << lod;
	int nlng = 2 << lod;

	double mlat0 = PI * double(ilat) / double(nlat);
	double mlat1 = PI * double(ilat+1) / double(nlat);
    // float mlng0 = PI*2 * (float(nlng/2 - ilng-1) / float(nlng)) - PI;
    // float mlng1 = PI*2 * (float(nlng/2 - ilng) / float(nlng)) - PI;
    double mlng0 = PI*2 * (double(ilng) / double(nlng)) - PI;
    double mlng1 = PI*2 * (double(ilng+1) / double(nlng)) - PI;

	// float latc = (PI/2) - PI * ((float(ilat)+0.5f) / float(nlat));
	// float lngc = (PI*2) * ((float(nlng/2 - ilng-1)+0.5) / float(nlng)) - PI;
 	// if (lngc < PI*2)
    //     lngc += PI*2;

 	double latc = PI * ((double(ilat)+0.5f) / double(nlat));
	double lngc = PI*2 * ((double(ilng)+0.5f) / double(nlng)) - PI;
 
	double slat = sin(latc), clat = cos(latc);
	double slng = sin(lngc), clng = cos(lngc);

	// cout << "Index:  (" << ilat << "," << ilng << ") of (" << nlat << "," << nlng
	//      << ") at LOD " << lod+3 << endl;
	// cout << "Center: (" << toDegree(latc)
	// 	 << "," << toDegree(lngc) << ")" << endl;
	// cout << "Coordinate: ( " << toDegree(mlng0) << " , " << toDegree(mlat0) << " ) - ( " 
	//                          << toDegree(mlng1) << " , " << toDegree(mlat0) << " )" << endl;
	// cout << "            ( " << toDegree(mlng0) << " , " << toDegree(mlat1) << " ) - ( " 
	//                          << toDegree(mlng1) << " , " << toDegree(mlat1) << " )" << endl;
						 
	// return vec3d_t(clat*clng, slat, clat*-slng);
	return vec3d_t(slat*clng, clat, slat*-slng);
}

void TerrainTile::setSubTexCoordRange(const tcrd_t &ptcr)
{
	// if ((ilng & 1) == 0) { // Right column
	if (ilng & 1) { // Right column
		tcRange.tumin = (ptcr.tumin + ptcr.tumax) / 2.0;
		tcRange.tumax = ptcr.tumax;
	} else {       // Left column
		tcRange.tumin = ptcr.tumin;
		tcRange.tumax = (ptcr.tumin + ptcr.tumax) / 2.0;
	}

	if (ilat & 1) { // Top row
		tcRange.tvmin = (ptcr.tvmin + ptcr.tvmax) / 2.0;
		tcRange.tvmax = ptcr.tvmax;
	} else {       // Bottom row
		tcRange.tvmin = ptcr.tvmin;
		tcRange.tvmax = (ptcr.tvmin + ptcr.tvmax) / 2.0;
	}
}

void TerrainTile::load()
{
	state = Loading;

	// Orbiter data files (database system)
	string surfName = fmt::sprintf("%s/surf/%02d/%06d/%06d.jpg",
		tmgr.surfFolder, lod+3, ilat, ilng);
	string maskName = fmt::sprintf("%s/mask/%02d/%06d/%06d.jpg",
		tmgr.surfFolder, lod+3, ilat, ilng);
	string cloudName = fmt::sprintf("%s/cloud/%02d/%06d/%06d.jpg",
		tmgr.surfFolder, lod+3, ilat, ilng);

	texImage = Texture::create(surfName);
	if (texImage == nullptr) {
    	// Non-existent tile. Have to load
    	// lower LOD tile from parent tile
    	// and set sub texture range.
    	TerrainTile *ptile = getParent();
    	if (ptile != nullptr) {
    		texImage = ptile->getTexture();
    		texOwn   = false;
    		setSubTexCoordRange(ptile->tcRange);
    		// std::cout << "Subtexture created" << std::endl;
    	}
	} else
		texOwn = true;

	elev = getElevationData();

	mesh = tmgr.createSphere(lod, ilat, ilng, 32, tcRange);
	if (mesh != nullptr) {
		// mesh->allocate(tmgr.scene.getContext());
		mesh->texImage = texImage;
	}

	state = Inactive;
}

void TerrainTile::render(renderParameter &prm)
{
	if (mesh != nullptr) {
		mesh->render(tmgr.scene.getContext(), prm);
	}
}

// **************************************************

TerrainManager::TerrainManager(Scene &scene, const Object &object)
: scene(scene), body(object), maxLOD(20), resGrid(32), resElev(1.0)
{
	ShaderManager &smgr = scene.getShaderManager();

	pgm = smgr.createShader("planet");

	string sysFolder = scene.getEngine()->getSystemFolder();

	switch (body.getType())
	{
	case objCelestialBody:
		const CelestialBody *obj = dynamic_cast<const CelestialBody *>(&body);
		const PlanetarySystem *system = obj->getInSystem();
		string starName = system->getStar()->getName();
		string bodyName = body.getName();

		surfFolder = fmt::sprintf("%s/%s/%s/terrain/orbiter",
			sysFolder, starName, bodyName);
		break;
	}

//	cout << "Path name: " << surfFolder << endl;

	// Initialize root of terrain tiles
	for (int idx = 0; idx < 2; idx++) {
		terrain[idx] = new TerrainTile(*this, 0, 0, idx);
		terrain[idx]->load();
	}
}

TerrainManager::~TerrainManager()
{
    // Delete all root tiles
	for (int idx = 0; idx < 2; idx++)
		delete terrain[idx];
}

void TerrainManager::ginit(Scene &scene)
{
	// Initialize and start up thread process for loading tiles
	loader = new TerrainHandler();
}

void TerrainManager::gexit()
{
	if (loader != nullptr) {
		delete loader;
		loader = nullptr;
	}
}

void TerrainManager::convertDoubleToTwoFloats(vec3d_t val, vec3f_t &high, vec3f_t &low)
{
//	high = vec3f_t(val);
//	low = vec3f_t(val - vec3d_t(high));

	high.x = float(val.x);
	high.y = float(val.y);
	high.z = float(val.z);
	low.x  = float(val.x - high.x);
	low.y  = float(val.y - high.y);
	low.z  = float(val.z - high.z);

//	cout << "Value:  " << val.x << "," << val.y << "," << val.z << endl;
//	cout << "  High: " << high.x << "," << high.y << "," << high.z << endl;
//	cout << "  Low:  "  << low.x << "," << low.y << "," << low.z << endl;

}

void TerrainManager::process(TerrainTile *tile, renderParameter &prm)
{
	int lod  = tile->lod;
	int nlat = 1 << tile->lod;
	int nlng = 2 << tile->lod;

	// farthest edge of quad tile radius
	constexpr double trad0 = sqrt(2.0)*(PI/2);

	double trad, alpha, adist;
	double erad;
	double tdist, apr;
	int    tlod;
	bool   splitFlag = false;
    int    bias = 4;

	tile->state = TerrainTile::Rendering;

	// Find angle between camera and tile center
	trad = trad0 / double(nlat);
	alpha = acos(glm::dot(prm.obj.cdir, -tile->center));
	adist = alpha - trad;
	
	// cout << "Tile Position:    (" << tile->center.x << "," << tile->center.y << "," << tile->center.z << ")" << endl;
	// cout << "Angle from center: " << toDegree(alpha) << endl;

	// Check if tile is visible from camera position
	// If tile is hiding from camera position, mark
	// tile as invisible (LOD level 1+).
	if (adist >= prm.obj.viewap) {
		// cout << "Out of view: " << tile->lod+3 << "," << tile->ilat << "," << tile->ilng << ") "
		// 	 << toDegree(adist) << " >= " << toDegree(prm.obj.viewap) << endl;
    	tile->state = TerrainTile::Invisible;
    	return;
	}
	// cout << "In View: (LOD " << tile->lod+3 << "," << tile->ilat << "," << tile->ilng << ") "
	// 	 << toDegree(adist) << " >= " << toDegree(prm.obj.viewap) << endl;

	// Check if tile is visible in view


	// Check LOD level from tile distance
	{
		erad = prm.obj.orad;
		if (adist < 0.0) {
			tdist = prm.obj.cdist - erad;
        	// cout << "*** Above tile (LOD " << tile->lod+3 << "," << tile->ilat << "," << tile->ilng << ")"
			// 	 << " Distance from ground " << erad << endl;
		} else {
			double h = erad * sin(adist);
			double a = prm.obj.cdist - (erad * cos(adist));
			tdist = sqrt(h*h + a*a);
		}
    	bias -= 2.0 * sqrt(std::max(0.0, adist) / prm.obj.viewap);
		apr = tdist * prm.tanap;
		if (apr > 0.000001)
			tlod = std::max(0, std::min(int(prm.obj.maxLOD), int(bias - log(apr)*1.1)));
		else
			tlod = prm.obj.maxLOD;
    	tlod += prm.obj.biasLOD;
		splitFlag = (lod < tlod+1);
	}

//    split = false;
	if (splitFlag == true) {
//        std::cout << "Tile split at LOD " << lod+4 << "(Expected LOD " << tlod+4 << ")" << std::endl;
       bool valid = true;
       // Check children that have valid flag

		for (int idx = 0; idx < 4; idx++) {
			TerrainTile *child = tile->getChild(idx);
			if (child == nullptr)
				child = tile->createChild(idx);
			else if (child->state == TerrainTile::Invalid)
				loader->queue(child);
           if ((child->state & TILE_VALID) == 0)
            	valid = false;
		}
       // When all children have valid flags, process nodes in next LOD level.
       if (valid) {
        	tile->state = TerrainTile::Active;
        	for (int idx = 0; idx < 4; idx++)
            	process(tile->getChild(idx), prm);
       }
	} else {
//		std::cout << "Tile LOD Level: " << lod+4 << " (" << tile->lat << ","
//				  << tile->lng << ")" << std::endl;
//		std::cout << "Alpha: " << toDegrees(alpha) << " Distance: " << toDegrees(adist)
//				<< " Tile Distance: " << tdist << std::endl;
//		std::cout << "Aperture: " << apr << " LOD: " << lod+4
//				<< " Tile Center LOD: " << tlod+4 << std::endl;
	}

}

void TerrainManager::render(TerrainTile *tile, renderParameter &prm)
{

	if (tile->state == TerrainTile::Rendering)
		tile->render(prm);
	else if (tile->state == TerrainTile::Active)
	{
		for (int idx = 0; idx < 4; idx++) {
			TerrainTile *child = tile->getChild(idx);
			if (child != nullptr && (child->state & TILE_ACTIVE))
				render(child, prm);
		}
	}
}

void TerrainManager::render(renderParameter &prm)
{
	pgm->use();

	prm.obj.maxLOD = 18;
	prm.obj.biasLOD = 0;

	// Object position and orientation parameters
	prm.obj.opos  = body.getPosition(prm.now);
	prm.obj.oqrot = body.getRotation(prm.now);
//	prm.obj.oqrot = quatd_t(1, 0, 0, 0);
	prm.obj.orot  = glm::toMat4(prm.obj.oqrot);
	prm.obj.orad  = body.getRadius();

	// Camera position and orientation parameters in object reference frame.
	prm.obj.cpos   = prm.obj.opos - prm.cpos;
	prm.obj.cdir   = prm.obj.orot * vec4f_t(prm.obj.cpos, 1.0f);
	prm.obj.cdist  = glm::length(prm.obj.cdir) / prm.obj.orad;
	prm.obj.viewap = (prm.obj.cdist >= 1.0f) ? acos(1.0f / prm.obj.cdist) : 0.0f;
	prm.obj.cdir   = glm::normalize(prm.obj.cdir);

//	cout << fmt::sprintf("Terrain Manager - Render Parameter\n");
//	cout << fmt::sprintf("Planet Radius:      %lf\n", prm.obj.orad);
//	cout << fmt::sprintf("Planet Position:    (%lf,%lf,%lf)\n",
//		prm.obj.opos.x, prm.obj.opos.y, prm.obj.opos.z);
//	cout << fmt::sprintf("Planet Orientation: (%lf,%lf,%lf,%lf)\n",
//		prm.obj.oqrot.w, prm.obj.oqrot.x, prm.obj.oqrot.y, prm.obj.oqrot.z);
//	cout << fmt::sprintf("Camera Position:    (%lf,%lf,%lf)\n",
//		prm.obj.cpos.x, prm.obj.cpos.y, prm.obj.cpos.z);
//	cout << fmt::sprintf("Camera Direction:   (%lf,%lf,%lf)\n",
//		prm.obj.cdir.x, prm.obj.cdir.y, prm.obj.cdir.z);
//	cout << fmt::sprintf("Camera Distance:    %lf\n", prm.obj.cdist);
//	cout << fmt::sprintf("Horizon View:       %lf\n", toDegree(prm.obj.viewap));
//	cout << fmt::sprintf("Camera Position:    (%lf,%lf,%lf) in Universe frame\n",
//		prm.cpos.x, prm.cpos.y, prm.cpos.z);

//    obj->getCoordinates(prm.cpos, &lat, &lng);
//    cout << "Planet Position:  (" << toDegrees(lat) << "," << toDegrees(lng) << ")" << endl;

	// Normalize units in object radius
	// prm.obj.cdist /= prm.obj.orad;
	// prm.obj.cdir   = glm::normalize(prm.obj.cdir);
	
	vec3f_t gCamera, eCamera;

	convertDoubleToTwoFloats(-prm.obj.cpos, gCamera, eCamera);

//	prm.dmModel = glm::translate(glm::transpose(prm.obj.orot), prm.obj.cpos);
	prm.dmModel = glm::transpose(prm.obj.orot);
	prm.dmView = glm::translate(prm.dmView, prm.obj.cpos);

//	cout << "MV:      " << setw(15) << fixed << prm.dmWorld[0][0] << "," << prm.dmWorld[1][0] << "," << prm.dmWorld[2][0] << "," << prm.dmWorld[3][0] << endl;
//	cout << "         " << setw(15) << fixed << prm.dmWorld[0][1] << "," << prm.dmWorld[1][1] << "," << prm.dmWorld[2][1] << "," << prm.dmWorld[3][1] << endl;
//	cout << "         " << setw(15) << fixed << prm.dmWorld[0][2] << "," << prm.dmWorld[1][2] << "," << prm.dmWorld[2][2] << "," << prm.dmWorld[3][2] << endl;
//	cout << "         " << setw(15) << fixed << prm.dmWorld[0][3] << "," << prm.dmWorld[1][3] << "," << prm.dmWorld[2][3] << "," << prm.dmWorld[3][3] << endl;
//
//	cout << "P:       " << setw(15) << fixed << prm.dmProj[0][0] << "," << prm.dmProj[1][0] << "," << prm.dmProj[2][0] << "," << prm.dmProj[3][0] << endl;
//	cout << "         " << setw(15) << fixed << prm.dmProj[0][1] << "," << prm.dmProj[1][1] << "," << prm.dmProj[2][1] << "," << prm.dmProj[3][1] << endl;
//	cout << "         " << setw(15) << fixed << prm.dmProj[0][2] << "," << prm.dmProj[1][2] << "," << prm.dmProj[2][2] << "," << prm.dmProj[3][2] << endl;
//	cout << "         " << setw(15) << fixed << prm.dmProj[0][3] << "," << prm.dmProj[1][3] << "," << prm.dmProj[2][3] << "," << prm.dmProj[3][3] << endl;

//	mat4d_t smWorld = prm.dmWorld;

	prm.dmModel[3].x = 0;
	prm.dmModel[3].y = 0;
	prm.dmModel[3].z = 0;

//	cout << "MV2:     " << setw(15) << fixed << prm.dmWorld[0][0] << "," << prm.dmWorld[1][0] << "," << prm.dmWorld[2][0] << "," << prm.dmWorld[3][0] << endl;
//	cout << "         " << setw(15) << fixed << prm.dmWorld[0][1] << "," << prm.dmWorld[1][1] << "," << prm.dmWorld[2][1] << "," << prm.dmWorld[3][1] << endl;
//	cout << "         " << setw(15) << fixed << prm.dmWorld[0][2] << "," << prm.dmWorld[1][2] << "," << prm.dmWorld[2][2] << "," << prm.dmWorld[3][2] << endl;
//	cout << "         " << setw(15) << fixed << prm.dmWorld[0][3] << "," << prm.dmWorld[1][3] << "," << prm.dmWorld[2][3] << "," << prm.dmWorld[3][3] << endl;

	prm.mvp = mat4f_t(prm.dmProj * prm.dmView * prm.dmModel);
//	mat4f_t smvp = mat4f_t(prm.dmProj * smWorld);

	prm.mPView = mat4f_t(prm.dmPView);
	prm.mWorld = mat4f_t(prm.dmWorld);

//	cout << "MVP:     " << setw(15) << fixed << prm.mvp[0][0] << "," << prm.mvp[1][0] << "," << prm.mvp[2][0] << "," << prm.mvp[3][0] << endl;
//	cout << "         " << setw(15) << fixed << prm.mvp[0][1] << "," << prm.mvp[1][1] << "," << prm.mvp[2][1] << "," << prm.mvp[3][1] << endl;
//	cout << "         " << setw(15) << fixed << prm.mvp[0][2] << "," << prm.mvp[1][2] << "," << prm.mvp[2][2] << "," << prm.mvp[3][2] << endl;
//	cout << "         " << setw(15) << fixed << prm.mvp[0][3] << "," << prm.mvp[1][3] << "," << prm.mvp[2][3] << "," << prm.mvp[3][3] << endl;
//
//	cout << "cMV:     " << setw(15) << fixed << smWorld[0][0] << "," << smWorld[1][0] << "," << smWorld[2][0] << "," << smWorld[3][0] << endl;
//	cout << "         " << setw(15) << fixed << smWorld[0][1] << "," << smWorld[1][1] << "," << smWorld[2][1] << "," << smWorld[3][1] << endl;
//	cout << "         " << setw(15) << fixed << smWorld[0][2] << "," << smWorld[1][2] << "," << smWorld[2][2] << "," << smWorld[3][2] << endl;
//	cout << "         " << setw(15) << fixed << smWorld[0][3] << "," << smWorld[1][3] << "," << smWorld[2][3] << "," << smWorld[3][3] << endl;
//
//	cout << "cMVP:    " << setw(15) << fixed << smvp[0][0] << "," << smvp[1][0] << "," << smvp[2][0] << "," << smvp[3][0] << endl;
//	cout << "         " << setw(15) << fixed << smvp[0][1] << "," << smvp[1][1] << "," << smvp[2][1] << "," << smvp[3][1] << endl;
//	cout << "         " << setw(15) << fixed << smvp[0][2] << "," << smvp[1][2] << "," << smvp[2][2] << "," << smvp[3][2] << endl;
//	cout << "         " << setw(15) << fixed << smvp[0][3] << "," << smvp[1][3] << "," << smvp[2][3] << "," << smvp[3][3] << endl;
//
//	cout << "Camera: " << gCamera.x << "," << gCamera.y << "," << gCamera.z << endl;
//	cout << " Error: " << eCamera.x << "," << eCamera.y << "," << eCamera.z << endl;

//	{
//		vec3f_t vpos = vec3f_t(-312.516001, -156.352167, 6361.409337);
//		vec3f_t epos = vec3f_t(-0.00010, 0.000006, 0.000119);
//
//	    vec3f_t t1 = epos - eCamera;
//	    vec3f_t e = t1 - epos;
//	    vec3f_t t2 = ((-eCamera - e) + (epos - (t1 - e))) +
//	        vpos - gCamera;
//	    vec3f_t hdiff = t1 + t2;
//	    vec3f_t ldiff = t2 - (hdiff - t1);
//	    vec3f_t pos = hdiff + ldiff;
//
//		cout << "Vertex: " << vpos.x << "," << vpos.y << "," << vpos.z << endl;
//		cout << "Result: " << pos.x << "," << pos.y << "," << pos.z << endl;
//
//		vec3f_t rpos = prm.mvp * vec4f_t(pos, 1.0f);
//		cout << " w/MVP: " << rpos.x << "," << rpos.y << "," << rpos.z << endl;
//
//		vec3f_t cpos = smvp * vec4f_t(vpos, 1.0f);
//		cout << " c/MVP: " << cpos.x << "," << cpos.y << "," << cpos.z << endl;
//	}

	uint32_t mvpLoc = glGetUniformLocation(pgm->getID(), "mvp");
    glUniformMatrix4fv(mvpLoc, 1, GL_FALSE, glm::value_ptr(prm.mvp));

//	uint32_t chLoc = glGetUniformLocation(pgm->getID(), "gCamera");
//    glUniform3fv(chLoc, 1, glm::value_ptr(gCamera));
//	uint32_t clLoc = glGetUniformLocation(pgm->getID(), "eCamera");
//    glUniform3fv(clLoc, 1, glm::value_ptr(eCamera));

	// Rendering terrain area
	for (int idx = 0; idx < 2; idx++)
		process(terrain[idx], prm);
	for (int idx = 0; idx < 2; idx++)
		render(terrain[idx], prm);

	glUseProgram(0);
}

Mesh *TerrainManager::createSphere(int lod, int ilat, int ilng, int grids, tcrd_t &tcr)
{
	int nlat = 1 << lod;
	int nlng = 2 << lod;

	double mlat0 = PI * double(ilat) / double(nlat);
	double mlat1 = PI * double(ilat+1) / double(nlat);
    // float mlng0 = PI*2 * (float(nlng/2 - ilng-1) / float(nlng)) - PI;
    // float mlng1 = PI*2 * (float(nlng/2 - ilng) / float(nlng)) - PI;
    double mlng0 = PI*2 * (double(ilng) / double(nlng)) - PI;
    double mlng1 = PI*2 * (double(ilng+1) / double(nlng)) - PI;

    // cout << "ilng " << ilng << " nlng " << nlng << " mlng0 " << mlng0 << " mlng1 " << mlng1 << 
    //     " ( " << toDegree(mlng0) << " , " << toDegree(mlng1) << " )" << endl;

	double   rad  = body.getRadius();
	double   erad = rad;

    double slng, clng;
    double slat, clat;
	double lng, lat;
    double tu, tv, du, dv;
    double tur, tvr;
    vec3d_t pos, nml;

    int      vidx;
    int      nVertices;
    int      nIndices, nIndices1;

    vtxf_t   *vtx;
    uint16_t *idx, *pidx;

    nVertices = (grids+1)*(grids+1);
    nIndices  = 6 * (grids*grids);
    vtx       = new vtxf_t[nVertices];
    idx       = new uint16_t[nIndices];

//    int      nTexCoords;
//    double *vtxs, *nmls;
//	double *pvtx, *pnml;
//    double   *tc, *ptc;
//    nVertices   = 3 * ((grids+1)*(grids+1));
//    nTexCoords  = 2 * ((grids+1)*(grids+1));
//    vtxs  = new double[nVertices];
//    nmls  = new double[nVertices];
//    tc    = new double[nTexCoords];
//    pvtx  = vtxs;
//    pnml  = nmls;
//    ptc   = tc;

//	std::cout << std::fixed << std::setprecision(10);
//	std::cout << "----------------------" << std::endl;
//	std::cout << "LOD:  " << lod << " nLAT: " << nlat << " nLNG: " << nlng
//			  << " Grids: " << grids << std::endl;
//	std::cout << "iLAT: " << ilat << " iLNG: " << ilng << std::endl;
//	std::cout << "Latitude Range:  " << toDegrees(mlat0) << " to " << toDegrees(mlat1) << std::endl;
//	std::cout << "Longitude Range: " << toDegrees(mlng0) << " to " << toDegrees(mlng1) << std::endl;

//	std::cout << "Delta Angle: " << dang << " U:" << du << std::endl;
//	std::cout << "Alpha: " << alpha << std::endl;

    du  = (mlng1 - mlng0) / grids;
    dv  = (mlat1 - mlat0) / grids;
    tur = tcr.tumax - tcr.tumin;
    tvr = tcr.tvmax - tcr.tvmin;

    vidx = 0;
	for (int y = 0; y <= grids; y++)
	{
		lat  = mlat0 + (mlat1-mlat0) * (double(y)/double(grids));
		slat = sin(lat); clat = cos(lat);
        tv = tcr.tvmin + tvr * (double(y)/double(grids));

//        std::cout << "Y = " << y << " LAT: " << toDegrees(lat) << std::endl;

		for (int x = 0; x <= grids; x++)
		{
			lng  = mlng0 + (mlng1-mlng0) * (double(x)/double(grids));
			slng = sin(lng); clng = cos(lng);
            tu   = tcr.tumin + tur * (double(x)/double(grids));

//            std::cout << "X = " << x << " LNG: " << toDegrees(lng) << std::endl;

//            pos = vec3d_t(axes.x()*slat*clng, axes.y()*clat, axes.z()*slat*slng);
//            nml = pos.normalized();

//            erad = rad + elevGlobe;
//            if (elev != nullptr) {
//            	int16_t edata = elev[(y+1)*ELEV_STRIDE + (x+1)];
//            	erad += (double(edata) * elevScale) / 1000.0;
//            	if (edata != 0)
//            		std::cout << "Elev X: " << x << " Y: " << y
//						      << "Elev: " << edata << std::endl;
//            }
//            else
//            	std::cout << "No elevation data for sphere..." << std::endl;

            nml = vec3d_t(slat*clng, clat, slat*-slng);

            pos = nml * erad;

			// Convert to 32-bit floats for vertices buffer/rendering
            vtx[vidx].vx = float(pos.x);
            vtx[vidx].vy = float(pos.y);
            vtx[vidx].vz = float(pos.z);

       		vtx[vidx].ex = float(pos.x - vtx[vidx].vx);
            vtx[vidx].ey = float(pos.y - vtx[vidx].vy);
            vtx[vidx].ez = float(pos.z - vtx[vidx].vz);

            vtx[vidx].nx = float(nml.x);
            vtx[vidx].ny = float(nml.y);
            vtx[vidx].nz = float(nml.z);

            vtx[vidx].tu = float(tu);
            vtx[vidx].tv = float(tv);

//            if (vidx == 0) {
//            	cout << "Value:   " << setw(15) << fixed << pos.x << "," << pos.y << "," << pos.z << endl;
//            	cout << " Vertex: " << setw(15) << fixed << vtx[vidx].vx << "," << vtx[vidx].vy << "," << vtx[vidx].vz << endl;
//            	cout << " Error:  " << setw(15) << fixed << vtx[vidx].ex << "," << vtx[vidx].ey << "," << vtx[vidx].ez << endl;
//            }

            vidx++;

//			std::cout << "(" << x << "," << y << "): " << lng0 << " " << lat0 << " --> ";
//			std::cout << "(" << nml0.x() << "," << nml0.y() << "," << nml0.z() << ")" << std::endl;
		}
//		std::cout << std::endl;
//		std::cout << "Actual: " << pvtx - vtxs << " Total: " << nvtx << std::endl;
//		std::cout << "Estimate: " << (grids+1)*2*(grids-1)+2 << std::endl;

		// Degenerate triangles to connect
//		pvtx[0] = pvtx[3] = pvtx[-3];
//		pvtx[1] = pvtx[4] = pvtx[-2];
//		pvtx[2] = pvtx[5] = pvtx[-1];
//
//		pnml[0] = pnml[3] = pnml[-3];
//		pnml[1] = pnml[4] = pnml[-2];
//		pnml[2] = pnml[5] = pnml[-1];

	}
//	std::cout << "Vertices: Actual: " << vidx << " Expect: " << nVertices1 << std::endl;

    nIndices1 = 0;
    pidx = idx;
    for (int y = 0; y < grids; y++) {
        for (int x = 0; x < grids; x++) {
            *pidx++ = (y+0)*(grids+1) + (x+0);
            *pidx++ = (y+1)*(grids+1) + (x+0);
            *pidx++ = (y+0)*(grids+1) + (x+1);

            *pidx++ = (y+1)*(grids+1) + (x+0);
            *pidx++ = (y+1)*(grids+1) + (x+1);
            *pidx++ = (y+0)*(grids+1) + (x+1);

//            std::cout << "Index: (" << pidx[0] << "," << pidx[1] << "," << pidx[2] << ")" << std::endl;
//            std::cout << "Index: (" << pidx[3] << "," << pidx[4] << "," << pidx[5] << ")" << std::endl;
//
//            pidx += 6;
            nIndices1 += 6;
        }
    }

    return Mesh::create(vidx, vtx, nIndices, idx);
}
