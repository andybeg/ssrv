#include "version.h"
#include "Emulator.h"
#include <unistd.h>
#include <getopt.h>
#include "CCmdInterpreter.h"
//#include "Session.h"

static void print_usage(const char *prog)
{
    printf("Usage: %s -[v]\n", prog);
    puts("  -v  Get version info\n"
         "     \n");
    exit(1);
}



int main(int argc,char *argv[])
{
    CCmdInterpreter * m_Cmd;
    m_Cmd = new CCmdInterpreter;
    //parse_opts(argc, argv);
    m_Cmd->run();
    while(1)
    {
        sleep(1000000);
    }
}
