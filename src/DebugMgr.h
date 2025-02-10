/*
 * DebugMgr.h
 *
 *  Created on: Mar 16, 2022
 *      Author: mballance
 */

#pragma once
#include <stdint.h>
#include <stdarg.h>
#include <unordered_map>
#include <memory>
#include <string>
#include <vector>
#include "dmgr/IDebug.h"
#include "dmgr/IDebugMgr.h"
#include "dmgr/IDebugOut.h"

namespace dmgr {

class Debug;

class DebugMgr;
using DebugMgrUP=std::unique_ptr<DebugMgr>;
class DebugMgr : public IDebugMgr {
public:
	DebugMgr();

	virtual ~DebugMgr();

	virtual void enable(bool en) override;

    virtual void registerSignalHandlers() override;

	virtual void addDebug(IDebug *dbg) override;

	virtual IDebug *findDebug(const char *name) override;

	virtual void setFlags(
			const std::unordered_map<std::string, int32_t> &flags) override;

    virtual void setDebugOut(IDebugOut *out) override {
        m_out = IDebugOutUP(out);
    }

    virtual IDebugOut *getDebugOut() override {
        return m_out.get();
    }

	virtual void enter(IDebug *dbg, const char *fmt, va_list ap) override;

	virtual void leave(IDebug *dbg, const char *fmt, va_list ap) override;

	virtual void debug(IDebug *dbg, const char *fmt, va_list ap) override;

	virtual void error(IDebug *dbg, const char *fmt, va_list ap) override;

	virtual void fatal(IDebug *dbg, const char *fmt, va_list ap) override;

    virtual void crashClose() override;

    virtual void flush() override;

private:
    static void signal_handler(int sigid);

    void crash_handler();

private:
	bool											m_en;
    bool                                            m_signal_handlers_registered;
    IDebug                                          *m_dbg;
    IDebugOutUP                                     m_out;
	std::unordered_map<std::string, int32_t>		m_debug_en_m;
	std::unordered_map<std::string, IDebug *>		m_debug_ep_m;

};

} /* namespace dmgr */
