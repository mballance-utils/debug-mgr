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
#include <alloca.h>
#include <stdio.h>
#include <string.h>
#include "dmgr/IDebug.h"
#include "DebugOutFile.h"
#ifndef _WIN32
#include <unistd.h>
#include <execinfo.h>
#endif


namespace dmgr {

DebugOutFile::DebugOutFile(const std::string &name) :
    m_path(name), m_fp(0), m_close_fp(true) {

    m_fp = fopen(name.c_str(), "w");
}

DebugOutFile::DebugOutFile(FILE *fp, bool close_fp) : 
    m_fp(fp), m_close_fp(close_fp), m_flush(false) {

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
    if (m_flush) {
        fflush(m_fp);
    }
}

void DebugOutFile::leave(IDebug *dbg, const char *fmt, va_list ap) {
	fprintf(m_fp, "<-- %s::", dbg->name().c_str());
	vfprintf(m_fp, fmt, ap);
	fputs("\n", m_fp);
    if (m_flush) {
        fflush(m_fp);
    }
}

void DebugOutFile::debug(IDebug *dbg, const char *fmt, va_list ap) {
	fprintf(m_fp, "%s: ", dbg->name().c_str());
	vfprintf(m_fp, fmt, ap);
	fputs("\n", m_fp);
    if (m_flush) {
        fflush(m_fp);
    }
}

void DebugOutFile::error(IDebug *dbg, const char *fmt, va_list ap) {
    fprintf(m_fp, "Error: %s: ", dbg->name().c_str());
    vfprintf(m_fp, fmt, ap);
    fputs("\n", m_fp);
    fflush(m_fp);
}

void DebugOutFile::fatal(IDebug *dbg, const char *fmt, va_list ap) {
    fprintf(m_fp, "Fatal: %s: ", dbg->name().c_str());
    vfprintf(m_fp, fmt, ap);
    fputs("\n", m_fp);
    fflush(m_fp);
}

void DebugOutFile::crashClose() {
    fflush(m_fp);
#ifndef _WIN32
    if (m_path.size()) {
        static void *bt_buffer[64];
        char *rename_path = (char *)alloca(m_path.size()+64);
        int32_t bt_size;

        bt_size = backtrace(bt_buffer, sizeof(bt_buffer)/sizeof(void *));

        backtrace_symbols_fd(bt_buffer, bt_size, fileno(m_fp));

        // Rename the file
        fclose(m_fp);

        sprintf(rename_path, "%s.%d", m_path.c_str(), getpid());
        ::rename(m_path.c_str(), rename_path);
    }
#endif
}

void DebugOutFile::flush() {
    fflush(m_fp);
}

}
