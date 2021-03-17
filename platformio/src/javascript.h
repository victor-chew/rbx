#ifndef JAVASCRIPT_H
#define JAVASCRIPT_H

void pushJsKey(String key);
void initJavascript();
void runJavascript(const char* src); 
void stopJavascript();
String getJsAutorun();
void setJsAutorun(String scriptName);
void autorunJavascript();
bool isScriptRunning();
bool pendingJsTermination();

#endif // JAVASCRIPT_H