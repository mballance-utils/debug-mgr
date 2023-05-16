/*
 * DebugOutFile.cpp
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
#include "dmgr/IDebug.h"
#include "DebugOutFile.h"


namespace dmgr {


DebugOutFile::DebugOutFile(FILE *fp, bool close_fp) : m_fp(fp), m_close_fp(close_fp) {

}

DebugOutFile::~DebugOutFile() {
    if (m_close_fp) {
        ::fclose(m_fp);
    }

}

void DebugOutFile::enter(IDebug *dbg, const char *fmt, va_list ap) {
	fprintf(m_fp, "--> %s::", dbg->name().c_str());
	vfprintf(m_fp, fmt, ap);
	fputs("\n", m_fp);
}

void DebugOutFile::leave(IDebug *dbg, const char *fmt, va_list ap) {
	fprintf(m_fp, "<-- %s::", dbg->name().c_str());
	vfprintf(m_fp, fmt, ap);
	fputs("\n", m_fp);
}

void DebugOutFile::debug(IDebug *dbg, const char *fmt, va_list ap) {
	fprintf(m_fp, "%s: ", dbg->name().c_str());
	vfprintf(m_fp, fmt, ap);
	fputs("\n", m_fp);
}

void DebugOutFile::fatal(IDebug *dbg, const char *fmt, va_list ap) {
    fprintf(m_fp, "Fatal: %s: ", dbg->name().c_str());
    vfprintf(m_fp, fmt, ap);
    fputs("\n", m_fp);
    fflush(m_fp);
}

void DebugOutFile::flush() {
    fflush(m_fp);
}

}
