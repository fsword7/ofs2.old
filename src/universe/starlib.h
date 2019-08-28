/*
 * starlib.h - Star catalogue library package
 *
 *  Created on: August 26, 2019
 *      Author: Tim Stark
 */

namespace ofs::universe {

    class StarCatalogue
    {
    public:
        StarCatalogue() = default;
        ~StarCatalogue() = default;

        bool loadHYGData(const string &fname);
        bool loadXHIPData(const string &fname);
        
    private:
    };
    
}
