/*
 * starlib.h - Star catalogue library package
 *
 *  Created on: August 26, 2019
 *      Author: Tim Stark
 */

#include "universe/star.h"
#include "universe/startree.h"

namespace ofs::universe {

    class StarCatalogue
    {
    public:
        StarCatalogue() = default;
        ~StarCatalogue() = default;

        bool loadHYGData(const string &fname);
        bool loadXHIPData(const string &fname);

    protected:
        void initOctreeData(std::vector<CelestialStar*> stars);
        void finish();
        
    private:
        vector<CelestialStar *> uStars;

    	StarTree *starTree;
    };
    
}
