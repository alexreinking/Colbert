#ifndef NATIVEINTERFACE_H
#define NATIVEINTERFACE_H

#include <QObject>
#include "symbols.h"

class NativeInterface : public QObject
{
public:
    virtual ~NativeInterface() {}
    virtual QList<NativeFunctionRow*> getFunctions() = 0;
};

#endif // NATIVEINTERFACE_H
