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
    Q_PLUGIN_METADATA(IID "com.alexreinking.Colbert.plugins.MathPlugin" FILE "math.json")
    Q_INTERFACES(NativeInterface)
public:
    QList<NativeFunctionRow*> getFunctions();
    static QVariant ___cos(QList<VariableRow*> args);
    static QVariant ___sin(QList<VariableRow*> args);
    static QVariant ___tan(QList<VariableRow*> args);

private:
    QList<NativeFunctionRow*> functionList;
    NativeFunctionRow* createCos();
    NativeFunctionRow* createSin();
    NativeFunctionRow* createTan();
};

#endif // MATHPLUGIN_H
