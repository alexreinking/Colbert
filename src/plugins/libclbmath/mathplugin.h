#ifndef MATHPLUGIN_H
#define MATHPLUGIN_H

#include <QtCore>
#include <math.h>
#include <nativeinterface.h>
#include <codetreenodes.h>
#include <symbols.h>

class MathPlugin : public QObject, NativeInterface
{
    Q_OBJECT
    Q_INTERFACES(NativeInterface)
public:
    QList<NativeFunctionRow*> getFunctions();
    static QVariant ___cos(QList<QVariant> args);
    static QVariant ___sin(QList<QVariant> args);

private:
    QList<NativeFunctionRow*> functionList;
    NativeFunctionRow* createCos();
    NativeFunctionRow* createSin();
};

#endif // MATHPLUGIN_H
