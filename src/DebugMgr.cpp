/*
 * DebugMgr.cpp
 *
 *  Created on: Mar 16, 2022
 *      Author: mballance
 */
#include <stdexcept>
#include <stdlib.h>
#ifndef _WIN32
#include <signal.h>
#include <unistd.h>
#endif
#include "DebugMgr.h"
#include "DebugOutFile.h"
#include "Debug.h"
#include "Factory.h"

namespace dmgr {

DebugMgr::DebugMgr() : 
    m_signal_handlers_registered(false),
    m_out(new DebugOutFile(stdout, false)) {
	m_en = false;

    m_dbg = findDebug("dmgr");

    const char *level = getenv("DEBUG_MGR_EN");
    if (level && level[0]) {
        m_en = strtoul(level, 0, 0);
    }
}

DebugMgr::~DebugMgr() {
	// TODO Auto-generated destructor stub
}

void DebugMgr::enable(bool en) {
	m_en = en;

	for (std::unordered_map<std::string,IDebug*>::const_iterator
		it=m_debug_ep_m.begin();
		it!=m_debug_ep_m.end(); it++) {
		if (en != it->second->en()) {
			fprintf(stdout, "DebugMgr: %s %s\n", (en)?"Enable":"Disable", it->second->name().c_str());
		}
		it->second->set_en(m_en);
	}
}

void DebugMgr::registerSignalHandlers() {
    if (!m_signal_handlers_registered) {
        m_signal_handlers_registered = true;
#ifndef _WIN32
        signal(SIGSEGV, &DebugMgr::signal_handler);
        signal(SIGBUS, &DebugMgr::signal_handler);
#endif
    }
}

void DebugMgr::addDebug(IDebug *dbg) {
	m_debug_ep_m.insert({dbg->name(), dbg});
	dbg->set_en(m_en);
}

IDebug *DebugMgr::findDebug(const std::string &name) {
	std::unordered_map<std::string,IDebug*>::const_iterator it;

	it = m_debug_ep_m.find(name);

	if (it != m_debug_ep_m.end()) {
		return it->second;
	} else {
		Debug *dbg = new Debug(this, name);
        addDebug(dbg);
		return dbg;
	}
}

void DebugMgr::setFlags(
			const std::unordered_map<std::string, int32_t> &flags) {

}

void DebugMgr::enter(IDebug *dbg, const char *fmt, va_list ap) {
    m_out->enter(dbg, fmt, ap);
}

void DebugMgr::leave(IDebug *dbg, const char *fmt, va_list ap) {
    m_out->leave(dbg, fmt, ap);
}

void DebugMgr::debug(IDebug *dbg, const char *fmt, va_list ap) {
    m_out->debug(dbg, fmt, ap);
}

void DebugMgr::error(IDebug *dbg, const char *fmt, va_list ap) {
    m_out->error(dbg, fmt, ap);
    m_out->flush();
}

void DebugMgr::fatal(IDebug *dbg, const char *fmt, va_list ap) {
    m_out->fatal(dbg, fmt, ap);
    throw std::runtime_error("");
}

void DebugMgr::flush() {
    m_out->flush();
}

void DebugMgr::signal_handler(int sigid) {
    dynamic_cast<DebugMgr *>(Factory::inst()->getDebugMgr())->crash_handler();
}

void DebugMgr::crash_handler() {
    m_dbg->error("Application Crashed");
    flush();
    _exit(1);
}


} /* namespace dmgr */
