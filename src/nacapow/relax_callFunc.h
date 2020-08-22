// Copyright 2020 XGSTeam, NaCaTeam
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef RELAX_NACA_H
#define RELAX_NACA_H

#include "f2c.h"

extern "C" {


int32_t naca_(integer *ntime, integer *idum, logical *runcg, 
  doublereal *xzuixi, doublereal *ezuixi, integer *iezuixi, logical *
  itrace);
}

#endif // RELAX_NACA_H
