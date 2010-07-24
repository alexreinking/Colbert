#include <QCoreApplication>
#include <iostream>
#include "scanner.h"
#include "parser.h"
#include "token.h"
#include "interpretor.h"
using namespace std;

int main(int argc, char *argv[])
{
    if(argc != 2)
    {
        cerr << "Usage: " << argv[0] << " file.clb" << endl;
        return 1;
    }

    QString fileName(argv[1]);
    Scanner s(fileName);
    s.refreshTokens();
    Parser p(&s);
    p.buildTree();
    //    foreach(Token tk, s.getTokens())
    //        cout << qPrintable(tk.getConstData()) << endl;
    //    p.printTree(p.getTree());
    Interpretor in(&p);
    in.beginInterpretation();
    return 0;
}
