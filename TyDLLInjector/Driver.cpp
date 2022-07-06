/**/

#include "stdafx.h"
#include "TyStringHelper.h"
#include "TyInjector.h"

using namespace std;

#define DEMO_VERSION 1


void printUsage()
{
    const char* usageString = "usage:\tTyDllInjector.exe TARGET_PROCESS INJECT_ME\n\n"
        "\tpositional arguments:\n"
        "\tTARGET_PROCESS\t\t\tTarget PID to inject into\n"
        "\tINJECT_ME\t\t\tPath to DLL to inject\n\n";

    printf("%s", usageString);
}

void runDemoOne()
{
    TyInjector *myInjector = new TyInjector();

    wchar_t* DLLPayload = (wchar_t*)L"C:\\Users\\tgree\\source\\repos\\"
        "PrivateProjects\\MeterpreterCallback\\x64\\Release\\MeterpreterCallback.dll";
    myInjector->injectDLL(DLLPayload);
}

int main(int argc, char *argv[])
{
    /*
    if (argc != 2)
    {
        printUsage();
        return 0;
    }

    std::string target_pid_str(argv[1]);
    int target_pid = std::stoi(target_pid_str);

    std::string injectee(argv[2]);

    const wchar_t* injectere_wide = getWideCFromStdString(injectee);
    */

    switch (DEMO_VERSION)
    {
        //DLL injection
        case 1:
            runDemoOne();
            break;
        case 2:
            cout << "case 2" << endl;
            break;
        default:
            cout << "default" << endl;
            break;
    }


    return 0;
}

