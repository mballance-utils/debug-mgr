/**
 * DebugOutList.h
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
#include "dmgr/IDebugOutList.h"

namespace dmgr {

class DebugOutList : public virtual IDebugOutList {
public:
    DebugOutList();

    virtual ~DebugOutList();

    virtual void addOutput(IDebugOut *out) override {
        m_outputs.push_back(IDebugOutUP(out));
    }

    virtual const std::vector<IDebugOutUP> &getOutputs() const {
        return m_outputs;
    }

	virtual void enter(IDebug *dbg, const char *fmt, va_list ap) override;
	virtual void leave(IDebug *dbg, const char *fmt, va_list ap) override;
	virtual void debug(IDebug *dbg, const char *fmt, va_list ap) override;
	virtual void error(IDebug *dbg, const char *fmt, va_list ap) override;
	virtual void fatal(IDebug *dbg, const char *fmt, va_list ap) override;

    virtual void crashClose() override;

    virtual void flush() override;

private:
    std::vector<IDebugOutUP>            m_outputs;

};

}


