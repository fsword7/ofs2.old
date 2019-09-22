/*
 * elevation.h - Orbiter elevation data package
 *
 *  Created on: Sep 22, 2019
 *      Author: Tim Stark
 */

#pragma once

#define TILE_RES    256

// Elevation tile resolution
#define ELEV_RES     TILE_RES
#define ELEV_XRES    (TILE_RES+3)
#define ELEV_YRES    (TILE_RES+3)
#define ELEV_STRIDE  ELEV_XRES
#define ELEV_LENGTH  (ELEV_XRES*ELEV_YRES)

// Elevation file header
// Format type:
//    0 = flat (no data)
//    8 = unsigned byte (8-bit)
//   -8 = signed byte (8-bit)
//   16 = unsigned short (16-bit)
//  -16 = signed short (16-bit)

#pragma pack(push, 1)
struct elevHeader
{
    uint32_t code;              // Code 'ELE1' in FourCC format
    int      hdrSize;           // Header length (expected 76 bytes)
    int      format;            // Data type format
    int      xgrd, ygrd;        // (X,Y) grids   (expected 259x259)
    int      xpad, ypad;        // (X,Y) pads    (expected 1x1)
    double   scale;             // Elevation scale
    double   offset;            // Elevation offset
    double   latmin, latmax;    // Latitude range [rad]
    double   lngmin, lngmax;    // Longtitude range [rad]
    double   emin, emax, emean; // Min, max, and mean elevation [m]
};
#pragma pack(pop)

