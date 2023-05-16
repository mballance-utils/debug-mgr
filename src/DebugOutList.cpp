/*
 * DebugOutList.cpp
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
#include "DebugOutList.h"


namespace dmgr {


DebugOutList::DebugOutList() {

}

DebugOutList::~DebugOutList() {

}

void DebugOutList::enter(IDebug *dbg, const char *fmt, va_list ap) {
    for (std::vector<IDebugOutUP>::const_iterator
        it=m_outputs.begin();
        it!=m_outputs.end(); it++) {
        (*it)->enter(dbg, fmt, ap);
    }
}

void DebugOutList::leave(IDebug *dbg, const char *fmt, va_list ap) {
    for (std::vector<IDebugOutUP>::const_iterator
        it=m_outputs.begin();
        it!=m_outputs.end(); it++) {
        (*it)->leave(dbg, fmt, ap);
    }
}

void DebugOutList::debug(IDebug *dbg, const char *fmt, va_list ap) {
    for (std::vector<IDebugOutUP>::const_iterator
        it=m_outputs.begin();
        it!=m_outputs.end(); it++) {
        (*it)->debug(dbg, fmt, ap);
    }
}

void DebugOutList::fatal(IDebug *dbg, const char *fmt, va_list ap) {
    for (std::vector<IDebugOutUP>::const_iterator
        it=m_outputs.begin();
        it!=m_outputs.end(); it++) {
        (*it)->fatal(dbg, fmt, ap);
    }
}

void DebugOutList::flush() {
    for (std::vector<IDebugOutUP>::const_iterator
        it=m_outputs.begin();
        it!=m_outputs.end(); it++) {
        (*it)->flush();
    }
}

}
