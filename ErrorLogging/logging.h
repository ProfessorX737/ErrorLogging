#pragma once 
#include <string>
#include <sstream>

enum Severity {
	INFO = 0,
	WARNING = 1,
	PROBLEM = 2,
	FATAL = 3,
	NUM_SEVERITIES = 4
};

static const char* SeverityNames[NUM_SEVERITIES] = { "INFO","WARNING","PROBLEM","FATAL" };

class LogMessage : public std::ostringstream {
public:
	LogMessage(Severity severity, const char* fileName, const char* funcName, int line);
	~LogMessage();
protected:
	void printLogMessage();
private:
	Severity severity_;
	const char* fileName_;
	const char* funcName_;
	int line_;
};

class LogMessageFatal : public LogMessage {
public:
	LogMessageFatal(const char* fileName, const char* funcName, int line);
	~LogMessageFatal();
};

#define LOG_INFO LogMessage(INFO,__FILE__,__FUNCTION__,__LINE__).flush()
#define LOG_WARNING LogMessage(WARNING,__FILE__,__FUNCTION__,__LINE__).flush()
#define LOG_PROBLEM LogMessage(PROBLEM,__FILE__,__FUNCTION__,__LINE__).flush()
#define LOG_FATAL LogMessageFatal(__FILE__,__FUNCTION__,__LINE__).flush()
#define LOG(severity) LOG_##severity

#define CHECK(expr) \
	if(!expr) LogMessageFatal(__FILE__,__FUNCTION__,__LINE__).flush() << "Check failed: " << #expr << " "

#define CHECK_OP(val1,op,val2) \
	if(!(val1 op val2)) LogMessageFatal(__FILE__,__FUNCTION__,__LINE__).flush() << "Check failed: " << #val1 << " " << #op << " " << #val2 << " "

#define CHECK_EQ(val1,val2) CHECK_OP(val1,==,val2)
#define CHECK_NE(val1,val2) CHECK_OP(val1,!=,val2)
#define CHECK_LE(val1,val2) CHECK_OP(val1,<=,val2)
#define CHECK_LT(val1,val2) CHECK_OP(val1,<,val2)
#define CHECK_GE(val1,val2) CHECK_OP(val1,>=,val2)
#define CHECK_GT(val1,val2) CHECK_OP(val1,>,val2)
#define CHECK_NOT_NULL(val) CHECK_NE(val,nullptr)
#define CHECK_POW2(val) \
	CHECK((val != 0) && ((val & (val - 1)) == 0))

#ifndef NDEBUG

#define DCHECK(expr) CHECK(expr)
#define DCHECK_EQ(val1,val2) CHECK_OP(val1,==,val2)
#define DCHECK_NE(val1,val2) CHECK_OP(val1,!=,val2)
#define DCHECK_LE(val1,val2) CHECK_OP(val1,<=,val2)
#define DCHECK_LT(val1,val2) CHECK_OP(val1,<,val2)
#define DCHECK_GE(val1,val2) CHECK_OP(val1,>=,val2)
#define DCHECK_GT(val1,val2) CHECK_OP(val1,>,val2)
#define DCHECK_NOT_NULL(val) CHECK_NE(val,nullptr)
#define DCHECK_POW2(val) \
	CHECK((val != 0) && ((val & (val - 1)) == 0))

#else 

#define DUD_STREAM if(false) LOG(FATAL)

#define DCHECK(expr) DUD_STREAM
#define DCHECK_EQ(val1,val2) DUD_STREAM
#define DCHECK_NE(val1,val2) DUD_STREAM 
#define DCHECK_LE(val1,val2) DUD_STREAM 
#define DCHECK_LT(val1,val2) DUD_STREAM 
#define DCHECK_GE(val1,val2) DUD_STREAM 
#define DCHECK_GT(val1,val2) DUD_STREAM 
#define DCHECK_NOT_NULL(val) DUD_STREAM 
#define DCHECK_POW2(val) DUD_STREAM

#endif

// ==================== logging.cpp ======================
#include <stdio.h>

LogMessage::LogMessage(Severity severity, const char* fileName, const char* funcName, int line) :
	severity_(severity), fileName_(fileName), funcName_(funcName), line_(line) {
}

LogMessage::~LogMessage() {
	printLogMessage();
}

void LogMessage::printLogMessage() {
	fprintf(stderr, "%s: %s:%s:%d '%s'\n", SeverityNames[severity_], fileName_, funcName_, line_, str().c_str());
}

LogMessageFatal::LogMessageFatal(const char* fileName, const char* funcName, int line) :
	LogMessage(FATAL, fileName, funcName, line) {
}

LogMessageFatal::~LogMessageFatal() {
	printLogMessage();
	abort();
}

