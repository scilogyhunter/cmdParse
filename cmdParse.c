/*********************************************************************************************************
** �ļ�: cmdParse.c
** ����: scilogyhunter
** վ��: https://blog.csdn.net/scilogyhunter/category_9735056.html
** ����: ͨ�������в���������
*********************************************************************************************************/
#include "cmdParse.h"
/*********************************************************************************************************
** ����: help
** ����: cmd        �������
**       opti       ѡ��������
** ���: �����룬NONE�ɹ���ERROR����
** ˵��: ������Ϣ���г�����ѡ���б����Ҫ˵������ȥ��ÿ�������ظ�ʵ��
*********************************************************************************************************/
static  int  help (command_st  *cmd, int opti)
{
#define OPT_NAME_LENGTH_MIX             (8)                             /*  Ĭ�ϴ�ӡѡ������С����      */
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
** ����: info
** ����: cmd        �������
**       opti       ѡ��������
** ���: �����룬NONE�ɹ���ERROR����
** ˵��: ����ѡ���б��������ϸ��Ϣ������ʵ���������������
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
     * �����в�����ӡ
     */
    printf("argc = %d\n", cmd->argc);
    printf("argv :\n");
    for (i = 0; i < cmd->argc; i++) {
        printf("[%02d] = %s\n", i, cmd->argv[i]);
    }
    /*
     * ѡ���б���Ϣ��ӡ
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
     * ѡ���б��ȡֵ��ӡ
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
             * ����Ŀ���������͵���Ŀ���ַ
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
** ����: targetSet
** ����: cmd        �������
**       opti       ѡ��������
** ���: �����룬NONE�ɹ���ERROR����
** ˵��: ��Ŀ����и�ֵ
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
     * ���ѡ��û�����������г���
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
     * ����ATT_TYPE_EXIST�����⣬�������Ͷ�����Я��ѡ����������򱨴�
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
         * ����Ŀ���������ͽ��и�ֵ
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
         * ����Ŀ���������͵���Ŀ���ַ
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
** ����: optInit
** ����: cmd        �������
** ���: �����룬NONE�ɹ���ERROR����
** ˵��: ѡ���б��鼰��ʼ��
*********************************************************************************************************/
static int optInit (command_st  *cmd)
{
    int         i;
    option_st  *opt;

    /*
     * �������
     */
    if (cmd == NULL) {
        return  (ERROR);
    }
    if ((cmd->argc <= 0) || (cmd->argv == NULL) || (cmd->optc <= 0)  || (cmd->optv == NULL)) {
        return  (ERROR);
    }

    /*
     * ѡ���б��鼰��ʼ��
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
** ����: optGet
** ����: cmd        �������
** ���: �����룬NONE�ɹ���ERROR����
** ˵��: �����������е����в��������������ڵ�ѡ��λ�ã�����ѡ���������
*********************************************************************************************************/
static int optGet (command_st  *cmd)
{
    int         i;
    int         j;
    option_st  *opt;
    option_st  *optLast = NULL;

    /*
     * �������в������б������ҳ����е�ѡ��
     */
    for (i = 1; i < cmd->argc; i++) {

        /*
         * ����ͳ��ѡ���������
         */
        if (optLast) {
            optLast->argc++;
        }

        /*
         * ֻ��һ���ַ����������һ������ѡ�����ֻ��һ��'-'��
         */
        if (strlen(cmd->argv[i]) == 1) {
            continue;
        }

        /*
         * ��һ���ַ���Ϊ'-'�ŵģ�һ������ѡ��
         */
        if (cmd->argv[i][0] != OPT_LABEL_CHAR) {
            continue;
        }

        /*
         * ��һ���ַ�Ϊ'-'�ţ����ڶ����ַ�Ϊ���ֵ���Ϊ�Ǹ���ֵ������ѡ��
         */
        if ((cmd->argv[i][1] >= '0') && (cmd->argv[i][1] <= '9')) {
            continue;
        }

        /*
         * cmd ������Ϣ
         */
        if (strcmp(&cmd->argv[i][1], "help") == 0) {
            help(cmd, 0);
            return  (ERROR);
        }
        /*
         * ������ѡ���Ƿ���ѡ���б��У����ڵĻ�����¼�������в����е�������ѡ���������
         * �б��в����ڵ�ѡ��Ҳ��Ϊ��ѡ�������������������Ƕ��
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
** ����: cmdParse
** ����: cmd        �������
** ���: �����룬NONE�ɹ���ERROR����
** ˵��: �������
*********************************************************************************************************/
int cmdParse (command_st  *cmd)
{
    int         i;
    int         err;
    int         isDebug = 0;
    option_st  *opt;

    /*
     * ���������һ������Ϊ INFO_CMD_STRING ʱ
     * cmd ���ݴ�ӡ�����ڵ��������д���������
     */
    if (strcmp(cmd->argv[cmd->argc - 1], INFO_CMD_STRING) == 0) {
        cmd->argc--;
        isDebug = 1;
    }
    /*
     * ѡ���б��鼰��ʼ��
     */
    err = optInit(cmd);
    if (NONE != err) {
        return  (err);
    }

    /*
     * ��ѡ���б���м���
     */
    err = optGet(cmd);
    if (NONE != err) {
        return  (err);
    }

    /*
     * �Ȱ����б�˳���ȡ����Ŀ�����ݻ�ص�Ŀ�꺯��
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
     * Ϊinfo����ʱ�����ڵ��������д���������
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
