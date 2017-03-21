#include "Log.h"

Log::Log() {
}

Log::~Log() {
	if (_pServer)
		delete _pServer;
	if (_pServerClient)
		delete _pServerClient;
}

void Log::begin() {
	begin(LOG_LEVEL_DEBUG, 115200L, true);
}

void Log::begin(int level, long baud, boolean logServer) {
	_level = constrain(level, LOG_LEVEL_NONE, LOG_LEVEL_VERBOSE);
	_baud = baud;
	_logServer = logServer;
	Serial.begin(_baud);
}

void Log::setLogLevel(int level) {
	_level = constrain(level, LOG_LEVEL_NONE, LOGLEVEL);
	if (level > LOGLEVEL) {
		error("Loglevel %d not possible", level);     // requested level
		error("Selected level %d instead", _level);   // selected/applied level
	}
}

void Log::error(char* msg, ...) {
	if (LOG_LEVEL_ERROR <= _level) {
		Serial.print("Error: ");
		va_list args;
		va_start(args, msg);
		print(msg, args);
	}
}

void Log::info(char* msg, ...) {
#if LOGLEVEL > 1
	if (LOG_LEVEL_INFO <= _level) {
		Serial.print("Info: ");
		va_list args;
		va_start(args, msg);
		print(msg, args);
	}
#endif
}

void Log::debug(char* msg, ...) {
#if LOGLEVEL > 2
	if (LOG_LEVEL_DEBUG <= _level) {
		Serial.print("Debug: ");
		va_list args;
		va_start(args, msg);
		print(msg, args);
	}
#endif
}

void Log::verbose(char* msg, ...) {
#if LOGLEVEL > 3
	if (LOG_LEVEL_VERBOSE <= _level)
	{
		Serial.print("Verbose: ");
		va_list args;
		va_start(args, msg);
		print(msg,args);
	}
#endif
}

void Log::print(const char *format, va_list args) {
	boolean sendClient = _logServer && _pServerClient
			&& _pServerClient->connected();

	for (; *format != 0; ++format) {
		if (*format == '%') {
			++format;
			if (*format == '\0')
				break;
			if (*format == '%') {
				Serial.print(*format);
				if (sendClient)
					_pServerClient->print(*format);

				continue;
			}
			if (*format == 's') {
				register char *s = (char *) va_arg(args, int);
				Serial.print(s);
				if (sendClient)
					_pServerClient->print(s);
				continue;
			}
			if (*format == 'd' || *format == 'i') {
				Serial.print(va_arg(args, int), DEC);
				if (sendClient)
					_pServerClient->print(va_arg(args, int), DEC);
				continue;
			}
			if (*format == 'x') {
				Serial.print(va_arg(args, int), HEX);
				if (sendClient)
					_pServerClient->print(va_arg(args, int), HEX);
				continue;
			}
			if (*format == 'X') {
				Serial.print("0x");
				Serial.print(va_arg(args, int), HEX);
				if (sendClient) {
					_pServerClient->print("0x");
					_pServerClient->print(va_arg(args, int), HEX);
				}
				continue;
			}
			if (*format == 'b') {
				Serial.print(va_arg(args, int), BIN);
				if (sendClient)
					_pServerClient->print(va_arg(args, int), BIN);
				continue;
			}
			if (*format == 'B') {
				Serial.print("0b");
				Serial.print(va_arg(args, int), BIN);
				if (sendClient) {
					_pServerClient->print("0b");
					_pServerClient->print(va_arg(args, int), BIN);
				}
				continue;
			}
			if (*format == 'l') {
				Serial.print(va_arg(args, long), DEC);
				if (sendClient)
					_pServerClient->print(va_arg(args, long), DEC);
				continue;
			}

			if (*format == 'c') {
				Serial.print(va_arg(args, int));
				if (sendClient)
					_pServerClient->print(va_arg(args, int));
				continue;
			}
			if (*format == 't') {
				if (va_arg(
						args, int) == 1) {
					Serial.print("T");
					if (sendClient)
						_pServerClient->print("T");
				} else {
					Serial.print("F");
					if (sendClient)
						_pServerClient->print("F");
				}
				continue;
			}
			if (*format == 'T') {
				if (va_arg(args, int) == 1) {
					Serial.print("true");
					if (sendClient)
						_pServerClient->print("true");
				} else {
					Serial.print("false");
					if (sendClient)
						_pServerClient->print("false");
				}
				continue;
			}
		}
		Serial.print(*format);
		if (sendClient)
			_pServerClient->print(*format);
	}
	Serial.println("");
	if (sendClient)
		_pServerClient->println("");
}

void Log::loop() {
	if (!_logServer)
		return;

	if (_logServer && !_pServer) {
		_pServer = new WiFiServer(8888);
		_pServerClient = new WiFiClient();
		_pServer->begin();
		_pServer->setNoDelay(true);
	}

	if (_pServer->hasClient()) {

		if (!_pServerClient || !_pServerClient->connected()) {
			if (_pServerClient)
				_pServerClient->stop();
			*_pServerClient = _pServer->available();
		}
		WiFiClient serverClient = _pServer->available();
		serverClient.stop();
	}
}

Log Logger;
