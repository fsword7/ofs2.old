/*
 * precession.h - Earth Precession Model
 *
 *  Created on: Oct 18, 2019
 *      Author: Tim Stark
 */

#pragma once

#define P03LP_PRECESSION_CENTURIES 5000.0

void computeEclipticPrecission(double T, double &pa, double &qa);
void computePrecissionObliquity(double T, double &pa, double &ea);
