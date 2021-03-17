#ifndef WEBSERVER_H
#define WEBSERVER_H

void initWebServer();
void logger(char type, const char* fmt, ...);
void serial(const char* fmt, ...);

#endif // WEBSERVER_H