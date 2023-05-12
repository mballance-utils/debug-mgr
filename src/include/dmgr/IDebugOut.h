/**
 * IDebugOut.h
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
#include <memory>

namespace dmgr {

class IDebug;

class IDebugOut;
using IDebugOutUP=std::unique_ptr<IDebugOut>;
class IDebugOut {
public:

    virtual ~IDebugOut() { }

	virtual void enter(IDebug *dbg, const char *fmt, va_list ap) = 0;
	virtual void leave(IDebug *dbg, const char *fmt, va_list ap) = 0;
	virtual void debug(IDebug *dbg, const char *fmt, va_list ap) = 0;
	virtual void fatal(IDebug *dbg, const char *fmt, va_list ap) = 0;

    virtual void flush() = 0;

};

} /* namespace dmgr */


