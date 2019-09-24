/*
 * starlib.h - Star catalogue library package
 *
 *  Created on: August 26, 2019
 *      Author: Tim Stark
 */

#include "universe/star.h"
#include "universe/startree.h"


namespace ofs::universe {

#include "universe/handler.h"

	class StarCatalogue
    {
    public:
        StarCatalogue() = default;
        ~StarCatalogue() = default;

        bool loadHYGData(const string &fname);
        bool loadXHIPData(const string &fname);

        CelestialStar *find(const string& name) const;

        void findVisibleStars(const ofsHandler& handle, const vec3d_t& obs,
        		const quatd_t &rot, double fov, double aspect, double limitMag) const;
        void findNearStars(const vec3d_t& obs, double radius,
        	vector<const CelestialStar *>& stars) const;

    protected:
        void initOctreeData(vector<CelestialStar*> stars);
        void finish();
        
    private:
        vector<CelestialStar *> uStars;

    	StarTree *starTree;
    };
    
}
