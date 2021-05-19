//
//  UnitTests.h
//  texsyn
//
//  Created by Craig Reynolds on 12/16/19.
//  Copyright © 2019 Craig Reynolds. All rights reserved.
//

#pragma once

namespace UnitTests
{
    // TODO very informal so far. This function should return true:
    bool allTestsOK();
    // Verbosity 0: no printing unless error occurs.
    //           1: print one line, unless error occurs.
    //           2: print report for each test like previous behavior.
    bool allTestsOK(int verbosity);
    // Ad hoc utility to verify all texture types build and run.
    void instantiateAllTextureTypes();
}
