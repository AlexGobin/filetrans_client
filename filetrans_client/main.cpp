#include "DispatchMsgService.h"
#include "interface.h"
#include "Logger.h"
#include "BusProcessor.h"

#include <functional>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>

#include <sys/types.h>
#include <sys/wait.h>

#define OPTERRCOLON (1)
#define OPTERRNF (2)
#define OPTERRARG (3)
#define DEFAULT_BIN_NAME "filetrans_client"
#define DEFAULT_SVR_PORT "9090" 
#define DEFAULT_SVR_IP   "192.168.116.216" 
#define BUF_SIZE   1024


int optreset = 0;
int optind = 1;
int opterr = 1;
int optopt;

static int _fprintf(FILE *file, const char *fmt,  ...)
{
    if (file == NULL)
        return -1;

    va_list args;
    va_start(args,fmt);
    int result = ::vfprintf(file, fmt, args);
    va_end(args);

    return result;
}

static int optiserr(int argc, char * const *argv, int oint, const char *optstr, int optchr, int err)
{
    if(opterr)
    {
        _fprintf(stderr, "Error in argument %d, char %d: ", oint, optchr+1);
        switch(err)
        {
        case OPTERRCOLON:
            _fprintf(stderr, ": in flags\n");
            break;
        case OPTERRNF:
            _fprintf(stderr, "option not found %c\n", argv[oint][optchr]);
            break;
        case OPTERRARG:
            _fprintf(stderr, "no argument for option %c\n", argv[oint][optchr]);
            break;
        default:
            _fprintf(stderr, "unknown\n");
            break;
        }
    }
    optopt = argv[oint][optchr];
    return('?');
}

static int getopt(int argc, char* const *argv, const char *optstr)
{
    static int optchr = 0;
    static int dash = 0; /* have already seen the - */

    char *cp;

    if (optreset)
        optreset = optchr = dash = 0;
    if(optind >= argc)
        return(EOF);
    if(!dash && (argv[optind][0] !=  '-'))
        return(EOF);
    if(!dash && (argv[optind][0] ==  '-') && !argv[optind][1])
    {
        /*
         * use to specify stdin. Need to let pgm process this and
         * the following args
         */
        return(EOF);
    }
    if((argv[optind][0] == '-') && (argv[optind][1] == '-'))
    {
        /* -- indicates end of args */
        optind++;
        return(EOF);
    }
    if(!dash)
    {
        assert((argv[optind][0] == '-') && argv[optind][1]);
        dash = 1;
        optchr = 1;
    }

    /* Check if the guy tries to do a -: kind of flag */
    assert(dash);
    if(argv[optind][optchr] == ':')
    {
        dash = 0;
        optind++;
        return(optiserr(argc, argv, optind-1, optstr, optchr, OPTERRCOLON));
    }
    if(!(cp = strchr(optstr, argv[optind][optchr])))
    {
        int errind = optind;
        int errchr = optchr;

        if(!argv[optind][optchr+1])
        {
            dash = 0;
            optind++;
        }
        else
            optchr++;
        return(optiserr(argc, argv, errind, optstr, errchr, OPTERRNF));
    }
    if(cp[1] == ':')
    {
        dash = 0;
        optind++;
        if(optind == argc)
            return(optiserr(argc, argv, optind-1, optstr, optchr, OPTERRARG));
        optarg = argv[optind++];
        return(*cp);
    }
    else
    {
        if(!argv[optind][optchr+1])
        {
            dash = 0;
            optind++;
        }
        else
            optchr++;
        return(*cp);
    }
    assert(0);
    return(0);
}

static int daemon(int nochdir, int noclose)
{
    int fd;

    switch (fork())
    {
        case -1:
            return (-1);
        case 0:
            break;
        default:
            _exit(0);
    }

    if (setsid() == -1)
        return (-1);

    if (!nochdir)
        (void)chdir("/");

    if (!noclose && (fd = open("/dev/null", O_RDWR, 0)) != -1) {
        (void)dup2(fd, STDIN_FILENO);
        (void)dup2(fd, STDOUT_FILENO);
        (void)dup2(fd, STDERR_FILENO);
        if (fd > 2)
            (void)close (fd);
    }

    return (0);
}

static void usage()
{
    char* usage_name = DEFAULT_BIN_NAME;
    printf("usage: %s [ -d | -v | -f segment_filename | -t threads | -l log.conf ]\n", usage_name);
    printf("-d: Run in the foreground\n");
    printf("-f original_filename: Specify a file to download from servers\n");
    printf("-t threads: Specify a parameter how many threads we will start \n");
    printf("-l log.conf: Specify a log config file\n");
}

int main(int argc, char** argv)
{
    if (argc <= 1) usage();

    char ch;
    bool run_in_foreground = false;
    std::string log_file_path = "";
    std::string orignal_file_name = "";
    u32 threads_for_download = "";
    while ((ch = getopt(argc,argv, "vdf:t:l:")) != EOF)
    {
        switch(ch)
        {
            case 'v':
                usage();
                ::exit(0);
                break;
            case 'd':
                run_in_foreground = true;
                break;
            case 'f':
                assert(optarg != NULL);// this means we didn't declare getopt options correctly or there is a bug in getopt.
                orignal_file_name = std::string(optarg);
                break;
            case 't':
                assert(optarg != NULL);// this means we didn't declare getopt options correctly or there is a bug in getopt.
                threads_for_download= (u32)optarg;
                break;
            case 'l':
                assert(optarg != NULL);// this means we didn't declare getopt options correctly or there is a bug in getopt.
                log_file_path= std::string(optarg);
                break;
            default:
                break;
        }
    }

    if(!Logger::instance()->init(log_file_path))
    {
        printf("init log module with file path = %s failed.\n", log_file_path.c_str());
        return -2;
    }

    /*if (!run_in_foreground)
    {
        if (0 != daemon(0, 0))
        {
            LOG_ERROR("run in daemon failed with error %s", strerror(errno));
            exit(-1);
        }
    }*/

    //订阅所有的消息类型
    std::shared_ptr<DispatchMsgService> dms(new DispatchMsgService); //EventHandler
    dms->open();

    //开启订阅，创建事件处理实例
    BusinessProcessor processor(dms);//event_handler 
    processor.init();    
    std::function< iEvent* (const iEvent*)> fun = std::bind(&DispatchMsgService::process, dms.get(), std::placeholders::_1);

    // create server socket and set to non block
    Interface intf(fun,orignal_file_name,threads_for_download);//把事件处理回调函数注册到Interface（reactorImpl）
    int guard_client_socket = intf.create_and_conn_serv(DEFAULT_SVR_IP,DEFAULT_SVR_PORT);
    intf.add_client_socket(guard_client_socket);
    u8 ready_2_rs_msg[BUF_SIZE] = {0};
    int encoded_msg_length = intf.encode_filetrans_start_msg(ready_2_rs_msg);	

    int sended_length = intf.nio_write(guard_client_socket,ready_2_rs_msg,encoded_msg_length);
    if(sended_length != encoded_msg_length)
    {
        LOG_ERROR("Can't send message completely,already send length:%d,but shoud send length:%d",sizeof(ready_2_rs_msg),sended_length);
        return (-1);
    }
    
    //monitor the guard_client_socket,if msg is come on
    while(1){
        int result = 0;
        memset(ready_2_rs_msg,0x0,sizeof(char)*BUF_SIZE);
        intf.nio_recv(guard_client_socket,ready_2_rs_msg,sizeof(protocol_head_t),&result);
        if (result <= 0) continue;
        else{
            LOG_DEBUG("guard_client_socket has received message:%s",ready_2_rs_msg);
            break;
        }        
    }
    //intf.run();//reactor.event_loop

    for(;;)
    {
        LOG_INFO("guard process is hanging up!");
        sleep(1);
    }

    return 0;
}


