/**
 * DebugOutFile.h
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
#pragma once
#include <stdio.h>
#include "dmgr/IDebugOut.h"

namespace dmgr {



class DebugOutFile : public virtual IDebugOut {
public:
    DebugOutFile(const std::string &name);

    DebugOutFile(FILE *fp, bool close_fp);

    virtual ~DebugOutFile();
    
	virtual void enter(IDebug *dbg, const char *fmt, va_list ap) override;
	virtual void leave(IDebug *dbg, const char *fmt, va_list ap) override;
	virtual void debug(IDebug *dbg, const char *fmt, va_list ap) override;
	virtual void error(IDebug *dbg, const char *fmt, va_list ap) override;
	virtual void fatal(IDebug *dbg, const char *fmt, va_list ap) override;

    virtual void crashClose() override;

    virtual void flush() override;

private:
    std::string         m_path;
    FILE                *m_fp;
    bool                m_close_fp;
    bool                m_flush;


};

}


