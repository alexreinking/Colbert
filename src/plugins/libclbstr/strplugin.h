#ifndef STRINGPLUGIN_H
#define STRINGPLUGIN_H

#include <QtCore>
#include <math.h>
#include <nativeinterface.h>
#include <codetreenodes.h>
#include <symbols.h>

class StringPlugin : public QObject, NativeInterface
{
    Q_OBJECT
    Q_INTERFACES(NativeInterface)
public:
    QList<NativeFunctionRow*> getFunctions();
    static QVariant __split(QList<VariableRow*> args);
    static QVariant __strlen(QList<VariableRow*> args);
    static QVariant __join(QList<VariableRow*> args);

private:
    QList<NativeFunctionRow*> functionList;
    NativeFunctionRow* createSplit();
    NativeFunctionRow* createStrlen();
    NativeFunctionRow* createJoin();
};

#endif // STRINGPLUGIN_H