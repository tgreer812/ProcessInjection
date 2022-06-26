/**/

#include "stdafx.h"

using namespace std;

void printUsage()
{
    const char* usageString = "usage:\tTyDllInjector.exe TARGET_PROCESS INJECT_ME\n\n"
        "\tpositional arguments:\n"
        "\tTARGET_PROCESS\t\t\tTarget PID to inject into\n"
        "\tINJECT_ME\t\t\tPath to DLL to inject\n\n";

    printf("%s", usageString);
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printUsage();
        return 0;
    }

    std::string target_pid_str(argv[1]);
    int target_pid = std::stoi(target_pid_str);

    std::string injectee(argv[2]);


    return 0;
}

