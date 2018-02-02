#include <jni.h>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/un.h>
#include <sys/socket.h>
#include <string.h>

extern "C" {

#include "android/log.h"
static const char *TAG="Canopencommand";
#define LOGI(fmt, args...) __android_log_print(ANDROID_LOG_INFO,  TAG, fmt, ##args)
#define LOGD(fmt, args...) __android_log_print(ANDROID_LOG_DEBUG, TAG, fmt, ##args)
#define BUF_SIZE            100000
static const int ERRNO_BUFFER_LEN = 1024;
/* Helper functions */
void errExit(char *msg) {
    perror(msg);
    exit(EXIT_FAILURE);
}

/**
 * 抛出异常方法
 */
static void throwException(JNIEnv *env, const std::string& exception_name,
                           const std::string& msg)
{
    const jclass exception = env->FindClass(exception_name.c_str());
    if (exception == NULL) {
        return;
    }
    env->ThrowNew(exception, msg.c_str());
}

/**
 * 抛出IO异常
 */
static void throwIOExceptionMsg(JNIEnv *env, const std::string& msg)
{
    throwException(env, "java/io/IOException", msg);
}

/**
 * 抛出errno对应的异常信息
 */
static void throwIOExceptionErrno(JNIEnv *env, const int exc_errno)
{
    char message[ERRNO_BUFFER_LEN];
    // The strerror() function returns a pointer to a string that describes the error code
    const char *const msg = (char *) strerror_r(exc_errno, message, ERRNO_BUFFER_LEN);
    if (((long)msg) == 0) {
        // POSIX strerror_r, success
        throwIOExceptionMsg(env, std::string(message));
    } else if (((long)msg) == -1) {
        // POSIX strerror_r, failure
        // (Strictly, POSIX only guarantees a value other than 0. The safest
        // way to implement this function is to use C++ and overload on the
        // type of strerror_r to accurately distinguish GNU from POSIX. But
        // realistic implementations will always return -1.)
        snprintf(message, ERRNO_BUFFER_LEN, "errno %d", exc_errno);
        throwIOExceptionMsg(env, std::string(message));
    } else {
        // glibc strerror_r returning a string
        throwIOExceptionMsg(env, std::string(msg));
    }
}

/**
 * 抛出参数传递异常
 */
static void throwIllegalArgumentException(JNIEnv *env, const std::string& message)
{
    throwException(env, "java/lang/IllegalArgumentException", message);
}

/**
 * 内存越界异常
 */
static void throwOutOfMemoryError(JNIEnv *env, const std::string& message)
{
    throwException(env, "java/lang/OutOfMemoryError", message);
}

static int printErrorDescription = 0;

JNIEXPORT jstring JNICALL Java_com_aplex_canopen_CANopen_canopenUtils
        (JNIEnv *env, jobject obj, jobjectArray argv){
    char *socketPath = "/dev/CO_command_socket";  /* Name of the local domain socket, configurable by arguments. */
    char *inputFilePath = NULL;
    char buf[BUF_SIZE];
    int fd;
    struct sockaddr_un addr;
    int opt;
    int i;
    int client_len;


    /* 创建socket */
    fd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (fd == -1) {
        errExit("Socket creation failed");
    }

    memset(&addr, 0, sizeof(struct sockaddr_un));
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, socketPath, sizeof(addr.sun_path) - 1);

    if (connect(fd, (struct sockaddr *) &addr, sizeof(struct sockaddr_un)) == -1) {
        errExit("Socket connection failed");
    }

    buf[0] = 0;
    size_t buflen = 0;
    printf("节点名为31：%s\n", socketPath);

    /* Add sequence number if not present on command line arguments */
    if (argv[optind][0] != '[') {
        strcat(buf, "[1] ");
    }

    for (i = optind; i < argc; i++) {
        strncat(buf, argv[i], (BUF_SIZE - 2) - buflen);
        strcat(buf, " ");
        buflen = strlen(buf);
        if (buflen >= (BUF_SIZE - 1)) {
            fprintf(stderr, "String too long!\n");
            exit(EXIT_FAILURE);
        }
    }
    printf("222\n");

    buf[buflen - 1] = '\n'; /* replace last space with newline */
    buf[buflen] = 0;
    printf("fd=%d;  buf = %s\n", fd, buf);
    printErrorDescription = 1;
    sendCommand(fd, buf, buflen);

    close(fd);

    exit(EXIT_SUCCESS);

    return env->NewStringUTF(hello.c_str());

}



//JNIEXPORT jstring JNICALL Java_com_aplex_canopen_CANopen_stringFromJNI(
//        JNIEnv *env,
//        jobject /* this */) {
//    std::string hello = "Hello from C++ aplex222";
//    return env->NewStringUTF(hello.c_str());
//
//}
//
///*
// * Class:     com_aplex_canopen_CANopen
// * Method:    excuteUtils
// * Signature: (I[Ljava/lang/String;)Ljava/lang/String;
// */
//JNIEXPORT jstring JNICALL Java_com_aplex_canopen_CANopen_excuteUtils
//        (JNIEnv *env, jobject, jint argc, jobjectArray argvbak) {
//    char *socketPath = "/dev/CO_command_socket";  /* Name of the local domain socket, configurable by arguments. */
//    char *inputFilePath = NULL;
//    std::string hello = "Hello from C++ aplex222";
//    char buf[BUF_SIZE];
//    int fd;
//    struct sockaddr_un addr;
//    int opt;
//    int i;
//    int client_len;
//
//    char *const argv[] = {"1", "1"};
//    //帮助信息
////    if(argc >= 2 && strcmp(argv[1], "--help") == 0) {
////        printUsage(argv[0]);
////        exit(EXIT_SUCCESS);
////    }
//    //帮助信息和错误信息
////    if(argc >= 2 && strcmp(argv[1], "--helpall") == 0) {
////        exit(EXIT_SUCCESS);
////    }
//
//    /* Get program options 没啥用，一般这三个都不要*/
//    while ((opt = getopt(argc, argv, "s:f:h")) != -1) {
//        switch (opt) {
//            case 'f':
//                inputFilePath = optarg;    //从文件中读取命令
//                break;
//            case 's':
//                socketPath = optarg;        //指定socket路径
//                break;
//            case 'h':
//                printErrorDescription = 1;  //指定错误描述
//                break;
//            default:
//                exit(EXIT_FAILURE);
//        }
//    }
//    printf("节点名为：%s\n", socketPath);
//
//    /* Create and connect client socket */
//    fd = socket(AF_UNIX, SOCK_STREAM, 0);
//    if (fd == -1) {
//        errExit("Socket creation failed");
//    }
//
//    memset(&addr, 0, sizeof(struct sockaddr_un));
//    addr.sun_family = AF_UNIX;
//    strncpy(addr.sun_path, socketPath, sizeof(addr.sun_path) - 1);
//
//    if (connect(fd, (struct sockaddr *) &addr, sizeof(struct sockaddr_un)) == -1) {
//        errExit("Socket connection failed");
//    }
//
//    printf("节点名为1：%s\n", socketPath);
//
//    /* get commands from input file, line after line */
//    if (inputFilePath != NULL) {
//        printf("节点名为2：%s\n", socketPath);
//        FILE *fp = fopen(inputFilePath, "r");
//        if (fp == NULL) {
//            errExit("Can't open input file");
//        }
//
//        while (fgets(buf, BUF_SIZE, fp) != NULL) {
//            sendCommand(fd, buf, strlen(buf));
//        }
//
//
//        fclose(fp);
//    }
//        /* get command from arguments */
//    else if (optind < argc) {
//        buf[0] = 0;
//        size_t buflen = 0;
//        printf("节点名为31：%s\n", socketPath);
//        /* Add sequence number if not present on command line arguments */
//        if (argv[optind][0] != '[') {
//            strcat(buf, "[1] ");
//        }
//        printf("1\n");
//        for (i = optind; i < argc; i++) {
//            strncat(buf, argv[i], (BUF_SIZE - 2) - buflen);
//            strcat(buf, " ");
//            buflen = strlen(buf);
//            if (buflen >= (BUF_SIZE - 1)) {
//                fprintf(stderr, "String too long!\n");
//                exit(EXIT_FAILURE);
//            }
//        }
//        printf("222\n");
//
//        buf[buflen - 1] = '\n'; /* replace last space with newline */
//        buf[buflen] = 0;
//        printf("fd=%d;  buf = %s\n", fd, buf);
//        printErrorDescription = 1;
//        sendCommand(fd, buf, buflen);
//    }
//    /* get commands from stdin, line after line从标准输入读取 */
//    else {
//        while (fgets(buf, BUF_SIZE, stdin) != NULL) {
//            sendCommand(fd, buf, strlen(buf));
//        }
//    }
//
//    close(fd);
//
//    exit(EXIT_SUCCESS);
//
//    return env->NewStringUTF(hello.c_str());
//}
//



static void sendCommand(int fd, char *command, size_t commandLength) {
//    size_t n;
//    char buf[BUF_SIZE];
//
//    if (write(fd, command, commandLength) != commandLength) {
//        //errExit("Socket write failed");
//    }
//
//    n = read(fd, buf, sizeof(buf));
//
//    if (n == -1) {
//        //errExit("Socket read failed");
//    }
//
//    if (printErrorDescription == 1) {
//        char *errLoc = strstr(buf, "ERROR:");
//        char *endLoc = strstr(buf, "\r\n");
//
//        if (errLoc != NULL && endLoc != NULL) {
//            int num;
//            char *sRet = NULL;
//
//            errLoc += 6;
//
//            num = strtol(errLoc, &sRet, 0);
//            if (strlen(errLoc) != 0 && sRet == strchr(errLoc, '\r')) {
//                int i, len;
//
//                //len = sizeof(errorDescs) / sizeof(errorDescs_t);
//
//                for (i = 0; i < len; i++) {
//                    //const errorDescs_t *ed = &errorDescs[i];
////                    if(ed->code == num) {
////                        sprintf(endLoc, " - %s\r\n", ed->desc);
////                        break;
////                    }
//                }
//            }
//        }
////    }
//
//    printf("%s", buf);
}















}
