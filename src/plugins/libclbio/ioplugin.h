#ifndef IOPLUGIN_H
#define IOPLUGIN_H

#include <QtCore>
#include <math.h>
#include <nativeinterface.h>
#include <codetreenodes.h>
#include <symbols.h>

class IOPlugin : public QObject, NativeInterface
{
    Q_OBJECT
    Q_INTERFACES(NativeInterface)
public:
    QList<NativeFunctionRow*> getFunctions();
    static QVariant ___print(QList<VariableRow*> args);
    static QVariant ___prompt(QList<VariableRow*> args);

private:
    QList<NativeFunctionRow*> functionList;
    NativeFunctionRow* createPrint();
    NativeFunctionRow* createPrompt();
};

#endif // IOPLUGIN_H
