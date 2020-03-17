/*********************************************************************************************************
** 文件: cmdParseExample.c
** 作者: scilogyhunter
** 站点: https://blog.csdn.net/scilogyhunter/category_9735056.html
** 描述: cmdParse 例程
*********************************************************************************************************/
#include "cmdParse.h"
/*********************************************************************************************************
  宏定义
*********************************************************************************************************/
#define CFG_DEFAULT_BUF_SIZE                (64)                        /*  缓冲区大小                  */
#define CFG_DEFAULT_ARRAY_SIZE              (4)                         /*  缓冲区个数                  */
/*********************************************************************************************************
  命令参数结构
*********************************************************************************************************/
typedef struct {
    uint32_t    e;
    uint32_t    b;
    char        c;
    uint8_t     u8;
    int8_t      s8;
    uint16_t    u16;
    int16_t     s16;
    uint32_t    u32;
    int32_t     s32;
    float       f32;
    uint64_t    u64;
    int64_t     s64;
    double      f64;
    char       *sptr;
    char        sbuf[CFG_DEFAULT_BUF_SIZE];

    int32_t     au8c;
    uint8_t     au8v[CFG_DEFAULT_ARRAY_SIZE];
    int32_t     abufc;
    char        abufv[CFG_DEFAULT_ARRAY_SIZE][CFG_DEFAULT_BUF_SIZE];
} CMD_ARG_ST;
/*********************************************************************************************************
** 名称: getPrint
** 输入: cmd        命令参数
**       opti       选项索引号
** 输出: 错误码，NONE成功，ERROR错误
** 说明: 帮助信息，列出参数列表信息
*********************************************************************************************************/
static  int  getPrint (command_st  *cmd, int opti)
{
    CMD_ARG_ST  *pArg = (CMD_ARG_ST  *)cmd->priv;
    option_st   *opt;
    uint32_t     i;

    opt  = &cmd->optv[opti];
    if (opt->index) {
        printf("-p option input!\n");
    } else {
        printf("-p option no input!\n");
    }

    printf("e   = %u\n",   pArg->e);
    printf("b   = %u\n",   pArg->b);
    printf("c   = '%c'\n", pArg->c);
    printf("u8  = %u\n",   pArg->u8);
    printf("s8  = %d\n",   pArg->s8);
    printf("u16 = %u\n",   pArg->u16);
    printf("s16 = %d\n",   pArg->s16);
    printf("u32 = %u\n",   pArg->u32);
    printf("s32 = %d\n",   pArg->s32);
    printf("f32 = %f\n",   pArg->f32);
    printf("u64 = %llu\n", pArg->u64);
    printf("s64 = %lld\n", pArg->s64);
    printf("f64 = %lf\n",  pArg->f64);
    printf("sptr= \"%s\"\n",   pArg->sptr);
    printf("sbuf= \"%s\"\n",   pArg->sbuf);

    printf("au8c  = %d, au8v data =", pArg->au8c);
    for (i = 0; i < pArg->au8c; i++) {
        if ((i % 8) == 0) {
            printf("\n[%08x]", i);
        }
        printf(" %02x", pArg->au8v[i]);
    }
    printf("\n");

    printf("abufc = %d, abufv data =\n", pArg->abufc);
    for (i = 0; i < pArg->abufc; i++) {
        printf("[%02d]%s\n", i, pArg->abufv[i]);
    }

    return  (NONE);
}
/*********************************************************************************************************
** 名称: cmdParseExample
** 输入: argc       参数个数
**       argv       参数列表
** 输出: 错误码，NONE成功，ERROR错误
** 说明: 文件读写速度测试工具函数
*********************************************************************************************************/
static int cmdParseExample (int argc, char *argv[])
{
    CMD_ARG_ST  cmdArg = {
        .e   = 0,
        .b   = 0,
        .c   = '1',
        .u8  = 2,
        .s8  = -3,
        .u16 = 4,
        .s16 = -5,
        .u32 = 6,
        .s32 = -7,
        .f32 = 8.0,
        .u64 = 9,
        .s64 = -10,
        .f64 = 11.0,
        .sptr= "sptr",
        .sbuf= "sbuf",

        .au8c = 0,
        .au8v = { 0 },
        .abufc = 0,
    };
    option_st  optv[] = {
        {ATT_TYPE_EXIST,&cmdArg.e,    "e",    "set e"},
        {ATT_TYPE_BOOL, &cmdArg.b,    "b",    "set b"},
        {ATT_TYPE_CHAR, &cmdArg.c,    "c",    "set c"},
        {ATT_TYPE_U8,   &cmdArg.u8,   "u8",   "set u8"},
        {ATT_TYPE_S8,   &cmdArg.s8,   "s8",   "set s8"},
        {ATT_TYPE_U16,  &cmdArg.u16,  "u16",  "set u16"},
        {ATT_TYPE_S16,  &cmdArg.s16,  "s16",  "set s16"},
        {ATT_TYPE_U32,  &cmdArg.u32,  "u32",  "set u32"},
        {ATT_TYPE_S32,  &cmdArg.s32,  "s32",  "set s32"},
        {ATT_TYPE_F32,  &cmdArg.f32,  "f32",  "set f32"},
        {ATT_TYPE_U64,  &cmdArg.u64,  "u64",  "set u64"},
        {ATT_TYPE_S64,  &cmdArg.s64,  "s64",  "set s64"},
        {ATT_TYPE_F64,  &cmdArg.f64,  "f64",  "set f64"},
        {ATT_TYPE_SPTR, &cmdArg.sptr, "sptr", "set sptr"},
        {ATT_BUF_SIZE(sizeof(cmdArg.sbuf)) |
         ATT_TYPE_SBUF, &cmdArg.sbuf, "sbuf", "set sbuf"},

        {ATT_ARRAY(GET_ARRAY_COUNT(cmdArg.au8v)) |
         ATT_TYPE_U8,   &cmdArg.au8v,  "au8", "set u8 array", &cmdArg.au8c},

        {ATT_ARRAY(GET_ARRAY_COUNT(cmdArg.abufv)) | ATT_BUF_SIZE(sizeof(cmdArg.abufv[0])) |
         ATT_TYPE_SBUF, &cmdArg.abufv, "abuf", "set sbuf array", &cmdArg.abufc},

        {ATT_FUNC |
         ATT_FORCE,     getPrint,       "p", "print value"},

    };
    command_st  cmd;
    int         err;

    cmd.argc = argc;
    cmd.argv = argv;
    cmd.optc = GET_OPTC(optv);
    cmd.optv = optv;
    cmd.priv = &cmdArg;

    err = cmdParse(&cmd);
    if (err == NONE) {
        printf("cmdParseExample ok!\n");
    } else {
        printf("cmdParseExample error = %d\n", err);
    }

    return  (NONE);
}
/*********************************************************************************************************
** 名称: main
** 描述: 程序入口
** 输入: argc   参数个数
**       argv   参数列表
** 输出: ERROR_CODE
*********************************************************************************************************/
int main (int argc, char **argv)
{
    cmdParseExample(argc, argv);

    return  (0);
}
/*********************************************************************************************************
  END
*********************************************************************************************************/
