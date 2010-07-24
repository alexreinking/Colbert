#ifndef NATIVEINTERFACE_H
#define NATIVEINTERFACE_H

#include <QObject>
#include "symbols.h"

class NativeInterface
{
public:
    virtual ~NativeInterface() {}
    virtual QList<NativeFunctionRow*> getFunctions() = 0;
};

Q_DECLARE_INTERFACE(NativeInterface, "com.reinking.Colbert.NativeInterface/1.0");

#endif // NATIVEINTERFACE_H
