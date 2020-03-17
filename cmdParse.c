/*********************************************************************************************************
** 文件: cmdParse.c
** 作者: scilogyhunter
** 站点: https://blog.csdn.net/scilogyhunter/category_9735056.html
** 描述: 通用命令行参数处理工具
*********************************************************************************************************/
#include "cmdParse.h"
/*********************************************************************************************************
** 名称: help
** 输入: cmd        命令参数
**       opti       选项索引号
** 输出: 错误码，NONE成功，ERROR错误
** 说明: 帮助信息，列出命令选项列表极其简要说明，免去了每个命令重复实现
*********************************************************************************************************/
static  int  help (command_st  *cmd, int opti)
{
#define OPT_NAME_LENGTH_MIX             (8)                             /*  默认打印选项名最小长度      */
    int  i;
    int  j;
    int  l;
    int  length = OPT_NAME_LENGTH_MIX;

    if (cmd == NULL) {
        return  (ERROR);
    }

    if (cmd->optc == 0) {
        return  (ERROR);
    }

    for (i = 0; i < cmd->optc; i++) {
        l = strlen(cmd->optv[i].name);
        if (l > length) {
            length = l + 1;
        }
    }

    printf("shell cmd \"%s\" options brief:\n", cmd->argv[0]);
    printf("no. opt      ");
    if (length > OPT_NAME_LENGTH_MIX) {
        for (j = 0; j < (length - OPT_NAME_LENGTH_MIX); j++) {
            printf(" ");
        }
    }
    printf("att      brief\n");

    for (i = 0; i < cmd->optc; i++) {
        printf("[%02d]%c%s", i, OPT_LABEL_CHAR, cmd->optv[i].name);
        l = strlen(cmd->optv[i].name);
        for (j = 0; j < (length - l); j++) {
            printf(" ");
        }
        printf("%08x %s\n", cmd->optv[i].att, cmd->optv[i].brief);
    }

    return  (NONE);
}
/*********************************************************************************************************
** 名称: info
** 输入: cmd        命令参数
**       opti       选项索引号
** 输出: 错误码，NONE成功，ERROR错误
** 说明: 命令选项列表的所有详细信息，用于实现命令行自身调试
*********************************************************************************************************/
static  int  info (command_st  *cmd, int opti)
{
    int          i;
    int          j;
    int          type;
    int          count;
    void        *target;
    option_st   *opt;

    printf("shell cmd \"%s\" argument parse information:\n", cmd->argv[0]);
    /*
     * 命令行参数打印
     */
    printf("argc = %d\n", cmd->argc);
    printf("argv :\n");
    for (i = 0; i < cmd->argc; i++) {
        printf("[%02d] = %s\n", i, cmd->argv[i]);
    }
    /*
     * 选项列表信息打印
     */
    printf("optc = %d\n", cmd->optc);
    printf("no. name    target   arg      att       i  c brief\n");
    for (i = 0; i < cmd->optc; i++) {
        opt = &cmd->optv[i];
        printf("[%02d]", i);
        printf("%c", OPT_LABEL_CHAR);
        printf("%-6s ", opt->name);
        printf("%08x ", (uint32_t)opt->target);
        printf("%08x ", (uint32_t)opt->targetArg);
        printf("%08x ", opt->att);
        printf("%2u ",  opt->index);
        printf("%2u ",  opt->argc);
        printf("%s\n",  opt->brief);
    }

    /*
     * 选项列表获取值打印
     */
    printf("target value:\n");
    for (i = 0; i < cmd->optc; i++) {
        opt  = &cmd->optv[i];
        type = ATT_GET_TYPE(opt);
        target = opt->target;

        printf("[%02d]", i);

        if (opt->att & ATT_FUNC) {
            for (j = 0; j < opt->argc; j++) {
                printf(" %s", cmd->argv[1 + opt->index + j]);
            }
            printf("\n");
            continue;
        }

        if (type == ATT_TYPE_EXIST) {
            printf(" %s\n", (*(uint32_t *)target ? "true" :"false"));
            continue;
        }

        if (opt->argc == 0) {
            printf("\n");
            continue;
        }

        count = ATT_GET_ARRAY(opt);
        if (count == 0) {
            count = 1;
        } else {
            count = (count < opt->argc) ? count : opt->argc;
        }

        for (j = 0; j < count; j++) {
            switch (type) {
            case ATT_TYPE_VOID:
                break;
            case ATT_TYPE_BOOL:
                printf(" %s", (*(uint32_t *)target ? "true" :"false"));
                break;
            case ATT_TYPE_CHAR:
                printf(" '%c'", *(char  *)target);
                break;
            case ATT_TYPE_SPTR:
                printf(" \"%s\"", *(char  **)target);
                break;
            case ATT_TYPE_SBUF:
                printf(" \"%s\"", (char  *)target);
                break;
            case ATT_TYPE_U64:
                printf(" %llu", *(uint64_t *)target);
                break;
            case ATT_TYPE_S64:
                printf(" %lld", *(int64_t *)target);
                break;
            case ATT_TYPE_F64:
                printf(" %f", *(double *)target);
                break;
            case ATT_TYPE_U32:
                printf(" %u", *(uint32_t *)target);
                break;
            case ATT_TYPE_S32:
                printf(" %d", *(int32_t *)target);
                break;
            case ATT_TYPE_F32:
                printf(" %f", *(float *)target);
                break;
            case ATT_TYPE_U16:
                printf(" %u", *(uint16_t *)target);
                break;
            case ATT_TYPE_S16:
                printf(" %d", *(int16_t *)target);
                break;
            case ATT_TYPE_U8:
                printf(" %u", *(uint8_t *)target);
                break;
            case ATT_TYPE_S8:
                printf(" %d", *(int8_t *)target);
                break;
            default:
                return  (ERROR);
            }

            /*
             * 按照目标数据类型调整目标地址
             */
            switch (type) {
            case ATT_TYPE_SPTR:
                target += sizeof(char *);
                break;
            case ATT_TYPE_SBUF:
                target += ATT_GET_BUF_SIZE(opt);
                break;
            case ATT_TYPE_U64:
            case ATT_TYPE_S64:
            case ATT_TYPE_F64:
                target += 8;
                break;
            case ATT_TYPE_EXIST:
            case ATT_TYPE_BOOL:
                target += sizeof(uint32_t);
                break;
            case ATT_TYPE_U32:
            case ATT_TYPE_S32:
            case ATT_TYPE_F32:
                target += 4;
                break;
            case ATT_TYPE_U16:
            case ATT_TYPE_S16:
                target += 2;
                break;
            case ATT_TYPE_U8:
            case ATT_TYPE_S8:
            case ATT_TYPE_CHAR:
                target += 1;
                break;
            default:
                printf("error:type error!(%d)", type);
                return  (ERROR);
            }
        }
        printf("\n");
    }

    return  (NONE);
}
/*********************************************************************************************************
** 名称: targetSet
** 输入: cmd        命令参数
**       opti       选项索引号
** 输出: 错误码，NONE成功，ERROR错误
** 说明: 对目标进行赋值
*********************************************************************************************************/
static int targetSet (command_st  *cmd, int opti)
{
    int          i;
    int          type;
    int          count;

    char        *arg;
    void        *target;
    char        *endptr;
    option_st   *opt;

    opt    = &cmd->optv[opti];
    type   = ATT_GET_TYPE(opt);
    target = opt->target;

    if (opt->att & ATT_FUNC) {
        return  (NONE);
    }

    if (type == ATT_TYPE_VOID) {
        return  (NONE);
    }

    if (opt->targetArg) {
        *(int32_t *)opt->targetArg = 0;
    }

    /*
     * 如果选项没有在命令行中出现
     */
    if (opt->index == 0) {
        if (opt->att & ATT_FORCE) {
            printf("error:missing option \"-%s\"!\n", opt->name);
            return  (ERROR);
        }

        if (type == ATT_TYPE_EXIST) {
            *(uint32_t *)target = 0;
            return  (NONE);
        }

        return  (NONE);
    }

    if (type == ATT_TYPE_EXIST) {
        *(uint32_t *)target = !0;
        if (opt->targetArg) {
            *(int32_t *)opt->targetArg = 0;
        }
        return  (NONE);
    }

    /*
     * 除了ATT_TYPE_EXIST类型外，其他类型都必须携带选项参数，否则报错
     */
    if (opt->argc == 0) {
        printf("error:option \"-%s\" missing arguments!\n", opt->name);
        return  (ERROR);
    }

    count = ATT_GET_ARRAY(opt);
    if (count == 0) {
        count = 1;
    } else {
        if (count < opt->argc) {
            printf("error:option argc more than array count!(%s)\n", opt->name);
        } else {
            count = opt->argc;
        }
    }

    for (i = 0; i < count; i++) {
        arg = cmd->argv[opt->index + 1 + i];

        /*
         * 按照目标数据类型进行赋值
         */
        if (type == ATT_TYPE_BOOL) {
            uint32_t  value;
            value = strtoul(arg, &endptr, 0);

            if (*endptr == '\0') {
                if (value != 0) {
                    value = !0;
                }
            } else {
                if (strcasecmp(arg, "true") == 0) {
                    value = !0;
                } else if (strcasecmp(arg, "false") == 0) {
                    value = 0;
                } else if (strcasecmp(arg, "on") == 0) {
                    value = !0;
                } else if (strcasecmp(arg, "off") == 0) {
                    value = 0;
                } else if (strcasecmp(arg, "high") == 0) {
                    value = !0;
                } else if (strcasecmp(arg, "low") == 0) {
                    value = 0;
                } else if (strcasecmp(arg, "enable") == 0) {
                    value = !0;
                } else if (strcasecmp(arg, "disable") == 0) {
                    value = 0;
                } else if (strcasecmp(arg, "up") == 0) {
                    value = !0;
                } else if (strcasecmp(arg, "down") == 0) {
                    value = 0;
                } else if (strcasecmp(arg, "start") == 0) {
                    value = !0;
                } else if (strcasecmp(arg, "stop") == 0) {
                    value = 0;
                } else {
                    printf("error:unknown bool value!(%s)", arg);
                    return  (ERROR);
                }
            }
            *(uint32_t *)target = value;
        } else if (type == ATT_TYPE_CHAR) {
            if (*arg & 0x80) {
                printf("error:ATT_TYPE_CHAR arg error!(%02x)", *arg);
            }
            *(char  *)target = *arg;
        } else if (type == ATT_TYPE_SPTR) {
            *(char  **)target = arg;

        } else if (type == ATT_TYPE_SBUF) {
            int len = strlen(arg);
            if (len > ATT_GET_BUF_SIZE(opt)) {
                len = ATT_GET_BUF_SIZE(opt);
                printf("error:sbuf arg lenth over!(%s)\n", arg);
            }
            memcpy(target, arg, len);
        } else {
            switch (type) {
            case ATT_TYPE_U64:
                *(uint64_t *)target = strtoull(arg, &endptr, 0);
                break;
            case ATT_TYPE_S64:
                *(int64_t *)target = strtoll(arg, &endptr, 0);
                break;
            case ATT_TYPE_F64:
                *(double *)target = strtod(arg, &endptr);
                break;
            case ATT_TYPE_U32:
                *(uint32_t *)target = strtoul(arg, &endptr, 0);
                break;
            case ATT_TYPE_S32:
                *(int32_t *)target = strtol(arg, &endptr, 0);
                break;
            case ATT_TYPE_F32:
                *(float *)target = strtof(arg, &endptr);
                break;
            case ATT_TYPE_U16:
                {
                    uint32_t  value;
                    value = strtoul(arg, &endptr, 0);
                    if (value > 0xffff) {
                        printf("error:u16 type arg error!(%u)\n", value);
                    }
                    *(uint16_t *)target = value;
                }
                break;
            case ATT_TYPE_S16:
                {
                    int32_t  value;
                    value = strtol(arg, &endptr, 0);
                    if ((value > 65535) || (value < -65536)) {
                        printf("error:s16 type arg error!(%d)\n", value);
                    }
                    *(int16_t *)target = value;
                }
                break;
            case ATT_TYPE_U8:
                {
                    uint32_t  value;
                    value = strtoul(arg, &endptr, 0);
                    if (value > 0xff) {
                        printf("error:u8 type arg error!(%u)\n", value);
                    }
                    *(uint8_t *)target = value;
                }
                break;
            case ATT_TYPE_S8:
                {
                    int32_t  value;
                    value = strtol(arg, &endptr, 0);
                    if ((value > 255) || (value < -256)) {
                        printf("error:s8 type arg error!(%d)\n", value);
                    }
                    *(int8_t *)target = value;
                }
                break;
            default:
                printf("error:type error!(%d)", type);
                return  (ERROR);
            }

            if (*endptr != '\0') {
                printf("error:numerical value error!(%s)", arg);
                return  (ERROR);
            }
        }

        /*
         * 按照目标数据类型调整目标地址
         */
        switch (type) {
        case ATT_TYPE_SPTR:
            target += sizeof(char *);
            break;
        case ATT_TYPE_SBUF:
            target += ATT_GET_BUF_SIZE(opt);
            break;
        case ATT_TYPE_U64:
        case ATT_TYPE_S64:
        case ATT_TYPE_F64:
            target += 8;
            break;
        case ATT_TYPE_EXIST:
        case ATT_TYPE_BOOL:
            target += sizeof(uint32_t);
            break;
        case ATT_TYPE_U32:
        case ATT_TYPE_S32:
        case ATT_TYPE_F32:
            target += 4;
            break;
        case ATT_TYPE_U16:
        case ATT_TYPE_S16:
            target += 2;
            break;
        case ATT_TYPE_U8:
        case ATT_TYPE_S8:
        case ATT_TYPE_CHAR:
            target += 1;
            break;
        default:
            printf("error:type error!(%d)", type);
            return  (ERROR);
        }
    }
    if (opt->targetArg) {
        *(int32_t *)opt->targetArg = count;
    }

    return  (NONE);
}
/*********************************************************************************************************
** 名称: optInit
** 输入: cmd        命令参数
** 输出: 错误码，NONE成功，ERROR错误
** 说明: 选项列表检查及初始化
*********************************************************************************************************/
static int optInit (command_st  *cmd)
{
    int         i;
    option_st  *opt;

    /*
     * 参数检测
     */
    if (cmd == NULL) {
        return  (ERROR);
    }
    if ((cmd->argc <= 0) || (cmd->argv == NULL) || (cmd->optc <= 0)  || (cmd->optv == NULL)) {
        return  (ERROR);
    }

    /*
     * 选项列表检查及初始化
     */
    for (i = 0; i < cmd->optc; i++) {
        opt = &cmd->optv[i];

        if (opt->target == NULL) {
            printf("error:option target can't null!");
            return  (ERROR);
        }

        if (opt->name == NULL) {
            printf("error:option name can't null!");
            return  (ERROR);
        }

        if (ATT_GET_TYPE(opt) == ATT_TYPE_SBUF) {
            if (ATT_GET_BUF_SIZE(opt) == 0) {
                printf("error:sbuf size can't be 0!");
                return  (ERROR);
            }
        }

        opt->index = 0;
        opt->argc  = 0;
    }

    return  (NONE);
}
/*********************************************************************************************************
** 名称: optGet
** 输入: cmd        命令参数
** 输出: 错误码，NONE成功，ERROR错误
** 说明: 检索命令行中的所有参数，解析出存在的选项位置，及其选项参数个数
*********************************************************************************************************/
static int optGet (command_st  *cmd)
{
    int         i;
    int         j;
    option_st  *opt;
    option_st  *optLast = NULL;

    /*
     * 对命令行参数进行遍历，找出其中的选项
     */
    for (i = 1; i < cmd->argc; i++) {

        /*
         * 用于统计选项参数个数
         */
        if (optLast) {
            optLast->argc++;
        }

        /*
         * 只有一个字符的命令参数一定不是选项，包括只有一个'-'号
         */
        if (strlen(cmd->argv[i]) == 1) {
            continue;
        }

        /*
         * 第一个字符不为'-'号的，一定不是选项
         */
        if (cmd->argv[i][0] != OPT_LABEL_CHAR) {
            continue;
        }

        /*
         * 第一个字符为'-'号，但第二个字符为数字的认为是个数值而不是选项
         */
        if ((cmd->argv[i][1] >= '0') && (cmd->argv[i][1] <= '9')) {
            continue;
        }

        /*
         * cmd 帮助信息
         */
        if (strcmp(&cmd->argv[i][1], "help") == 0) {
            help(cmd, 0);
            return  (ERROR);
        }
        /*
         * 检索该选项是否在选项列表中，存在的话，记录在命令行参数中的索引和选项参数个数
         * 列表中不存在的选项也认为是选项参数，可用于命令行嵌套
         */
        for (j = 0; j < cmd->optc; j++) {
            opt = &cmd->optv[j];

            if (strcmp(&cmd->argv[i][1], cmd->optv[j].name) == 0) {
                opt->index   = i;
                opt->argc    = 0;

                if (optLast) {
                    optLast->argc--;
                }
                optLast = opt;
                break;
            }
        }
    }

    return  (NONE);
}
/*********************************************************************************************************
** 名称: cmdParse
** 输入: cmd        命令参数
** 输出: 错误码，NONE成功，ERROR错误
** 说明: 命令解析
*********************************************************************************************************/
int cmdParse (command_st  *cmd)
{
    int         i;
    int         err;
    int         isDebug = 0;
    option_st  *opt;

    /*
     * 命令行最后一个参数为 INFO_CMD_STRING 时
     * cmd 内容打印，用于调试命令行处理函数本身
     */
    if (strcmp(cmd->argv[cmd->argc - 1], INFO_CMD_STRING) == 0) {
        cmd->argc--;
        isDebug = 1;
    }
    /*
     * 选项列表检查及初始化
     */
    err = optInit(cmd);
    if (NONE != err) {
        return  (err);
    }

    /*
     * 对选项列表进行检索
     */
    err = optGet(cmd);
    if (NONE != err) {
        return  (err);
    }

    /*
     * 先按照列表顺序获取所有目标数据或回调目标函数
     */
    for (i = 0; i < cmd->optc; i++) {
        opt = &cmd->optv[i];
        if (opt->att & ATT_FUNC) {
            if ((opt->index) || (opt->att & ATT_FORCE)) {
                optionCb_fp  pFunc;

                pFunc = (optionCb_fp)opt->target;
                err = pFunc(cmd, i);
                if (err < 0) {
                    return  (err);
                }
            }
        } else {
            err = targetSet(cmd, i);
            if (err != NONE) {
                return  (err);
            }
        }
    }

    /*
     * 为info命令时，用于调试命令行处理函数本身
     */
    if (isDebug) {
        info(cmd, 0);
        return  (ERROR);
    }

    return  (NONE);
}
/*********************************************************************************************************
  END
*********************************************************************************************************/
