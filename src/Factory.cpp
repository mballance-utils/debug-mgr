/*
 * Factory.cpp
 *
 * Copyright 2022 Matthew Ballance and Contributors
 *
 * Licensed under the Apache License, Version 2.0 (the "License"); you may 
 * not use this file except in compliance with the License.  
 * You may obtain a copy of the License at:
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software 
 * distributed under the License is distributed on an "AS IS" BASIS, 
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.  
 * See the License for the specific language governing permissions and 
 * limitations under the License.
 *
 * Created on:
 *     Author:
 */
#include <stdio.h>
#include "dmgr/FactoryExt.h"
#include "DebugOutFile.h"
#include "DebugOutList.h"
#include "Factory.h"


namespace dmgr {


Factory::Factory() : m_dbg_mgr(new DebugMgr()) {

}

Factory::~Factory() {

}

IDebugMgr *Factory::getDebugMgr() {
    return m_dbg_mgr.get();
}

IDebugOut *Factory::mkDebugOutFile(FILE *fp, bool close_fp) {
    return new DebugOutFile(fp, close_fp);
}

IDebugOut *Factory::mkDebugOutPath(const std::string &name) {
    return new DebugOutFile(name);
}

IDebugOutList *Factory::mkDebugOutList() {
    return new DebugOutList();
}

IFactory *Factory::inst() {
    if (!m_inst) {
        m_inst = new Factory();
    }
    return m_inst;
}

Factory *Factory::m_inst = 0;

extern "C" IFactory *debug_mgr_getFactory() {
//    fprintf(stdout, "debug_mgr_getFactory() %p\n",
//        Factory::inst());
//    fflush(stdout);
    return Factory::inst();
}

}

dmgr::IFactory *dmgr_getFactory() {
    return dmgr::Factory::inst();
}
